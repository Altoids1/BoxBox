#include "InheritedContainer.h"
#include <iostream>
#include <vector>
#include <string>

int main()
{
    InheritedContainer<std::vector<int>> one({1,2,3,4});
    InheritedContainer<std::vector<int>> two({5,6,7,8}, one);
   // two.set_parent(one);
    int i = 0;
    for(auto it = two.begin(); it != two.end() && i < 64; it++, i++)
    {
        std::cout << std::to_string(*it) << std::endl;
    }
    return 0;
}