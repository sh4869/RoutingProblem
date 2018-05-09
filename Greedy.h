#ifndef GREEDY_H_
#define GREEDY_H_
#include <algorithm>
#include <limits>
#include "address.h"
#include "dijkstra.h"

struct Greedy {
    ShortestPathMap map;
    std::vector<Address> addressList;
    Greedy(ShortestPathMap _map, std::vector<Address> _address)
        : map(_map), addressList(_address) {}
    void Slove(int start) {
        int p = start;
        std::vector<std::pair<double, std::vector<int>>> result = {};
        while (1) {
            double cost = 0;
            std::vector<int> path = {};
            int next = 0;
            int count = 0;
            while (1) {
                double min = std::numeric_limits<double>::max();
                path.push_back(p);
                std::cout << p << " <- ";
                if (path.size() == addressList.size()) break;
                for (auto m : map.At(p)) {
                    if (min > m.second.cost &&
                        std::find(path.begin(), path.end(), m.first) == path.end()) {
                        next = m.first;
                        min = m.second.cost;
                    } else if (min == m.second.cost) {
                        if (!alreadyChecked(result, path, count, m.first)) {
                            next = m.first;
                            min = m.second.cost;
                        }
                    }
                }
                cost += min;
                p = next;
            }
            path.push_back(start);
            cost += map.GetPath(p, start).cost;
            result.push_back(std::make_pair(cost, path));
        }
        /*
        std::cout << start << std::endl;
        std::cout << cost * 5 / 30 << "h" << std::endl;
        */
    }

private:
    bool alreadyChecked(std::vector<std::pair<double, std::vector<int>>> result,
                        std::vector<int> path,
                        int count,
                        int next) {
        for (auto e : result) {
            for (int i = 0; i < count; i++) {
                if (i == count - 1 && e.second.at(count) == next) {
                    return true;
                }
                if (e.second.at(i) != path.at(i)) break;
            }
        }
        return false;
    }
};

#endif