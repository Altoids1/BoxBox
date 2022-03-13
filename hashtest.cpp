#include "HashTable.h"
#include <unordered_map>
#include <chrono>
#include <iostream>

int main()
{
    HashTable<int,int> table;
    std::unordered_map<int,int> map;

    clock_t table_init_time = clock();
    table.ensure_capacity(1'000'000);
    for(int i = 0; i < 1'000'000; ++i)
    {
        int y = i % 1000;
        table[i] = y;
        
    }
    table_init_time = clock() - table_init_time;

    clock_t map_init_time = clock();
    for(int i = 0; i < 1'000'000; ++i)
    {
        int y = i % 1000;
        map[i] = y;
        
    }
    map_init_time = clock() - map_init_time;
    
    uint64_t shitty = 0;
    clock_t map_it_time = clock();
    for(auto it = map.begin(); it != map.end(); it++)
    {
        shitty += (*it).first % 100 + (*it).second;
    }
    map_it_time = clock() - map_it_time;

    uint64_t bitty = 0;
    clock_t table_it_time = clock();
    for(auto it = table.begin(); it != table.end(); it++)
    {
        bitty += it.key() % 100 + it.value();
    }
    table_it_time = clock() - table_it_time;

    if(bitty != shitty)
    {
        std::cout << "The results aren't equal somehow!\n";
    }
    std::cout << "Clocks for iterating map: " << std::to_string(map_it_time) << "\n";
    std::cout << "Clocks for iterating table: " << std::to_string(table_it_time) << "\n\n";
    std::cout << "Clocks for initializing map: " << std::to_string(map_init_time) << "\n";
    std::cout << "Clocks for initializing table: " << std::to_string(table_init_time) << "\n";
    std::cout << "Hash collisions: " << std::to_string(table.hash_collisions) <<"\n";

    CLOCKS_PER_SEC;
    std::hash<int> hasher;
    return 0;
}