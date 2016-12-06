/*
 * cycles.h
 *
 * this is an object meant to help with
 * timing the experiment.
 * it determines the number of cpu cycles
 * in a second.
 * this is lifted directly from ramcloud.
 */

#pragma once

#include <sys/time.h>
#include <errno.h>
#include <stdint.h>
#include <string>
#include <x86intrin.h>


class cycles
{
    public:
        cycles();
        bool init();
        double get_cycles_per_sec();
        double to_seconds(uint64_t);

    private:
        double cycles_per_sec;
};
