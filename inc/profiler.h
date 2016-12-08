/*
 * profiler.h
 *
 * tests data structure performance
 * under different situations.
 *
 * Derek Johnson
 * Graham Zuber
 *
 * 12.8.16
 */

#include <iostream>
#include <string>
#include <unordered_map>
#include <unistd.h>
#include <x86intrin.h>
#include <semaphore.h>
#include <inttypes.h>
#include "tbb/concurrent_unordered_map.h"
#include "tbb/spin_mutex.h"
#include "frand.h"
#include "cycles.h"

/* htm typedefs */
typedef tbb::speculative_spin_mutex tsx_mutex;
typedef tbb::speculative_spin_mutex::scoped_lock tsx_scoped_mutex;

/* function pointer typedef */
typedef void* (*test_func)(void*);

/* accessed array struct */
struct elem {
    uint64_t val;
    uint64_t padding[7];
};
