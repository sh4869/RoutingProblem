#ifndef UTIL_H_
#define UTIL_H_

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using Address = std::unordered_map<int, std::string>;
using Result = std::pair<double, std::vector<int>>;


std::vector<std::string> split(const std::string &str, char sep) {
    std::vector<std::string> v;
    std::stringstream ss(str);
    std::string buffer;
    while (std::getline(ss, buffer, sep)) {
        v.push_back(buffer);
    }
    return v;
}

Address readAddress(std::string filename) {
    Address v;
    std::ifstream ifs(filename);
    std::string s;
    while (std::getline(ifs, s)) {
        auto vec = split(s, ',');
        v.insert(std::make_pair(std::stoi(vec.at(0)), vec.at(1)));
    }
    return v;
}

void PrintResult(Result result, Address address) {
    std::cout << address.at(result.second.at(0));
    for (std::size_t i = 1; i < result.second.size(); i++) {
        std::cout << " <- " << address.at(result.second.at(i));
    }
    std::cout << std::endl;
    std::cout << "cost: " << result.first * 5 / 30 << "h" << std::endl;
}


#endif