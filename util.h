#ifndef UTIL_H_
#define UTIL_H_

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <cmath>
#include "dijkstra.h"
#include "types.h"

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
        v.insert(std::make_pair(std::stoi(vec.at(0)),
                                std::make_pair(vec.at(1), std::stoi(vec.at(2)))));
    }
    return v;
}

MapData readMap(std::string filename) {
    MapData map;
    std::ifstream ifs(filename);
    std::string s;
    while (std::getline(ifs, s)) {
        auto vec = split(s, ',');
        auto cost = std::stoi(vec.at(2));
        map.push_back(
                std::make_pair(cost, std::make_pair(std::stoi(vec.at(0)), std::stoi(vec.at(1)))));
        map.push_back(
                std::make_pair(cost, std::make_pair(std::stoi(vec.at(1)), std::stoi(vec.at(0)))));
    }
    return map;
}

void PrintResult(Result result, Address address) {
    for (std::size_t i = 0; i < result.second.size(); i++) {
        std::cout << i + 1 << "台目: " << address.at(result.second.at(i).at(0)).first;
        for (std::size_t j = 1; j < result.second.at(i).size(); j++) {
            std::cout << " <- " << address.at(result.second.at(i).at(j)).first;
        }
        std::cout << std::endl;
    }
    std::cout << "cost: " << result.first << "h" << std::endl;
}

void checkCost(Address address, ShortestPathMap pathmap) {
    Address::iterator itr = address.begin();
    Address::iterator itr2 = address.begin();
    for (; itr != address.end(); itr++) {
        for (; itr2 != address.end(); itr2++) {
            if (itr->first != itr2->first) {
                int start = itr->first;
                int end = itr2->first;
                int startx = start / 9;
                int starty = start % 9;
                int endx = end / 9;
                int endy = end % 9;
                if (pathmap.GetPath(itr->first, itr2->first).cost ==
                    std::abs(startx - endx) + std::abs(starty - endy)) {
                    // std::cout << start << " " << end << " 正解" << std::endl;
                } else {
                    std::cout << start << " " << end << " Error" << std::endl;
                }
            }
        }
    }
}

#endif