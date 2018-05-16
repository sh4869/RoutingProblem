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
    int start;
    double upperlimit;
    Solver(ShortestPathMap _map, Address _address, int _start, double _upperlimit)
        : map(_map), addressList(_address), start(_start), upperlimit(_upperlimit) {}
    // 焼きなまし法

    Result SA() {
        auto x = SolveGreedy(2);
        double cost = x.first;
        std::vector<std::vector<int>> path = x.second;
        double T = 10000000, cool = 0.999999, hot = 1.000000001;
        std::random_device seed_gen;
        std::default_random_engine engine(seed_gen());
        std::uniform_int_distribution<> choice(0, x.second.size()-1);
        std::uniform_real_distribution<> dist1(0, 1.0);
        while (T > 0.001) {
            int a = choice(engine), b = choice(engine);
            int a_index = std::uniform_int_distribution<>(1, x.second.at(a).size() - 2)(engine);
            int b_index = std::uniform_int_distribution<>(1, x.second.at(b).size() - 2)(engine);
            std::vector<std::vector<int>> newpaths = x.second;
            newpaths.at(a).at(a_index) = x.second.at(b).at(b_index);
            newpaths.at(b).at(b_index) = x.second.at(a).at(a_index);
            // 追加されるパスと削除されるパスのコストを足し引きする
            double newcost = caclucateCost(newpaths);
            if (newcost < cost || dist1(engine) < std::exp(-std::abs(newcost - cost) / T)) {
                cost = newcost;
                path = newpaths;
                T = T * cool;
            } else {
                T = T * hot;
            }
        }
        x.first = cost;
        x.second = path;
        return x;
    }

    Result SolveGreedy(int number) {
        std::vector<int> p(number, start);
        std::vector<double> cost(number, 0);
        std::vector<std::vector<int>> path(number);
        std::set<int> visited = {};
        std::vector<int> next(number, start);
        std::vector<double> weight(number, 0);
        bool finish = false;

        visited.insert(start);
        while (!finish) {
            // 台数分だけ繰り返す
            for (int i = 0; i < number; i++) {
                double min = std::numeric_limits<double>::max();
                path.at(i).push_back(p.at(i));
                // 二重ループなんでtrueをfalseに
                if (visited.size() == addressList.size()) {
                    finish = true;
                    break;
                }
                for (auto m : map.At(p.at(i))) {
                    if (min > m.second.cost && visited.find(m.first) == visited.end()) {
                        next.at(i) = m.first;
                        min = m.second.cost;
                    }
                }
                weight.at(i) += addressList.at(next.at(i)).second;
                if (weight.at(i) > upperlimit) {
                    // スタートに戻る必要があるのでその分を追加
                    path.at(i).push_back(start);
                    cost.at(i) += map.GetPath(p.at(i), start).cost;
                    cost.at(i) += map.GetPath(start, next.at(i)).cost;
                    weight.at(i) = 0;
                } else {
                    cost.at(i) += min;
                }
                p.at(i) = next.at(i);
                visited.insert(p.at(i));
            }
        };
        for (int i = 0; i < number; i++) {
            cost.at(i) += map.GetPath(p.at(i), start).cost;
            path.at(i).push_back(start);
        }
        auto tmp = std::accumulate(cost.begin(), cost.end(), 0.0);
        return std::make_pair(tmp, path);
    }

    // Greedy法複数台ランダム施行
    Result SolveGreedyRandom(int number) {
        std::vector<std::vector<int>> resultpath(number);
        double resultcost = std::numeric_limits<double>::max();
        int count = 0;
        // for random
        std::random_device seed_gen;
        std::default_random_engine engine(seed_gen());
        while (count < 1000) {
            count++;

            std::vector<int> p(number, start);
            std::vector<double> cost(number, 0);
            std::vector<std::vector<int>> path(number);
            std::set<int> visited = {};
            std::vector<double> weight(number, 0);
            bool finish = false;
            visited.insert(start);
            while (!finish) {
                for (int i = 0; i < number; i++) {
                    path.at(i).push_back(p.at(i));
                    if (visited.size() == addressList.size()) {
                        finish = true;
                        break;
                    }
                    // 一番低いコストのものを探し続ける
                    double min = std::numeric_limits<double>::max();
                    std::vector<int> choices = {};
                    for (auto m : map.At(p.at(i))) {
                        if (visited.find(m.first) != visited.end()) continue;
                        if (min > m.second.cost) {
                            choices.clear();
                            choices.push_back(m.first);
                            min = m.second.cost;
                        } else if (min == m.second.cost) {
                            choices.push_back(m.first);
                        }
                    }
                    // その中から一つランダムに選択
                    if (choices.size() == 1) {
                        p.at(i) = choices.at(0);
                    } else {
                        std::uniform_int_distribution<> dist(0, choices.size() - 1);
                        p.at(i) = choices.at(dist(engine));
                    }

                    weight.at(i) += addressList.at(p.at(i)).second;
                    if (weight.at(i) > upperlimit) {
                        // スタートに戻る必要があるのでその分を追加
                        auto last = path.at(i).back();
                        path.at(i).push_back(start);
                        cost.at(i) += map.GetPath(last, start).cost;
                        cost.at(i) += map.GetPath(start, p.at(i)).cost;
                        weight.at(i) = 0;
                    } else {
                        cost.at(i) += min;
                    }
                    visited.insert(p.at(i));
                }
            };
            // スタート地点への移動を追加
            for (int i = 0; i < number; i++) {
                cost.at(i) += map.GetPath(p.at(i), start).cost;
                path.at(i).push_back(start);
            }
            auto tmp = std::accumulate(cost.begin(), cost.end(), 0.0);
            if (resultcost > tmp) {
                resultpath = path;
                resultcost = tmp;
            }
        }
        return std::make_pair(resultcost, resultpath);
    }

private:
    double caclucateCost(std::vector<std::vector<int>> paths) {
        double cost = 0;
        for (std::size_t i = 0; i < paths.size(); i++) {
            for (std::size_t j = 0; j < paths.at(i).size() - 1; j++) {
                cost += map.GetPath(paths.at(i).at(j), paths.at(i).at(j + 1)).cost;
            }
        }
        return cost;
    }
};

#endif