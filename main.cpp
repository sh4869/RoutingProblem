#include <cmath>
#include <iostream>
#include "dijkstra.h"
#include "roadgraph.h"
#include "solver.h"
#include "util.h"

void checkCost(Address address, ShortestPathMap pathmap) {
    Address::iterator itr = address.begin();
    Address::iterator itr2 = address.begin();
    for (; itr != address.end(); itr++) {
        for (; itr2 != address.end(); itr2++) {
            if (itr->first != itr2->first) {
                int start = itr->first;
                int end = itr2->first;
                int startx = start / 9;
                int starty = start % 9;
                int endx = end / 9;
                int endy = end % 9;
                if (pathmap.GetPath(itr->first, itr2->first).cost ==
                    std::abs(startx - endx) + std::abs(starty - endy)) {
                    // std::cout << start << " " << end << " 正解" << std::endl;
                } else {
                    std::cout << start << " " << end << " Error" << std::endl;
                }
            }
        }
    }
}

int main(void) {
    auto address = readAddress("data/address.txt");
    auto map = RoadGraph<9>(1);
    Dijkstra<9> d = Dijkstra<9>();
    d.SetRoadGraph(map);
    ShortestPathMap pathmap = ShortestPathMap();
    Address::iterator itr = address.begin();
    for (; itr != address.end(); itr++) {
        auto tmp = address;
        tmp.erase(itr->first);
        pathmap.Add(itr->first, d.Solve(itr->first, tmp));
    }
    auto solver = Solver(pathmap, address,40);
    // Cost Check
    PrintResult(solver.SolveGreedy(), address);
    PrintResult(solver.SolveGreedyRandom(), address);
    PrintResult(solver.SA(), address);
}
