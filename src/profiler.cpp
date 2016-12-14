/*
 * profiler.cpp
 *
 * tests data structure performance
 * under different situations.
 *
 * Graham Zuber
 * Derek Johnson
 *
 * 12.8.16
 */

#include "profiler.h"
#include "frand.h"
#include <stdlib.h>
#include <stdio.h>

#define NUM_REPS 5
#define NUM_THREADS 4
#define NUM_OPS 1000000

/* htm globals */
tsx_mutex *tsx_mu;

/* spin lock globals */
spin_lock *spin_mu; // for coarse locking
spin_lock **spin_mu_array; // lock array for fine-grained locking

/* pthread globals */
pthread_t *threads;

/* global shared access data structures */
uint64_t shared_len;
elem *shared;
elem_small *shared_small;

/* fast random globals */

/********** ARRAY ACCESSES **********/
/*
 * speculative_spin_lock_array
 * tests array accesses with a 
 * speculative spin lock.
 *
 * indexp: index of array to access
 *
 * return void
 */
void* speculative_spin_lock_array(void *x)
{
    frand fast_random(time(NULL));
    for (long i = 0; i < NUM_OPS; ++i)
    {
        int index = fast_random.gen() % shared_len;
        
        tsx_scoped_mutex temp_lock(*tsx_mu);
        shared[index].val++;
        temp_lock.release();
    }

    return NULL;
}

/*
 * spin_lock_array
 * tests array accesses with a 
 * spin lock.
 *
 * indexp: index of array to access
 *
 * return void
 */
void* spin_lock_array(void *x)
{
    frand fast_random(time(NULL));
    for (long i = 0; i < NUM_OPS; ++i)
    {
        int index = fast_random.gen() % shared_len;
        
        scoped_spin_lock temp_lock(*spin_mu);
        shared[index].val++;
        temp_lock.release();
    }

    return NULL;
}


/*
 * no_lock_array
 * tests array accesses with no lock
 * indexp: index of array to access
 *
 * return void
 */
void* no_lock_array(void *x)
{
    frand fast_random(time(NULL));
    for (long i = 0; i < NUM_OPS; ++i)
    {
        int index = fast_random.gen() % shared_len;

        shared[index].val++;
    }

    return NULL;
}


void* fine_grained_spin_lock_array(void *x)
{
    frand fast_random(time(NULL));
    for (long i = 0; i < NUM_OPS; ++i)
    {

        // scoped_spin_lock temp_lock_mutual(*spin_mu);


        int index = fast_random.gen() % shared_len;
        // std::cout << "index: " << index << std::endl << std::flush;
        scoped_spin_lock temp_lock(*(spin_mu_array[index]));
        // std::cout << "Lock " << index << " taken" << std::endl << std::flush;
        shared[index].val++;
        temp_lock.release();
        // std::cout << "Lock " << index << " released\n" << std::endl << std::flush;


        // temp_lock_mutual.release();
    }

    return NULL;
}


/********** SMALL ARRAY ACCESSES **********/
/*
 * speculative_spin_lock_array_small
 * tests array accesses with a 
 * speculative spin lock.
 *
 * indexp: index of array to access
 *
 * return void
 */
void* speculative_spin_lock_array_small(void *x)
{
    frand fast_random(time(NULL));
    for (long i = 0; i < NUM_OPS; ++i)
    {
        int index = fast_random.gen() % shared_len;

        tsx_scoped_mutex temp_lock(*tsx_mu);
        shared_small[index].val++;
        temp_lock.release();
    }

    return NULL;
}

/*
 * spin_lock_array_small
 * tests array accesses with a 
 * spin lock.
 *
 * indexp: index of array to access
 *
 * return void
 */
void* spin_lock_array_small(void *x)
{
    frand fast_random(time(NULL));
    for (long i = 0; i < NUM_OPS; ++i)
    {
        int index = fast_random.gen() % shared_len;
        
        scoped_spin_lock temp_lock(*spin_mu);
        shared_small[index].val++;
        temp_lock.release();
    }

    return NULL;
}


/*
 * no_lock_array_small
 * tests array accesses with no lock
 *
 * indexp: index of array to access
 *
 * return void
 */
void* no_lock_array_small(void *x)
{
    frand fast_random(time(NULL));
    for (long i = 0; i < NUM_OPS; ++i)
    {
        int index = fast_random.gen() % shared_len;
        
        shared_small[index].val++;
    }

    return NULL;
}

void* fine_grained_spin_lock_array_small(void *x)
{
    frand fast_random(time(NULL));
    for (long i = 0; i < NUM_OPS; ++i)
    {
        int index = fast_random.gen() % shared_len;

        scoped_spin_lock temp_lock(*(spin_mu_array[index]));
        shared_small[index].val++;
        temp_lock.release();
    }

    return NULL;
}



