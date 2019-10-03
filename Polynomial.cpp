/**
 * @file    Polynomial.cpp
 * @author  Vadim Piven <vadim@piven.tech>
 * @date    2019/09/22
 * @license Free use of this library is permitted under the
 * guidelines and in accordance with the MIT License (MIT).
 * @url     https://github.com/vadimpiven/irrpolygf2
 */

#include <vector>

#include "Polynomial.hpp"

/**
 * Создаёт новый многочлен над полем GF[2^n], n = 0,...,63.
 * @param[in] val представляет многочлен как 64-битное целое число,
 * где представлению P(x) = a + b*x + c*x^2 соответствует число
 * с побитовым представлением ...cba, здесь a, b, c = 0 или 1,
 * т.к. коэффициенты многочлена лежат в поле GF[2].
 */
Polynomial::Polynomial(const uint_fast64_t val) noexcept : val(val) {}

/**
 * @return текущий многочлен, представленный как 64-битное целое число.
 */
[[nodiscard]]
uint_fast64_t Polynomial::Get() const noexcept {
    return val;
}

/**
 * Вычисляет производную многочлена над полем GF[2].
 * @return многочлен, являющийся производной данного.
 */
[[nodiscard]]
uint_fast64_t Polynomial::derivative() const noexcept {
    return (val & 0xAA'AA'AA'AA'AA'AA'AA'AAull) >> 1ull;
}

/**
 * Определяет, являются ли два многочлена взаимно простыми.
 * @param[in] p1 первый многочлен, старшей степени.
 * @param[in] p2 второй многочлен, младшей степени.
 * @return не взаимно просты ли два проверяемых многочлена,
 * ведущее отрицание используется для уменьшения числа выполняемых операций.
 */
[[nodiscard]]
uint_fast64_t Polynomial::gcd(
        uint_fast64_t p1, uint_fast64_t p2
) noexcept {
    uint_fast64_t temp;
    while (p2 != 0) {
        temp = mod(p1, p2, deg(p2));
        p1 = p2;
        p2 = temp;
    }
    return p1;
}

/**
 * Вычисляет степень многочлена, представленного как 64-битное число.
 * Для вычисления степени используется встроенная в процессоры архитектуры
 * AMD64 и ARM команда для подсчёта числа ведущих нулей в числе.
 * Поэтому данный код можно компилировать только с использованием GCC (Clang).
 * Степень многочлена находится вычитанием из 63 (максимальная возможная степень)
 * числа ведущих нулей. Например, если число ведущих нулей 0, значит
 * старший бит – единица, значит многочлен степени 63 = 63 - 0.
 * @param[in] p многочлен, степень от 0 до 63.
 * @return степень многочлена от 0 до 63.
 */
[[nodiscard]]
uint_fast8_t Polynomial::deg(const uint_fast64_t p) noexcept {
    if (p == 0) { return 0; }
    return static_cast<uint_fast8_t>
    (63ull - __builtin_clzll(static_cast<unsigned long long>(p)));
}

/**
 * @param p1 делимое.
 * @param p2 делитель.
 * @param degree степень делителя.
 * @return многочлен p1 по модулю многочлена p2.
 */
[[nodiscard]]
uint_fast64_t Polynomial::mod(
        uint_fast64_t p1, const uint_fast64_t p2, const uint_fast8_t degree
) noexcept {
    for (uint_fast8_t i = deg(p1); i >= degree && p1 != 0; i = deg(p1)) {
        p1 ^= (p2 << static_cast<uint_fast8_t>(i - degree));
    }
    return p1;
}

/**
 * Создаёт 128-битное число из 64-битного.
 * @param[in] p 64-битное число.
 */
Polynomial::uint_fast128_t::uint_fast128_t(
        const uint_fast64_t p
) noexcept : h(0), l(p) {}

/**
 * Создаёт 128-битное число из двух 64-битных.
 * @param[in] ph старшие 64 бита.
 * @param[in] pl младшие 64 бита.
 */
Polynomial::uint_fast128_t::uint_fast128_t(
        const uint_fast64_t ph, const uint_fast64_t pl
) noexcept : h(ph), l(pl) {}

/**
 * Вычисляет степень многочлена над GF[2], представленного 128-битным числом.
 * @return степень входного многочлена от 0 до 127.
 */
[[nodiscard]]
uint_fast8_t Polynomial::uint_fast128_t::deg() const noexcept {
    if (h == 0) {
        if (l == 0) { return 0; }
        return static_cast<uint_fast8_t>
        (63ull - __builtin_clzll(static_cast<unsigned long long>(l)));
    }
    return static_cast<uint_fast8_t>
    (127ull - __builtin_clzll(static_cast<unsigned long long>(h)));
}

/**
 * Выполняет побитовый сдвиг 128-битного числа.
 * @param[in] n на сколько бит сдвинуть.
 * @return новое число, представляющее из себя сдвинутое старое.
 */
[[nodiscard]]
Polynomial::uint_fast128_t Polynomial::uint_fast128_t::operator<<(
        const uint_fast8_t n
) const noexcept {
    auto res = uint_fast128_t(h << n, n >= 64 ? 0 : l << n);
    if (deg() + n > 63u) { res.h |= (n > 64u) ? (l << (n - 64u)) : (l >> (64u - n)); }
    return res;
}

/**
 * Реализует операцию XOR.
 * @param[in] p 128-битное число, с которым необходимо сложить данное по модулю 2.
 */
void Polynomial::uint_fast128_t::operator^=(const uint_fast128_t p) noexcept {
    h ^= p.h;
    l ^= p.l;
}

/**
 * @return младшие 64 бита 128-битного числа (предполагается, что старшие пусты).
 */
[[nodiscard]]
uint_fast64_t Polynomial::uint_fast128_t::get() const noexcept {
    return l;
}

/**
 * @return является ли данное 128-битное число нулём.
 */
[[nodiscard]]
bool Polynomial::uint_fast128_t::notZero() const noexcept {
    return l != 0 || h != 0;
}

/**
 * @param p1 делимое.
 * @param p2 делитель.
 * @param degree степень делителя.
 * @return многочлен p1 по модулю многочлена p2.
 */
[[nodiscard]]
uint_fast64_t Polynomial::mod(
        uint_fast128_t p1, const uint_fast128_t p2, const uint_fast8_t degree
) noexcept {
    for (uint_fast8_t i = p1.deg(); i >= degree && p1.notZero(); i = p1.deg()) {
        p1 ^= (p2 << static_cast<uint_fast8_t>(i - degree));
    }
    return p1.get();
}

/**
 * Выполняет последний шаг алгоритма Берлекампа проверки многочлена на неприводимость.
 * Строится матрица M[nxn], где строки - коэффициенты многочлена x^(ip) (mod P(x)),
 * где p - двойка, 0 < i < n, P(x) - текущий многочлен над полем GF[2] степени n.
 * Подробное описание и пример расчёта можно найти в статье
 * "A Formalization of Berlekamp’s Factorization Algorithm" по ссылке
 * http://www21.in.tum.de/~nipkow/Isabelle2016/Isabelle2016_6.pdf (стр. 3-4).
 * Матрица строится в зеркально отражённом виде для оптимизации хранимых данных.
 * Затем из матрицы вычитается единичная матрица.
 * В конце вычисляется ранг получившейся матрицы. Если n - rgM = 1, то, с учётом
 * выполненных до вызова данной функции проверок, данный многочлен неприводим.
 * @param[in] degree степень текущего многочлена, от 1 до 63,
 * не проверяется корректность для уменьшания числа выполняемых операций.
 * @return является ли данный многочлен степени n неприводимы над полем GF[2],
 * с учётом предварительных проверок, ведущее отрицание используется для
 * уменьшения числа выполняемых операций.
 */
[[nodiscard]]
bool Polynomial::notBerlekampFinal(const uint_fast8_t degree) const noexcept {
    std::vector<uint_fast64_t> M(degree);
    uint_fast64_t temp;
    uint_fast8_t i, j, k;
    bool flag;

    for (i = 0; i < degree; ++i) {
        // x ^ ip, p = 2
        temp = i < 32 ?
               // случай, когда x ^ ip влезает в 64 бита
               mod(1ull << static_cast<uint_fast8_t>(i * 2), val, degree) :
               // случай, когда x ^ ip не влезает в 64 бита
               mod(uint_fast128_t(1ull) << static_cast<uint_fast8_t>(i * 2),
                   uint_fast128_t(val), degree);
        M[i] = temp ^ (1ull << i); // M -= E (mod 2)
    }

    // приведение матрицы к ступенчатому виду
    for (i = k = 0, temp = 1; i < degree && k < degree; ++k, temp <<= 1ull) {
        flag = M[i] & temp;
        for (j = i + 1; j < degree; ++j) {
            if (M[j] & temp) {
                switch (flag) { // ~if-else
                case false: // swap
                    M[i] ^= M[j];
                    M[j] = M[i] ^ M[j];
                    M[i] ^= M[j];
                    flag = true;
                    continue;
                default: // Mj -= Mi
                    M[j] ^= M[i];
                }
            }
        }
        i += flag;
    }
    return i + 1 != degree;
}

/**
 * Определяет, является ли данный многочлен степени n неприводимым в поле GF[2].
 * Для определения неприводимости используется алгоритм Берлекампа.
 * Первый шаг - вычисление производной данного многочлена. Если производная
 * равна нулю, то многочлен является степенью какого-то другого многочлена,
 * то есть он приводим.
 * Второй шаг - поиск общих множителей многочлена и его производной.
 * Если общие множители (многочлены, а не числа) есть, т.е. многочлены
 * не взаимно просты, то f(x) делится на них, т.е. он не неприводим.
 * Третий шаг - простоение матрицы Берлекампа и вычисление её ранга.
 * @param[in] degree степень текущего многочлена, от 1 до 63,
 * не проверяется корректность для уменьшания числа выполняемых операций.
 * @return является ли данный многочлен степени n неприводимы над полем GF[2].
 */
[[nodiscard]]
bool Polynomial::IsIrredusible(const uint_fast8_t degree) const noexcept {
    auto pp = derivative();
    return !(pp == 0 || gcd(val, pp) != 1
            || notBerlekampFinal(degree));
}
