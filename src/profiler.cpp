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
#include <stdlib.h>
#include <stdio.h>

#define NUM_REPS 5
#define NUM_THREADS 10
#define NUM_ACCESSES 1000000


/* htm globals */
tsx_mutex *htm_mu;

/* spin lock globals */
spin_lock *spin_mu;

/* pthread globals */
pthread_t *threads;
pthread_mutex_t p_mu = PTHREAD_MUTEX_INITIALIZER;

/* global shared access data structures */
elem *shared;
elem_small *shared_small;

map *shared_map;
small_map *shared_small_map;






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
void* speculative_spin_lock_array(void *indexp)
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

/*
 * spin_lock_array
 * tests array accesses with a 
 * spin lock.
 *
 * indexp: index of array to access
 *
 * return void
 */
void* spin_lock_array(void *indexp)
{
    int index = *(int*)indexp;
    free(indexp);
    
    for (long i = 0; i < NUM_ACCESSES; ++i)
    {
        scoped_spin_lock temp_lock(*spin_mu);
        shared[index].val++;
        temp_lock.release();
    }

    return NULL;
}

/*
 * pthread_lock_array
 * tests array accesses with a 
 * pthread lock.
 *
 * indexp: index of array to access
 *
 * return void
 */
void* pthread_lock_array(void *indexp)
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

/*
 * no_lock_array
 * tests array accesses with a 
 * pthread lock.
 *
 * indexp: index of array to access
 *
 * return void
 */
