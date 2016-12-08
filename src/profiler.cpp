/*
 * profiler.cpp
 *
 * tests data structure performance
 * under different situations.
 *
 * Derek Johnson
 * Graham Zuber
 *
 * 12.8.16
 */

#include "profiler.h"

#define NUM_REPS 10
#define NUM_THREADS 10
#define NUM_ACCESSES 1000000

/* htm globals */
tsx_mutex *htm_mu;

/* pthread globals */
pthread_t *threads;
pthread_mutex_t p_mu = PTHREAD_MUTEX_INITIALIZER;

/* global shared access array */
elem *shared;

void* speculative_lock(void *indexp)
{
    int index = *(int*)indexp;
    free(indexp);
    
    for (long i = 0; i < NUM_ACCESSES; ++i)
    {
        tsx_scoped_mutex temp_lock(*htm_mu);
        shared[index].val++;
        temp_lock.release();
    }

    return NULL;
}

void* pthread_lock(void *indexp)
{
    int index = *(int*)indexp;
    free(indexp);
    
    for (long i = 0; i < NUM_ACCESSES; ++i)
    {
        pthread_mutex_lock(&p_mu);
        shared[index].val++;
        pthread_mutex_unlock(&p_mu);
    }

    return NULL;
}

void* no_lock(void *indexp)
{
    int index = *(int*)indexp;
    free(indexp);
    
    for (long i = 0; i < NUM_ACCESSES; ++i)
    {
        shared[index].val++;
    }

    return NULL;
}


void run_test(int num_threads, test_func func_call)
{
    // loop through and create threads
    for (int i = 0; i < num_threads; ++i)
    {
        int* x = (int*) malloc(sizeof(int));
        *x = i;
        pthread_create(&threads[i], NULL, func_call, x);
    }

    // wait for threads
    for (int i = 0; i < num_threads; ++i)
    {
        pthread_join(threads[i], NULL);
        if(shared[i].val != NUM_ACCESSES)
        {
            std::cout << "Error. Thread " << i << " only counted to " << shared[i].val << std::endl;
        }
    }
}


int main(int argc, char* argv[])
{
    // initialize the htm mutex
    htm_mu = new tsx_mutex();
    
    // initialize the test calls
    char const *tests[] = {"none", "pthread", "tsx"};
    test_func test_funcs[] = {no_lock, pthread_lock, speculative_lock};

    // init the cycler
    cycles cycler;
    if(!cycler.init())
    {
        std::cout << "nope. cycles::init failed" << std::endl;
        return 1;
    }

    // loop for repition
    for(int num_rep = 0; num_rep < NUM_REPS; ++num_rep)
    {
        // loop for different tests
        for(size_t test_type = 0; test_type < sizeof(tests); ++test_type)
        {
            std::cout << "*** Testing " << tests[test_type] << " ***" << std::endl;

            // loop for testing with different amounts of threads
            for (int num_of_thds = 0; num_of_thds < NUM_THREADS; ++num_of_thds)
            {
                shared = (elem*) calloc(num_of_thds, sizeof(elem));
                threads = (pthread_t*) calloc(num_of_thds, sizeof(pthread_t));

                uint64_t start = __rdtsc();

                run_test(num_of_thds, test_funcs[test_type]);

                uint64_t end = __rdtsc();

                std::cout << num_of_thds << "\t" << cycler.to_seconds(end - start) << std::endl;

                // clean up
                free(shared);
                free(threads);
            }
        }
    }

}







