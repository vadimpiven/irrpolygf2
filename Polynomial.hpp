/**
 * @file    Polynomial.hpp
 * @author  Vadim Piven <vadim@piven.tech>
 * @date    2019/09/22
 * @license Free use of this library is permitted under the
 * guidelines and in accordance with the MIT License (MIT).
 * @url     https://github.com/vadimpiven/irrpolygf2
 */

#ifndef BERLEKAMP_POLYNOMIAL_HPP
#define BERLEKAMP_POLYNOMIAL_HPP

#include <cstdint>

class Polynomial {
    uint_fast64_t val;

    [[nodiscard]]
    uint_fast64_t derivative() const noexcept;

    [[nodiscard]] static
    uint_fast64_t gcd(uint_fast64_t, uint_fast64_t) noexcept;

    [[nodiscard]] static
    uint_fast8_t deg(uint_fast64_t) noexcept;

    [[nodiscard]] static
    uint_fast64_t mod(uint_fast64_t, uint_fast64_t, uint_fast8_t) noexcept;

    class uint_fast128_t {
        uint_fast64_t h;
        uint_fast64_t l;

    public:
        explicit
        uint_fast128_t(uint_fast64_t) noexcept;

        explicit
        uint_fast128_t(uint_fast64_t, uint_fast64_t) noexcept;

        [[nodiscard]]
        uint_fast8_t deg() const noexcept;

        [[nodiscard]]
        uint_fast128_t operator<<(uint_fast8_t) const noexcept;

        void operator^=(uint_fast128_t) noexcept;

        [[nodiscard]]
        uint_fast64_t get() const noexcept;

        [[nodiscard]]
        bool notZero() const noexcept;
    };

    [[nodiscard]]
    static uint_fast64_t mod(uint_fast128_t, uint_fast128_t, uint_fast8_t) noexcept;

    [[nodiscard]]
    bool berlekampFinal(uint_fast8_t) const noexcept;

public:
    explicit
    Polynomial(uint_fast64_t) noexcept;

    [[nodiscard]]
    bool IsIrredusible(uint_fast8_t) const noexcept;

    [[nodiscard]]
    uint_fast64_t Get() const noexcept;
};

#endif //BERLEKAMP_POLYNOMIAL_HPP