/**********  RUN  **********/

void run_test(test_func func_call)
{
    // loop through and create threads
    for (int i = 0; i < NUM_THREADS; ++i)
    {

        pthread_create(&threads[i], NULL, func_call, NULL);
    }

    // wait for threads
    for (int i = 0; i < NUM_THREADS; ++i)
    {
        pthread_join(threads[i], NULL);
    }
}


int main(int argc, char* argv[])
{
    // initialize the mutexes
    tsx_mu = new tsx_mutex();
    spin_mu = new spin_lock();
    shared_len = 1;

    
    // initialize the test calls
    char const *tests[] = {
        "no_lock_array", 
        "spin_lock_array", 
        "fine_grained_spin_lock_array",
        "speculative_spin_lock_array",
        "no_lock_array_small", 
        "spin_lock_array_small", 
        "fine_grained_spin_lock_array_small",
        "speculative_spin_lock_array_small"
    };

    test_func test_funcs[] = {
        no_lock_array, 
        spin_lock_array, 
        fine_grained_spin_lock_array,
        speculative_spin_lock_array,
        no_lock_array_small, 
        spin_lock_array_small, 
        fine_grained_spin_lock_array_small,
        speculative_spin_lock_array_small
    };

    // determine number of types of tests to run
    size_t tests_size = sizeof(tests) / sizeof(tests[0]);

    // init the cycler
    cycles cycler;
    if(!cycler.init())
    {
        std::cout << "nope. cycles::init failed" << std::endl;
        return 1;
    }

    std::cout << "arr_len";
    for (int i = 0; i < tests_size; ++i)
    {
        std::cout << "," << tests[i];
    }
    std::cout << std::endl;

    // loop for array size
    while (1)
    {
        std::cout << shared_len << std::flush;

        // loop for different tests
        for(size_t test_type = 0; test_type < tests_size; ++test_type)
        {

            double avg = 0;

            // loop for testing with different amounts of threads
            for(int num_rep = 0; num_rep < NUM_REPS; ++num_rep)
            {
                // make sure the start of the array is aligned on 64 bytes 
                // so each element of array is fully contained on its own cache line
                posix_memalign((void **)&shared, 64, shared_len * sizeof(elem));
                if(((uint64_t)shared & (64-1)) != 0)
                {
                    std::cout << "Error: array not aligned" << std::endl;
                }
                memset(shared, 0, shared_len * sizeof(elem));

                shared_small = (elem_small*) calloc(shared_len, sizeof(elem_small));
                threads = (pthread_t*) calloc(NUM_THREADS, sizeof(pthread_t));
                spin_mu_array = (spin_lock **) calloc(shared_len, sizeof(spin_lock *));

                // initialize fine-grained locks
                for (int i = 0; i < shared_len; ++i)
                {
                    spin_mu_array[i] = new spin_lock();
                }

                uint64_t start = __rdtsc();
                run_test(test_funcs[test_type]);
                uint64_t end = __rdtsc();

                double time = cycler.to_seconds(end - start);
                avg += time;

                // clean up
                free(shared);
                free(shared_small);
                free(threads);
                free(spin_mu_array);
            }
            std::cout << "," << avg / NUM_REPS << std::flush;
        }
        std::cout << std::endl;
        shared_len <<= 1;
    }

    // int op_avg = 0;
    // for(int num_rep = 0; num_rep < NUM_REPS; ++num_rep)
    // {
    //     std::cout << "*** Testing XBEGIN/XEND Cost ***" << std::endl;

    //     shared = (elem*) calloc(1, sizeof(elem));

    //     uint64_t start_spec = __rdtsc();
    //     run_test(1, speculative_spin_lock_array);
    //     uint64_t end_spec = __rdtsc();

    //     uint64_t start = __rdtsc();
    //     run_test(1, spin_lock_array);
    //     uint64_t end = __rdtsc();

    //     uint64_t spec_total = end_spec - start_spec;
    //     uint64_t non_spec_total = end - start;

    //     uint64_t total_diff;
    //     if(spec_total > non_spec_total)
    //     {
    //         total_diff = spec_total - non_spec_total;
    //     }
    //     else
    //     {
    //         total_diff = non_spec_total - spec_total;
    //     }

    //     int op_diff = total_diff / NUM_OPS;
    //     op_avg += op_diff;
    //     std::cout << "Total Cycles Spec: " << spec_total << std::endl;
    //     std::cout << "Total Cycles Non Spec: " << non_spec_total << std::endl;
    //     std::cout << "Total Diff in Cycles: " << total_diff << std::endl;
    //     std::cout << "Total Diff per Op: " << op_diff << std::endl;

    //     free(shared);
    // }

    // std::cout << "AVG OP COST: " << op_avg / NUM_REPS << std::endl;
}







