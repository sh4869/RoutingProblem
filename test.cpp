#include <cstdlib>
#include <iostream>
#include "dijkstra.h"
#include "roadgraph.h"
#include "util.h"
#include "Greedy.h"

void checkCost(int start, int end, double cost) {
    int startx = start / 9;
    int starty = start % 9;
    int endx = end / 9;
    int endy = end % 9;
    if (cost == std::abs(startx - endx) + std::abs(starty - endy)) {
        // std::cout << start << " " << end << " 正解" << std::endl;
    }
}

int main(void) {
    auto list = readAddress("address.txt");
    auto map = RoadGraph<9>(1);
    Dijkstra<9> d = Dijkstra<9>();
    d.SetRoadGraph(map);
    ShortestPathMap pathmap = ShortestPathMap();
    for (auto i = 0; i < list.size(); i++) {
        auto tmp = list;
        tmp.erase(tmp.begin() + i);
        pathmap.Add(list.at(i).node, d.Slove(list.at(i).node, tmp));
    }
    // Cost Check
    for (auto i = 0; i < list.size(); i++) {
        for (auto j = 0; j < list.size(); j++) {
            auto start = list.at(i).node;
            auto end = list.at(j).node;
            if (start != end) {
                checkCost(start, end, pathmap.GetPath(start, end).cost);
            }
        }
    }
    Greedy(pathmap,list).Slove(40);
}
