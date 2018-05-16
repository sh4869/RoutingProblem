#include <cmath>
#include <iostream>
#include "dijkstra.h"
#include "roadgraph.h"
#include "solver.h"
#include "util.h"

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
    auto solver = Solver(pathmap, address);
    // Cost Check
    PrintResult(solver.SolveGreedy(40), address);
    PrintResult(solver.SolveGreedyRandom(40), address);
    PrintResult(solver.SA(40), address);
}
