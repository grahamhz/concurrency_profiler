/*
 * cycles.cpp
 *
 * this is an object meant to help with
 * timing the experiment.
 * it determines the number of cpu cycles
 * in a second.
 * this is lifted directly from ramcloud.
 */

#include "cycles.h"

cycles::cycles()
{
    cycles_per_sec = 0;
}

bool cycles::init()
{
    if(cycles_per_sec != 0)
    {
        return true;
    }

    struct timeval start_time, stop_time;
    uint64_t start_cycles, stop_cycles, micros;
    double old_cycles = 0;

    while(1)
    {
        if(gettimeofday(&start_time, NULL) != 0)
        {
            return false;
        }
        start_cycles = __rdtsc();

        while(1)
        {
            if(gettimeofday(&stop_time, NULL) != 0)
            {
                return false;
            }
            stop_cycles = __rdtsc();

            micros = (stop_time.tv_usec - start_time.tv_usec) + 
                (stop_time.tv_sec - start_time.tv_sec) * 1000000;
            if(micros > 10000)
            {
                cycles_per_sec = static_cast<double>(stop_cycles - start_cycles);
                cycles_per_sec = 1000000.0 * cycles_per_sec / static_cast<double>(micros);
                break;
            }
        }

        double delta = cycles_per_sec / 1000.0;
        if((old_cycles > (cycles_per_sec - delta)) && 
                (old_cycles < (cycles_per_sec + delta)))
        {
            return true;
        }
        old_cycles = cycles_per_sec;
    }
}


double cycles::get_cycles_per_sec()
{
    return cycles_per_sec;
}


double cycles::to_seconds(uint64_t cycles)
{
    if(cycles_per_sec == 0)
    {
        return 0;
    }
    return (static_cast<double>(cycles) / cycles_per_sec);
}
        



