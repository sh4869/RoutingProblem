#ifndef DIJKSTRA_H_
#define DIJKSTRA_H_
#include <array>
#include <iostream>
#include <limits>
#include <unordered_map>
#include <vector>
#include "roadgraph.h"
#include "util.h"

struct DijkstraData {
    double cost;
    int parent;
    DijkstraData() = default;
    DijkstraData(double _cost, int _parent) : cost(_cost), parent(_parent) {}
};

struct ShortestPath {
    int start;
    int end;
    double cost;
    std::vector<int> path;
    ShortestPath() = default;
    ShortestPath(int _start, int _end, double _cost, const std::vector<int>& _path)
        : start(_start), end(_end), cost(_cost), path(_path) {}
    void print() {
        std::cout << "start: " << start << " goal: " << end << std::endl;
        for (int x : path) {
            std::cout << x;
        }
        std::cout << std::endl;
    }
};

struct ShortestPathMap {
    std::unordered_map<int, std::unordered_map<int, ShortestPath>> map;
    ShortestPathMap() = default;
    void Add(int start, std::unordered_map<int, ShortestPath> paths) {
        map.insert(std::make_pair(start, paths));
    }
    ShortestPath GetPath(int start, int end) {
        return map.at(start).at(end);
    }
    std::unordered_map<int, ShortestPath> At(int start) {
        return map.at(start);
    }
};

template <int N>
struct Dijkstra {
    std::array<DijkstraData, N * N> data;
    RoadGraph<N> graph;
    Dijkstra() {
        initialize();
    }
    void SetRoadGraph(const RoadGraph<N>& _graph) {
        graph = _graph;
    }
    // ダイクストラを計算する
    std::unordered_map<int, ShortestPath> Solve(int start, Address goals) {
        initialize();
        SolveDijkstra(start);
        return GetPaths(start, goals);
    }

private:
    std::array<bool, N * N> check;

    void initialize() {
        DijkstraData ini =
                DijkstraData(std::numeric_limits<double>::max(), std::numeric_limits<int>::max());
        data.fill(ini);
        check.fill(false);
    }

    bool allNodeChecked() {
        for (bool v : check) {
            if (!v) return false;
        }
        return true;
    }

    std::unordered_map<int, ShortestPath> GetPaths(int start, Address goals) {
        std::unordered_map<int, ShortestPath> paths;
        Address::iterator itr = goals.begin();
        for (; itr != goals.end(); itr++) {
            paths.insert(std::make_pair(itr->first, GetShortestPath(start, itr->first)));
        }
        return paths;
    }

    ShortestPath GetShortestPath(int start, int end) {
        std::vector<int> path;
        int p = end;
        if (data[p].cost == std::numeric_limits<double>::max()) {
            std::cout << "Not Found" << std::endl;
            std::vector<int> v;
            return ShortestPath(start, end, 0, v);
        }
        while (p != -1) {
            path.push_back(p);
            p = data[p].parent;
        }
        return ShortestPath(start, end, data[end].cost, path);
    }

    void SolveDijkstra(int start) {
        data.at(start).cost = 0;
        data.at(start).parent = -1;
        while (!allNodeChecked()) {
            int min = N * N + 1;
            for (int i = 0; i < N * N; i++) {
                if (!check[i]) {
                    if (min == N * N + 1) {
                        min = i;
                    }
                    if (data[min].cost > data[i].cost) {
                        min = i;
                    }
                }
            }
            check.at(min) = true;
            for (int i = 0; i < N * N; i++) {
                if (graph.At(min, i) != 0 && !check.at(i)) {
                    if (data[i].cost > data[min].cost + graph.At(min, i)) {
                        data[i].cost = data[min].cost + graph.At(min, i);
                        data[i].parent = min;
                    }
                }
            }
        }
    }
};

#endif