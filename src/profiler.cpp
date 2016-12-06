#include<iostream>
#include<string>
#include<unordered_map>
#include<unistd.h>
#include<x86intrin.h>
#include "tbb/concurrent_unordered_map.h"
#include "frand.h"
#include "cycles.h"

// map related defs
typedef tbb::concurrent_unordered_map<std::string, std::string> con_map;

con_map* test()
{
    con_map* data = new con_map;
    data->emplace("hello", "my name is graham.");
    return data;
}

void print_map(con_map* data)
{
    for(auto iter = data->begin(); iter != data->end(); ++iter)
    {
        std::cout << iter->first << ": " << iter->second << std::endl;
    }
}

int main(int argc, char* argv[])
{
    con_map* thing = test();
    print_map(thing);
    uint64_t first = __rdtsc();
    std::cout << first << std::endl;
    usleep(1000000);
    uint64_t second = __rdtsc();
    std::cout << second << std::endl;
    std::cout << "diff: " << (second - first) << std::endl;

    frand ran(time(NULL));
    int count = 25;
    while(count > 0)
    {
        std::cout << ran.gen() << std::endl;
        --count;
    }

    cycles cycler;
    if(!cycler.init())
    {
        std::cout << "nope. cycles::init failed" << std::endl;
        return 1;
    }
    std::cout << "cycles per second: " << cycler.get_cycles_per_sec() << std::endl;
    std::cout << "it took: " << cycler.to_seconds(second - first) << std::endl;

}







