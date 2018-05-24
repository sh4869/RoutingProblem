#ifndef TYPES_H_
#define TYPES_H_

#include <unordered_map>
#include <vector>

using Address = std::unordered_map<int, std::pair<std::string, int>>;
using Result = std::pair<double, std::vector<std::vector<int>>>;
using MapData = std::vector<std::pair<int, std::pair<int, int>>>;

#endif