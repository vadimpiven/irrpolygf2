/**
 * @file    Checker.hpp
 * @author  Vadim Piven <vadim@piven.tech>
 * @date    2019/09/22
 * @license Free use of this library is permitted under the
 * guidelines and in accordance with the MIT License (MIT).
 * @url     https://github.com/vadimpiven/irrpolygf2
 */

#ifndef BERLEKAMP_CHECKER_HPP
#define BERLEKAMP_CHECKER_HPP

#include <pthread.h>
#include "Polynomial.hpp"

class Checker {
    Polynomial poly;
    uint_fast8_t deg;

    pthread_mutex_t *mutex;
    pthread_cond_t *cond;

    bool notDone;
    bool irr;

public:
    explicit
    Checker(pthread_mutex_t *, pthread_cond_t *) noexcept;

    void Set(uint_fast64_t, uint_fast8_t) noexcept;

    static
    void *Check(void *arg) noexcept;

    [[nodiscard]]
    uint_fast64_t Get() const noexcept;

    [[nodiscard]]
    bool IsNotDone() const noexcept;

    [[nodiscard]]
    bool IsIrreducible() const noexcept;
};

#endif //BERLEKAMP_CHECKER_HPP
