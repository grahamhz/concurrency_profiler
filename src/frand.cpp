/*
 * frand.cpp
 *
 * frand is a faster random number generator.
 * this was lifted straight from intel.
 */

#include "frand.h"

frand::frand() {}

frand::frand(int seed): f_seed(seed) {}

void frand::seed(int seed)
{
    f_seed = seed;
}

int frand::gen()
{
    f_seed = (214013 * f_seed + 2531011);
    return (f_seed >> 16) & 0x7FFF;
}
