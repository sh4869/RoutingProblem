#ifndef UTIL_H_
#define UTIL_H_

#include <fstream>
#include <iostream>
#include <unordered_map>
#include <sstream>
#include <string>
#include <vector>

using Address = std::unordered_map<int, std::string>;

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


#endif