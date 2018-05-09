#ifndef MAP_H_
#define MAP_H_

#include <array>
#include <iostream>

template <int N>
struct RoadGraph {
    std::array<std::array<int, N * N>, N * N> w;
    RoadGraph() = default;
    RoadGraph(const int start) {
        int result;
        for (int i = 0; i < N * N; i++) {
            for (int j = 0; j < N * N; j++) {
                if (i > N && j == i - N)  // 下
                    result = start;
                if (i < N * (N - 1) && j == i + N)  // 上
                    result = start;
                if (i % N != 0 && j == i + 1)  // 右
                    result = start;
                if (i % N != 1 && j == i - 1)  // 左
                    result = start;
                w.at(i).at(j) = result;
                result = 0;
            }
        }
    };
    RoadGraph<N>& operator=(const RoadGraph<N>&) = default;
    int At(int x, int y) {
        return w.at(x).at(y);
    }
};

#endif