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
    Solver(ShortestPathMap _map, Address _address, int _start)
        : map(_map), addressList(_address), start(_start) {}
    // Greedy法
    Result SolveGreedy() {
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
    Result SolveGreedyRandom() {
        int count = 0;
        std::vector<int> resultpath;
        double resultcost = std::numeric_limits<double>::max();
        // for random
        std::random_device seed_gen;
        std::default_random_engine engine(seed_gen());
        while (count < 10000) {
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

    // 焼きなまし法
    Result SA() {
        auto x = SolveGreedy();
        double cost = x.first;
        std::vector<int> path = x.second;
        double T = 100000, cool = 0.99999, hot = 1.0000001;
        std::random_device seed_gen;
        std::default_random_engine engine(seed_gen());
        std::uniform_int_distribution<> dist(1, path.size() - 2);
        std::uniform_real_distribution<> dist1(0, 1.0);
        while (T > 0.001) {
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
                T = T * cool;
            } else {
                T = T * hot;
            }
        }
        x.first = cost;
        x.second = path;
        return x;
    }

    // 複数台のGreedy法
    void SolveGreedyMulti(int number) {
        std::vector<int> p(number, start);
        std::vector<double> cost(number, 0);
        std::vector<std::vector<int>> path(number);
        std::set<int> visited = {};
        std::vector<int> next(number, start);
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
                cost.at(i) += min;
                p.at(i) = next.at(i);
                visited.insert(p.at(i));
            }
        };
        for (int i = 0; i < number; i++) {
            cost.at(i) += map.GetPath(p.at(i), start).cost;
            path.at(i).push_back(start);
        }

        for (int i = 0; i < number; i++) {
            std::cout << i + 1 << "台目: ";
            std::cout << addressList.at(path.at(i).at(0));
            for (std::size_t j = 1; j < path.at(i).size(); j++) {
                std::cout << " <- " << addressList.at(path.at(i).at(j));
            }
            std::cout << std::endl;
        }
        auto tmp = std::max_element(cost.begin(), cost.end());
        std::cout << "cost: " << *tmp  << "h" << std::endl;
    }

    // Greedy法複数台ランダム施行
    void SolveGreedyMultiRandom(int number) {
        std::vector<std::vector<int>> resultpath(number);
        double resultcost = std::numeric_limits<double>::max();
        int count = 0;
        // for random
        std::random_device seed_gen;
        std::default_random_engine engine(seed_gen());
        while (count < 10) {
            count++;

            std::vector<int> p(number, start);
            std::vector<double> cost(number, 0);
            std::vector<std::vector<int>> path(number);
            std::set<int> visited = {};
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
                    cost.at(i) += min;
                    visited.insert(p.at(i));
                }
            };
            // スタート地点への移動を追加
            for (int i = 0; i < number; i++) {
                cost.at(i) += map.GetPath(p.at(i), start).cost;
                path.at(i).push_back(start);
            }
            auto tmp = std::max_element(cost.begin(), cost.end());
            if (resultcost > *tmp) {
                resultpath = path;
                resultcost = *tmp;
            }
        }

        for (int i = 0; i < number; i++) {
            std::cout << i + 1 << "台目: ";
            std::cout << addressList.at(resultpath.at(i).at(0));
            for (std::size_t j = 1; j < resultpath.at(i).size(); j++) {
                std::cout << " <- " << addressList.at(resultpath.at(i).at(j));
            }
            std::cout << std::endl;
        }
        std::cout << "cost: " << resultcost  << "h" << std::endl;
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