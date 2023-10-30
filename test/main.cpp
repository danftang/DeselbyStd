//
// Created by daniel on 29/09/23.
//

#include <iostream>
#include "stlstream.h"

int main() {
    std::cout << std::vector<int>{1,2,3,4} << std::endl;
    std::cout << std::vector<std::pair<int,int>>{{1,2},{2,3}} << std::endl;
    std::cout << std::pair<std::vector<int>,std::vector<int>>{{1,2},{2,3}} << std::endl;
    std::cout << std::map<int,double>{{1,1.1}, {2,2.2}, {3, 3.3}} << std::endl;
    std::cout << std::valarray<int>{1,2,3,4} << std::endl;
    std::cout << std::chrono::milliseconds(234) << std::endl;
    std::cout << std::integer_sequence<int,1,2,3,4>{} << std::endl;
    std::cout << std::pair<int,double>(1234,1.234) << std::endl;
    std::cout << std::tuple<int,double,char>(1234,5.678,'a') << std::endl;
    std::cout << std::tuple<int>(1234) << std::endl;
    std::cout << std::tuple<>{} << std::endl;

    // OK bored now...

    return 0;
}