#include<iostream>
#include<string>
#include<unordered_map>

typedef std::unordered_map<std::string, std::string> map;


map* test()
{
    map* data = new map;
    data->emplace("hello", "my name is graham.");
    return data;
}

void print_map(map* data)
{
    for(auto iter = data->begin(); iter != data->end(); ++iter)
    {
        std::cout << iter->first << ": " << iter->second << std::endl;
    }
}

int main(int argc, char* argv[])
{
    map* thing = test();
    print_map(thing);
}
