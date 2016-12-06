/*
 * frand.h
 *
 * frand is a faster random number generator.
 * this was lifted straight from intel.
 */

#pragma once

class frand
{
    public:
        frand();
        frand(int);
        void seed(int);
        int gen();

    private:
        unsigned int f_seed;
};
