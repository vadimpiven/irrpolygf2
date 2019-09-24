/**
 * @file    Generator.hpp
 * @author  Vadim Piven <vadim@piven.tech>
 * @date    2019/09/22
 * @license Free use of this library is permitted under the
 * guidelines and in accordance with the MIT License (MIT).
 * @url     https://github.com/vadimpiven/irrpolygf2
 */

#ifndef BERLEKAMP_GENERATOR_HPP
#define BERLEKAMP_GENERATOR_HPP

#include <cstdint>
#include <vector>
#include "Checker.hpp"

class Generator {
    [[nodiscard]] static
    uint_fast8_t countBusy(const std::vector<Checker> &) noexcept;

    [[nodiscard]] static
    uint_fast64_t generate(uint_fast8_t, uint_fast8_t) noexcept;

public:
    [[nodiscard]] static
    uint_fast64_t GetIrrPoly(uint_fast8_t) noexcept;
};

#endif //BERLEKAMP_GENERATOR_HPP
