/**
 * @file    Checker.cpp
 * @author  Vadim Piven <vadim@piven.tech>
 * @date    2019/09/22
 * @license Free use of this library is permitted under the
 * guidelines and in accordance with the MIT License (MIT).
 * @url     https://github.com/vadimpiven/irrpolygf2
 */

#include "Checker.hpp"

/**
 * Инициализирует новый объекта-проверщик, используемый для
 * распараллеливания проверки многочленов на неприводимость.
 * @param[in] mutex мьютекс, относящийся к cond.
 * @param[in] cond используется для сигнализирования об окончании работы.
 */
Checker::Checker(pthread_mutex_t *mutex, pthread_cond_t *cond) noexcept :
        poly(0), deg(0), mutex(mutex), cond(cond), notDone(false), irr(false) {}

/**
 * @param p многочлен для проверки на неприводимость.
 * @param degree степень многочлена p, от 1 до 63,
 * не проверяется корректность для уменьшания числа выполняемых операций.
 */
void Checker::Set(const uint_fast64_t p, const uint_fast8_t degree) noexcept {
    poly = Polynomial(p);
    deg = degree;
    irr = false;
    notDone = true;
}

/**
 * Используется для выполнения проверки на неприводимость в отдельном потоке pthread.
 * @param arg экземпляр Checker с установленным для проверки многочленом.
 * @return никакое значение не возвращается, т.к. выполнение функции,
 * как и работа текущего потока, прекращается вызовом pthread_exit.
 */
void *Checker::Check(void *arg) noexcept {
    auto *c = static_cast<Checker *>(arg);

    c->irr = c->poly.IsIrredusible(c->deg);

    pthread_mutex_lock(c->mutex);
    pthread_cond_signal(c->cond);
    pthread_mutex_unlock(c->mutex);

    c->notDone = false;

    pthread_exit(nullptr);
}

/**
 * @return многочлен, для которого выполнялась проверка на неприводимость.
 */
[[nodiscard]]
uint_fast64_t Checker::Get() const noexcept {
    return poly.Get();
}

/**
 * @return закончена ли проверка на неприводимость.
 */
[[nodiscard]]
bool Checker::IsNotDone() const noexcept {
    return notDone;
}

/**
 * @return неприводим ли многочлен, для которого была выполнена проверка.
 */
[[nodiscard]]
bool Checker::IsIrreducible() const noexcept {
    return irr;
}
