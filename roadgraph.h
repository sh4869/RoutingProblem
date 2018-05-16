#ifndef MAP_H_
#define MAP_H_

#include <array>
#include <iostream>
#include <vector>
#include "util.h"

struct RoadGraph {
    std::vector<std::vector<double>> w;
    int length;
    int cost;
    RoadGraph() = default;
    RoadGraph(const int length, const int _cost, const int defaultspeed) : cost(_cost) {
        w.resize(length * length);
        for (std::size_t i = 0; i < w.size(); i++) {
            w.at(i).resize(length * length);
        }
        double result = 0;
        for (int i = 0; i < length * length; i++) {
            for (int j = 0; j < length * length; j++) {
                if (i > length && j == i - length)  // 下
                    result = (double)cost / (double)defaultspeed;
                if (i < length * (length - 1) && j == i + length)  // 上
                    result = (double)cost / (double)defaultspeed;
                if (i % length != 0 && j == i + 1)  // 右
                    result = (double)cost / (double)defaultspeed;
                if (i % length != 1 && j == i - 1)  // 左
                    result = (double)cost / (double)defaultspeed;
                w.at(i).at(j) = result;
                result = 0;
            }
        }
    }
    void SetMapData(MapData data) {
        for (auto d : data) {
            w.at(d.second.first).at(d.second.second) = (double)cost / d.first;
        }
    }
    RoadGraph& operator=(const RoadGraph&) = default;
    double At(int x, int y) {
        return w.at(x).at(y);
    }
};

#endif