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
#include "types.h"

struct Solver {
    ShortestPathMap map;
    Address addressList;
    int start, goal;
    double upperlimit;
    Solver() = default;
    Solver(ShortestPathMap _map, Address _address, int _start, double _upperlimit)
        : map(_map), addressList(_address), start(_start), goal(_start), upperlimit(_upperlimit) {}
    // 焼きなまし法
    void setGoal(int _goal) {
        goal = _goal;
    }
    Result SA(bool displaygraph) {
        auto x = SolveGreedy(1);
        double cost = x.first;
        std::vector<std::vector<int>> path = x.second;
        double T = 5000000, cool = 0.9999, hot = 1.000001;
        std::random_device seed_gen;
        std::default_random_engine engine(seed_gen());

        // 記録用の変数
        int count = 0;
        std::vector<double> thistory;
        std::vector<double> costhistory;

        // goalが-1なら最後の要素も入れ替えてしまって良い
        int minus = (goal == -1) ? -1 : -2;
        std::uniform_int_distribution<> choice(1, x.second.at(0).size() + minus);
        std::uniform_real_distribution<> dist1(0, 1.0);
        while (T > 0.01) {
            int a = choice(engine), b = choice(engine);
            std::vector<std::vector<int>> newpaths = path;
            newpaths.at(0).at(a) = path.at(0).at(b);
            newpaths.at(0).at(b) = path.at(0).at(a);
            if (weightIsOK(newpaths.at(0))) {
                // 追加されるパスと削除されるパスのコストを足し引きする
                double newcost = caclucateCost(newpaths);
                if (newcost < cost || dist1(engine) < std::exp(-std::abs(newcost - cost) / T)) {
                    cost = newcost;
                    path = newpaths;
                    T = T * cool;
                } else {
                }
                    T = T * hot;
                count++;
                if (count % 500 == 0) {
                    thistory.push_back(T);
                    costhistory.push_back(cost);
                }
            }
        }
        if (displaygraph) {
            for (int i = 1; i < int(count / 500); i++) {
                std::cout << i * 500 << "," << thistory.at(i) << "," << costhistory.at(i)
                          << std::endl;
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
                    cost.at(i) +=
                            map.GetPath(p.at(i), start).cost + map.GetPath(start, next.at(i)).cost;
                    weight.at(i) = 0;
                } else {
                    cost.at(i) += min;
                }
                p.at(i) = next.at(i);
                visited.insert(p.at(i));
            }
        };
        if (goal != -1) {
            for (int i = 0; i < number; i++) {
                cost.at(i) += map.GetPath(p.at(i), start).cost;
                path.at(i).push_back(start);
            }
        }
        auto tmp = std::accumulate(cost.begin(), cost.end(), 0.0);
        return std::make_pair(tmp, path);
    }

    // Greedy法複数台ランダム施行
    Result SolveGreedyRandom(int number, int times) {
        std::vector<std::vector<int>> resultpath(number);
        double resultcost = std::numeric_limits<double>::max();
        int count = 0;
        // for random
        std::random_device seed_gen;
        std::default_random_engine engine(seed_gen());
        while (count < times) {
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
            if (goal != -1) {
                for (int i = 0; i < number; i++) {
                    cost.at(i) += map.GetPath(p.at(i), start).cost;
                    path.at(i).push_back(start);
                }
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

    bool weightIsOK(std::vector<int> path) {
        std::vector<std::vector<int>> routes(0);
        int count = -1;
        for (auto p : path) {
            if (p == start) {
                count++;
                routes.push_back(std::vector<int>());
            }
            routes.at(count).push_back(p);
        }
        for (auto p : routes) {
            if (calculateWeight(p) > upperlimit) {
                return false;
            }
        }
        return true;
    }

    double calculateWeight(std::vector<int> path) {
        double weight = 0;
        for (auto x : path) {
            weight += addressList.at(x).second;
        }
        return weight;
    }
};

#endif