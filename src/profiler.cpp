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

// map related defs

typedef tbb::speculative_spin_mutex tsx_mutex;

tsx_mutex mu;
// Needs padding to make each struct 128 
struct elem {
    uint64_t val;
    uint64_t padding;
};

long NUM_ACCESSES = 1000000;
int NUM_THREADS;

elem *shared;
pthread_t *threads;

// pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void * do_it(void *indexp)
{
    int index = *(int*)indexp;
    
    for (long i = 0; i < NUM_ACCESSES; ++i)
    {
        // std::cout << "Before lock" << std::endl;
        tbb::speculative_spin_mutex::scoped_lock lock(mu);
        shared[index].val++;
        // std::cout << "Inside lock" << std::endl;
        
        // std::cout << "After lock" << std::endl;

    }

    return NULL;
}


void run_test()
{
    shared = (elem *) calloc(NUM_THREADS, sizeof(elem));
    threads = (pthread_t *) calloc(NUM_THREADS, sizeof(pthread_t));

    for (int i = 0; i < NUM_THREADS; ++i)
    {
        int* x = (int *) malloc(sizeof(int));
        *x = i;
        pthread_create(&threads[i], NULL, do_it, x);
    }


    for (int i = 0; i < NUM_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
        if(shared[i].val != NUM_ACCESSES)
            std::cout << "Error. Thread " << i << " only counted to " << shared[i].val << std::endl;
    }

}


int main(int argc, char* argv[])
{

    // std::cout << "Testing " << NUM_THREADS << " threads on " << NUM_ACCESSES << " accesses " << std::endl;
    std::cout << "Testing htm" << std::endl;
    for (int i = 1; i <= 10; ++i)
    {
        NUM_THREADS = i;

        uint64_t first = __rdtsc();


        run_test();


        uint64_t second = __rdtsc();

        cycles cycler;
        if(!cycler.init())
        {
            std::cout << "nope. cycles::init failed" << std::endl;
            return 1;
        }

        // std::cout << "took: " << cycler.to_seconds(second - first) << " seconds" << std::endl;
        std::cout << NUM_THREADS << "\t" << cycler.to_seconds(second - first) << std::endl;
    }

}







