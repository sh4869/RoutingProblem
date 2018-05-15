#ifndef GREEDY_H_
#define GREEDY_H_
#include <algorithm>
#include <limits>
#include <random>
#include <set>
#include <unordered_map>
#include "dijkstra.h"
#include "util.h"

struct Solver {
    ShortestPathMap map;
    Address addressList;
    Solver(ShortestPathMap _map, Address _address) : map(_map), addressList(_address) {}
    // Greedy法
    void SolveGreedy(int start) {
        int p = start;
        double cost = 0;
        std::vector<int> path = {};
        int next = 0;
        while (1) {
            double min = std::numeric_limits<double>::max();
            path.push_back(p);
            if (path.size() == addressList.size()) break;
            for (auto m : map.At(p)) {
                if (min > m.second.cost &&
                    std::find(path.begin(), path.end(), m.first) == path.end()) {
                    next = m.first;
                    min = m.second.cost;
                }
            }
            cost += min;
            p = next;
        }
        path.push_back(start);
        cost += map.GetPath(p, start).cost;
        for (auto v : path) {
            std::cout << addressList.at(v) << " <- ";
        }
        std::cout << std::endl;
        std::cout << cost * 5 / 30 << "h" << std::endl;
    }
    // Greedy法をランダムでたくさんやる
    void SolveGreedyRandom(int start) {
        int count = 0;
        std::vector<int> resultpath;
        double resultcost = std::numeric_limits<double>::max();
        // for random
        std::random_device seed_gen;
        std::default_random_engine engine(seed_gen());
        while (count < 500) {
            count++;
            double cost = 0;
            int next = 0, p = start;
            std::vector<int> path = {};
            std::set<int> visited = {};
            bool skip = false;
            while (1) {
                double min = std::numeric_limits<double>::max();
                std::vector<int> choices = {};
                path.push_back(p);
                visited.insert(p);
                // すでに追加された経路がAddressのサイズを超えていたら終了
                if (path.size() == addressList.size()) break;
                // Mapの中身を検索
                for (auto m : map.At(p)) {
                    if (visited.find(m.first) != visited.end()) continue;
                    if (min > m.second.cost) {
                        choices.clear();
                        choices.push_back(m.first);
                        min = m.second.cost;
                    } else if (min == m.second.cost) {
                        choices.push_back(m.first);
                    }
                }
                // 複数の候補の中から選択
                if (choices.size() == 1) {
                    next = choices.at(0);
                } else {
                    std::uniform_int_distribution<> dist(0, choices.size() - 1);
                    next = choices.at(dist(engine));
                }
                // costを計算
                cost += min;
                if (cost > resultcost) {
                    skip = true;
                    break;
                }
                p = next;
            }
            if (!skip) {
                path.push_back(start);
                cost += map.GetPath(p, start).cost;
                // もしcostが更新されてたらresultpathも更新
                if (cost < resultcost) {
                    resultpath = path;
                    resultcost = cost;
                }
            }
        }
        for (auto v : resultpath) {
            std::cout << addressList.at(v) << " <- ";
        }
        std::cout << std::endl;
        std::cout << resultcost * 5 / 30 << "h" << std::endl;
    }
};

#endif