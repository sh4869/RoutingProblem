#include <cstdlib>
#include <iostream>
#include "solver.h"
#include "dijkstra.h"
#include "roadgraph.h"
#include "util.h"

void checkCost(int start, int end, double cost) {
    int startx = start / 9;
    int starty = start % 9;
    int endx = end / 9;
    int endy = end % 9;
    if (cost == std::abs(startx - endx) + std::abs(starty - endy)) {
        // std::cout << start << " " << end << " 正解" << std::endl;
    } else {
        std::cout << start << " " << end << " Error" << std::endl;
    }
}

int main(void) {
    auto address = readAddress("address.txt");
    auto map = RoadGraph<9>(1);
    Dijkstra<9> d = Dijkstra<9>();
    d.SetRoadGraph(map);
    ShortestPathMap pathmap = ShortestPathMap();
    Address::iterator itr = address.begin();
    for (; itr != address.end(); itr++) {
        auto tmp = address;
        tmp.erase(itr->first);
        pathmap.Add(itr->first, d.Slove(itr->first, tmp));
    }
    // Cost Check
        
    itr = address.begin();
    Address::iterator itr2 = address.begin();
    for (; itr != address.end(); itr++) {
        for (; itr2 != address.end(); itr2++) {
            if(itr->first != itr2->first){
                checkCost(itr->first,itr2->first,pathmap.GetPath(itr->first,itr2->first).cost);
            }
        }
    }
    Solver(pathmap, address).SloveGreedy(40);
}
