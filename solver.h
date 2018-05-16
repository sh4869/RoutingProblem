#ifndef GREEDY_H_
#define GREEDY_H_
#include <algorithm>
#include <cmath>
#include <limits>
#include <random>
#include <set>
#include <unordered_map>
#include <vector>
#include "dijkstra.h"
#include "util.h"

struct Solver {
    ShortestPathMap map;
    Address addressList;
    Solver(ShortestPathMap _map, Address _address) : map(_map), addressList(_address) {}
    // Greedy法
    Result SolveGreedy(int start) {
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
        cost += map.GetPath(p, start).cost;
        path.push_back(start);
        return std::make_pair(cost, path);
    }
    // Greedy法をランダムでたくさんやる
    Result SolveGreedyRandom(int start) {
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
        return std::make_pair(resultcost, resultpath);
    }

    Result SA(int start) {
        auto x = SolveGreedy(start);
        double cost = x.first;
        std::vector<int> path = x.second;
        double T = 10000, cool = 0.9999;
        std::random_device seed_gen;
        std::default_random_engine engine(seed_gen());
        std::uniform_int_distribution<> dist(1, path.size() - 2);
        std::uniform_real_distribution<> dist1(0, 1.0);
        while (T > 0.001) {
            for (int i = 0; i < 10; i++) {
                int x = dist(engine), y = dist(engine);
                // 追加されるパスと削除されるパスのコストを足し引きする
                auto newpath = path;
                newpath.at(x) = path.at(y);
                newpath.at(y) = path.at(x);
                double newcost = caclucateCost(newpath);
                if (newcost < cost || dist1(engine) < std::exp(-std::abs(newcost - cost) / T)) {
                    cost = newcost;
                    // swap
                    int tmp = path.at(x);
                    path[x] = path[y];
                    path[y] = tmp;
                }
            }
            T = T * cool;
        }
        x.first = cost;
        x.second = path;
        return x;
    }

private:
    double caclucateCost(std::vector<int> path) {
        double cost = 0;
        for (std::size_t i = 0; i < path.size() - 1; i++) {
            cost += map.GetPath(path.at(i), path.at(i + 1)).cost;
        }
        return cost;
    }
};

#endif