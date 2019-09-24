/**
 * @file    Generator.cpp
 * @author  Vadim Piven <vadim@piven.tech>
 * @date    2019/09/22
 * @license Free use of this library is permitted under the
 * guidelines and in accordance with the MIT License (MIT).
 * @url     https://github.com/vadimpiven/irrpolygf2
 */

#include <random>
#include <thread>
#include <pthread.h>

#include "Generator.hpp"

/**
 * @param[in] c список текущих проверщиков
 * @return число активно работающих проверщиков
 */
[[nodiscard]]
uint_fast8_t Generator::countBusy(const std::vector<Checker> &c) noexcept {
    uint_fast8_t n = 0;
    for (auto &checker : c) { n += checker.IsNotDone(); }
    return n;
}

/**
 * Генерирует неприводимый многочлен заданной степени, используя несколько потоков.
 * Генерация проходит путём последовательной проверки случайно выбранных многочленов
 * заданной степени. Всегда выбираются многочлены, у которых младший и старший
 * коэффициенты не нулевые. Случай degree = 1 не подходит под это правило,
 * т.к. все полиномы первой степени неприводимы над полем GF[2], поэтому один
 * из них возвращается случайным образом (случай рассмотрен отдельно)
 * @param[in] degree степень генерируемого многочлена, от 1 до 63,
 * не проверяется корректность для уменьшания числа выполняемых операций.
 * @param[in] threadsNum число потоков.
 * @return неприводимый многочлен требуемой степени,
 * 1 в случае ошибки pthread.
 */
[[nodiscard]]
uint_fast64_t Generator::generate(const uint_fast8_t degree, const uint_fast8_t threadsNum) noexcept {
    // всё для генерации случайных чисел в заданном диапазоне, C++11
    static std::random_device rd;
    static std::mt19937_64 gen(rd());
    static std::uniform_int_distribution<uint_fast64_t> binDis(0, 1);

    if (degree == 1) {
        // случайным образом возвращаем либо x, либо x+1
        return binDis(gen) & 1ul ? 0x00'00'00'02 : 0x00'00'00'03;
    }
    uint_fast64_t res; // возвращаемое значение

    // всё для работы с потоками
    pthread_t threads[threadsNum];
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutexattr_t attr;
    pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

    if (pthread_mutexattr_init(&attr) ||
            pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK) ||
            pthread_mutex_init(&mutex, &attr) ||
            pthread_cond_init(&cond, nullptr)) { return 1; }

    // по одному проверщику в каждом потоке
    auto c = std::vector<Checker>(threadsNum, Checker(&mutex, &cond));
    // генератор случайных чисел для формирования многочленов на проверку
    std::uniform_int_distribution<uint_fast64_t> dis(0, (1ul << (degree - 1ul)) - 1);

    pthread_mutex_lock(&mutex);
    while (true) {
        while (countBusy(c) >= threadsNum) {
            // ждём изменения числа занятых потоков
            pthread_cond_wait(&cond, &mutex);
        }

        for (uint_fast8_t j = 0; j < threadsNum; ++j) {
            // находим первый свободный поток
            if (c[j].IsNotDone()) { continue; }
            if (c[j].IsIrreducible()) {
                res = c[j].Get();
                goto END;
            }
            // генерируем случайный многочлен для проверки
            // младший и старший коэффициенты всегда единицы
            c[j].Set((1ul << degree) | (dis(gen) << 1ul) | 1ul, degree);
            // создаём новый поток для выполнения проверки
            pthread_create(&threads[j], nullptr, &Checker::Check, &c[j]);
            // отсоединеняем поток
            pthread_detach(threads[j]);
            break;
        }
    }
    END:
    // ждём завершения всех созданных потоков
    while (countBusy(c)) {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_unlock(&mutex);
    if (pthread_cond_destroy(&cond) || pthread_mutex_destroy(&mutex)
        || pthread_mutexattr_destroy(&attr)) { return 1; }
    return res;
}

/**
 * Генерирует неприводимый многочлен заданной степени.
 * При вызове подсчитывает число доступных потоков обращением к std::threads.
 * @param[in] degree степень многочлена в пределах от 1 до 63,
 * проверка попадания в эти границы выполняется.
 * @return неприводимый многочлен требуемой степени,
 * 0 в случае если degree задан некорректно,
 * 1 в случае ошибки pthread.
 */
[[nodiscard]]
uint_fast64_t Generator::GetIrrPoly(const uint_fast8_t degree) noexcept {
    if (degree == 0 || degree > 63) { return 0; }
    static const uint_fast8_t cores = std::thread::hardware_concurrency();
    return generate(degree, cores > 0 ? cores : 1);
}