void* no_lock_array(void *indexp)
{
    int index = *(int*)indexp;
    free(indexp);
    
    for (long i = 0; i < NUM_ACCESSES; ++i)
    {
        shared[index].val++;
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
void* speculative_spin_lock_array_small(void *indexp)
{
    int index = *(int*)indexp;
    free(indexp);
    
    for (long i = 0; i < NUM_ACCESSES; ++i)
    {
        tsx_scoped_mutex temp_lock(*htm_mu);
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
void* spin_lock_array_small(void *indexp)
{
    int index = *(int*)indexp;
    free(indexp);
    
    for (long i = 0; i < NUM_ACCESSES; ++i)
    {
        scoped_spin_lock temp_lock(*spin_mu);
        shared_small[index].val++;
        temp_lock.release();
    }

    return NULL;
}

/*
 * pthread_lock_array_small
 * tests array accesses with a 
 * pthread lock.
 *
 * indexp: index of array to access
 *
 * return void
 */
void* pthread_lock_array_small(void *indexp)
{
    int index = *(int*)indexp;
    free(indexp);
    
    for (long i = 0; i < NUM_ACCESSES; ++i)
    {
        pthread_mutex_lock(&p_mu);
        shared_small[index].val++;
        pthread_mutex_unlock(&p_mu);
    }

    return NULL;
}

/*
 * no_lock_array_small
 * tests array accesses with a 
 * pthread lock.
 *
 * indexp: index of array to access
 *
 * return void
 */
void* no_lock_array_small(void *indexp)
{
    int index = *(int*)indexp;
    free(indexp);
    
    for (long i = 0; i < NUM_ACCESSES; ++i)
    {
        shared_small[index].val++;
    }

    return NULL;
}



/********** MAP ACCESSES **********/
/*
 * speculative_spin_lock_map
 * tests array accesses with a 
 * speculative spin lock.
 *
 * indexp: index of array to access
 *
 * return void
 */
void* speculative_spin_lock_map(void *indexp)
{
    int index = *(int*)indexp;
    free(indexp);
    
    for (long i = 0; i < NUM_ACCESSES; ++i)
    {
        tsx_scoped_mutex temp_lock(*htm_mu);
        (*shared_map)[index]->val++;
        temp_lock.release();
    }

    return NULL;
}

/*
 * spin_lock_map
 * tests array accesses with a 
 * spin lock.
 *
 * indexp: index of array to access
 *
 * return void
 */
void* spin_lock_map(void *indexp)
{
    int index = *(int*)indexp;
    free(indexp);
    
    for (long i = 0; i < NUM_ACCESSES; ++i)
    {
        scoped_spin_lock temp_lock(*spin_mu);
        (*shared_map)[index]->val++;
        temp_lock.release();
    }

    return NULL;
}

/*
 * pthread_lock_map
 * tests array accesses with a 
 * pthread lock.
 *
 * indexp: index of array to access
 *
 * return void
 */
void* pthread_lock_map(void *indexp)
{
    int index = *(int*)indexp;
    free(indexp);
    
    for (long i = 0; i < NUM_ACCESSES; ++i)
    {
        pthread_mutex_lock(&p_mu);
        (*shared_map)[index]->val++;
        pthread_mutex_unlock(&p_mu);
    }

    return NULL;
}

/*
 * no_lock_map
 * tests array accesses with a 
 * pthread lock.
 *
 * indexp: index of array to access
 *
 * return void
 */
void* no_lock_map(void *indexp)
{
    int index = *(int*)indexp;
    free(indexp);
    
    for (long i = 0; i < NUM_ACCESSES; ++i)
    {
        (*shared_map)[index]->val++;
    }

    return NULL;
}












/********** SMALL MAP ACCESSES **********/
/*
 * speculative_spin_lock_map_small
 * tests array accesses with a 
 * speculative spin lock.
 *
 * indexp: index of array to access
 *
 * return void
 */
void* speculative_spin_lock_map_small(void *indexp)
{
    int index = *(int*)indexp;
    free(indexp);
    
    for (long i = 0; i < NUM_ACCESSES; ++i)
    {
        tsx_scoped_mutex temp_lock(*htm_mu);
        (*shared_small_map)[index]->val++;
        temp_lock.release();
    }

    return NULL;
}

/*
 * spin_lock_map
 * tests array accesses with a 
 * spin lock.
 *
 * indexp: index of array to access
 *
 * return void
 */
void* spin_lock_map_small(void *indexp)
{
    int index = *(int*)indexp;
    free(indexp);
    
    for (long i = 0; i < NUM_ACCESSES; ++i)
    {
        scoped_spin_lock temp_lock(*spin_mu);
        (*shared_small_map)[index]->val++;
        temp_lock.release();
    }

    return NULL;
}

/*
 * pthread_lock_map_small
 * tests array accesses with a 
 * pthread lock.
 *
 * indexp: index of array to access
 *
 * return void
 */
void* pthread_lock_map_small(void *indexp)
{
    int index = *(int*)indexp;
    free(indexp);
    
    for (long i = 0; i < NUM_ACCESSES; ++i)
    {
        pthread_mutex_lock(&p_mu);
        (*shared_small_map)[index]->val++;
        pthread_mutex_unlock(&p_mu);
    }

    return NULL;
}

/*
 * no_lock_map_small
 * tests array accesses with a 
 * pthread lock.
 *
 * indexp: index of array to access
 *
 * return void
 */
void* no_lock_map_small(void *indexp)
{
    int index = *(int*)indexp;
    free(indexp);
    
    for (long i = 0; i < NUM_ACCESSES; ++i)
    {
        (*shared_small_map)[index]->val++;
    }

    return NULL;
}


/**********  RUN  **********/

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
    }
}


int main(int argc, char* argv[])
{
    // initialize the mutexes
    htm_mu = new tsx_mutex();
    spin_mu = new spin_lock();
    
    // initialize the test calls
    char const *tests[] = {
        "no_lock_array", 
        "pthread_lock_array",
        "spin_lock_array", 
        "speculative_spin_lock_array",
        "no_lock_array_small", 
        "pthread_lock_array_small",
        "spin_lock_array_small", 
        "speculative_spin_lock_array_small",
        "no_lock_map",
        "pthread_lock_map",
        "spin_lock_map", 
        "speculative_spin_lock_map_small",
        "no_lock_map_small",
        "pthread_lock_map_small",
        "spin_lock_map_small", 
        "speculative_spin_lock_map_small"
    };

    test_func test_funcs[] = {
        no_lock_array, 
        pthread_lock_array,
        spin_lock_array, 
        speculative_spin_lock_array,
        no_lock_array_small, 
        pthread_lock_array_small,
        spin_lock_array_small, 
        speculative_spin_lock_array_small,
        no_lock_map,
        pthread_lock_map,
        spin_lock_map, 
        speculative_spin_lock_map,
        no_lock_map_small,
        pthread_lock_map_small,
        spin_lock_map_small, 
        speculative_spin_lock_map_small
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

    // loop for different tests
    for(size_t test_type = 0; test_type < tests_size; ++test_type)
    {
        std::cout << "*** Testing " << tests[test_type] << " ***" << std::endl;

        // loop for testing with different amounts of threads
        for (int num_of_thds = 0; num_of_thds < NUM_THREADS; ++num_of_thds)
        {
            double avg = 0;

            // loop for repition
            for(int num_rep = 0; num_rep < NUM_REPS; ++num_rep)
            {
                posix_memalign((void **)&shared, 64, num_of_thds * sizeof(elem));
                if(((uint64_t)shared & (64-1)) != 0)
                {
                    std::cout << "Error: array not aligned" << std::endl;
                }
                memset(shared, 0, num_of_thds * sizeof(elem));

                shared_small = (elem_small*) calloc(num_of_thds, sizeof(elem_small));
                threads = (pthread_t*) calloc(num_of_thds, sizeof(pthread_t));
                shared_map = new map();
                shared_small_map = new small_map();
                for(int i = 0; i < num_of_thds; ++i)
                {
                    shared_map->emplace(i, new elem());
                    shared_small_map->emplace(i, new elem_small());
                }

                uint64_t start = __rdtsc();
                run_test(num_of_thds, test_funcs[test_type]);
                uint64_t end = __rdtsc();

                double time = cycler.to_seconds(end - start);
                avg += time;

                //std::cout << num_of_thds << "\t" << time << std::endl;

                // clean up
                free(shared);
                free(shared_small);
                free(threads);
                free(shared_map);
                free(shared_small_map);
            }
            std::cout << "Avg: " << num_of_thds << "\t" << avg / NUM_REPS << std::endl;
        }
    }

    int op_avg = 0;
    for(int num_rep = 0; num_rep < NUM_REPS; ++num_rep)
    {
        std::cout << "*** Testing XBEGIN/XEND Cost ***" << std::endl;

        shared = (elem*) calloc(1, sizeof(elem));

        uint64_t start_spec = __rdtsc();
        run_test(1, speculative_spin_lock_array);
        uint64_t end_spec = __rdtsc();

        uint64_t start = __rdtsc();
        run_test(1, spin_lock_array);
        uint64_t end = __rdtsc();

        uint64_t spec_total = end_spec - start_spec;
        uint64_t non_spec_total = end - start;

        uint64_t total_diff;
        if(spec_total > non_spec_total)
        {
            total_diff = spec_total - non_spec_total;
        }
        else
        {
            total_diff = non_spec_total - spec_total;
        }

        int op_diff = total_diff / NUM_ACCESSES;
        op_avg += op_diff;
        std::cout << "Total Cycles Spec: " << spec_total << std::endl;
        std::cout << "Total Cycles Non Spec: " << non_spec_total << std::endl;
        std::cout << "Total Diff in Cycles: " << total_diff << std::endl;
        std::cout << "Total Diff per Op: " << op_diff << std::endl;

        free(shared);
    }

    std::cout << "AVG OP COST: " << op_avg / NUM_REPS << std::endl;
}







