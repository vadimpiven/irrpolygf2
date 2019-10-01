/**
 * @file    Random.cpp
 * @author  Vadim Piven <vadim@piven.tech>
 * @date    2019/10/01
 * @license Free use of this library is permitted under the
 * guidelines and in accordance with the MIT License (MIT).
 * @url     https://github.com/vadimpiven/irrpolygf2
 */

#include <vector>

#include "Random.hpp"

/**
 * Неприводимые многочлены над GF[2] до степени 62 включительно.
 */
const uint_fast64_t irrPoly[63] = {
        0x0000000000000001ull,
        0x0000000000000003ull,
        0x0000000000000007ull,
        0x000000000000000Dull,
        0x0000000000000019ull,
        0x0000000000000029ull,
        0x0000000000000061ull,
        0x00000000000000C1ull,
        0x000000000000014Dull,
        0x0000000000000315ull,
        0x0000000000000481ull,
        0x0000000000000A01ull,
        0x0000000000001407ull,
        0x0000000000003601ull,
        0x0000000000005F19ull,
        0x000000000000C001ull,
        0x00000000000150D7ull,
        0x0000000000024001ull,
        0x000000000005FFFBull,
        0x00000000000E4001ull,
        0x0000000000120001ull,
        0x0000000000280001ull,
        0x0000000000600001ull,
        0x0000000000840001ull,
        0x0000000001B00001ull,
        0x0000000002400001ull,
        0x0000000006AC06ADull,
        0x000000000E400001ull,
        0x0000000011112D13ull,
        0x0000000028000001ull,
        0x000000005566A9A7ull,
        0x0000000090000001ull,
        0x0000000165656565ull,
        0x000000025A5A5A59ull,
        0x00000006AAAAAAADull,
        0x0000000A00000001ull,
        0x000000130000FF1Bull,
        0x0000003F00000001ull,
        0x0000006300000001ull,
        0x0000008800000001ull,
        0x0000013800000001ull,
        0x0000024000000001ull,
        0x0000073400000735ull,
        0x000009A000000001ull,
        0x00001313001D0FF1ull,
        0x0000360000000001ull,
        0x0000555500306667ull,
        0x0000840000000001ull,
        0x0001696800016969ull,
        0x0002380000000001ull,
        0x0005C00000000001ull,
        0x000D200000000001ull,
        0x0012000000000001ull,
        0x0038800000000001ull,
        0x005F000000000001ull,
        0x00E2000000000001ull,
        0x0152000000000001ull,
        0x0303030313750313ull,
        0x0630000000000001ull,
        0x0DE0000000000001ull,
        0x1800000000000001ull,
        0x3900000000000001ull,
        0x4B00000000000001ull,
};

/**
 * @param k число бит (от 1 до 62), которые нужно заполнить случайными данными.
 * @return случайное число длиной k бит (всегда 0, если k задано неверно).
 */
[[nodiscard]]
uint_fast64_t Random(const uint_fast8_t k) noexcept {
    static std::vector<uint_fast64_t> seed(63, 1);
    static std::vector<bool> zero(63, true);
    if (k == 0 || k > 62) { return 0; }
    if (zero[k]) { return zero[k] = false; }
    seed[k] <<= 1u;
    seed[k] ^= (seed[k] & (1ull << k)) ? irrPoly[k] : 0;
    zero[k] = seed[k] == 1;
    return seed[k];
}
