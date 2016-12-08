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
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

tsx_mutex mu;
// Needs padding to make each struct 128 
struct elem {
    uint64_t val;
    uint64_t padding;
};

long NUM_ACCESSES = 1000000;

elem *shared;
pthread_t *threads;


void * speculative_lock(void *indexp)
{
    int index = *(int*)indexp;
    
    for (long i = 0; i < NUM_ACCESSES; ++i)
    {
        tbb::speculative_spin_mutex::scoped_lock lock(mu);
        shared[index].val++;
    }

    return NULL;
}

void * pthread_lock(void *indexp)
{
    int index = *(int*)indexp;
    
    for (long i = 0; i < NUM_ACCESSES; ++i)
    {
        pthread_mutex_lock(&mutex);
        shared[index].val++;
        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void * no_lock(void *indexp)
{
    int index = *(int*)indexp;
    
    for (long i = 0; i < NUM_ACCESSES; ++i)
    {
        shared[index].val++;
    }

    return NULL;
}


void run_test(int num_threads, test_func)
{



    for (int i = 0; i < num_threads; ++i)
    {
        int* x = (int *) malloc(sizeof(int));
        *x = i;
        pthread_create(&threads[i], NULL, test_func, x);
    }


    for (int i = 0; i < num_threads; ++i)
    {
        pthread_join(threads[i], NULL);
        if(shared[i].val != NUM_ACCESSES)
            std::cout << "Error. Thread " << i << " only counted to " << shared[i].val << std::endl;
    }

}


int main(int argc, char* argv[])
{
    char** tests = ["none", "pthread", "tsx"];

    void* functions []

    std::cout << "Testing htm" << std::endl;
    for (int i = 1; i <= 10; ++i)
    {

        shared = (elem *) calloc(NUM_THREADS, sizeof(elem));
        threads = (pthread_t *) calloc(NUM_THREADS, sizeof(pthread_t));
        
        uint64_t start = __rdtsc();

        run_test(i, functions[i]);

        uint64_t end = __rdtsc();

        cycles cycler;
        if(!cycler.init())
        {
            std::cout << "nope. cycles::init failed" << std::endl;
            return 1;
        }

        std::cout << NUM_THREADS << "\t" << cycler.to_seconds(end - start) << std::endl;
    }

}







