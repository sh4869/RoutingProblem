#ifndef MAP_H_
#define MAP_H_

#include <array>
#include <iostream>

struct RoadGraph {
    std::vector<std::vector<int>> w;
    int length;
    RoadGraph() = default;
    RoadGraph(const int length,const int start){
        w.resize(length * length);
        for(std::size_t i = 0;i < w.size();i++){
            w.at(i).resize(length * length) ;
        }
        int result;
        for (int i = 0; i < length * length; i++) {
            for (int j = 0; j < length * length; j++) {
                if (i > length && j == i - length)  // 下
                    result = start;
                if (i < length * (length - 1) && j == i + length)  // 上
                    result = start;
                if (i % length != 0 && j == i + 1)  // 右
                    result = start;
                if (i % length != 1 && j == i - 1)  // 左
                    result = start;
                w.at(i).at(j) = result;
                result = 0;
            }
        }
    }
    RoadGraph& operator=(const RoadGraph&) = default;
    int At(int x, int y) {
        return w.at(x).at(y);
    }
};

#endif