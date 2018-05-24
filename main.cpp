#include <cmath>
#include <iostream>
#include "dijkstra.h"
#include "roadgraph.h"
#include "solver.h"
#include "util.h"

static constexpr int x = 9;

int main(void) {
    auto address = readAddress("data/address.txt");
    auto map = RoadGraph(x, 5, 30);
    Dijkstra d = Dijkstra(x);
    d.SetRoadGraph(map);
    ShortestPathMap pathmap = ShortestPathMap(address, d);

    auto solver = Solver(pathmap, address, 40, 1000);

    std::cout << "------- 問1 -------" << std::endl;
    std::cout << ">>>>> Greedy法(一度，決め打ち)" << std::endl;
    PrintResult(solver.SolveGreedy(1), address);
    std::cout << ">>>>> Greedy法(ランダム選択を複数回)" << std::endl;
    PrintResult(solver.SolveGreedyRandom(1, 1000), address);
    std::cout << ">>>>> 焼きなまし法" << std::endl;
    PrintResult(solver.SA(), address);
    // 問2
    auto address2 = readAddress("data/address2.txt");
    auto mapdata2 = readMap("data/map2.txt");

    auto map2 = RoadGraph(x, 5, 10);
    map2.SetMapData(mapdata2);
    Dijkstra d2 = Dijkstra(x);
    d2.SetRoadGraph(map2);
    ShortestPathMap pathmap2 = ShortestPathMap(address2, d2);

    auto solver2 = Solver(pathmap2, address2, 73, 1000);

    std::cout << "-------問2-------" << std::endl;
    std::cout << ">>>>> Greedy法(一度，決め打ち)" << std::endl;
    PrintResult(solver2.SolveGreedy(1), address2);
    std::cout << ">>>>> Greedy法(ランダム選択を複数回)" << std::endl;
    PrintResult(solver2.SolveGreedyRandom(1, 1000), address2);
    std::cout << ">>>>> 焼きなまし法" << std::endl;
    PrintResult(solver2.SA(), address2);

    auto address3 = readAddress("data/address3.txt");
    auto mapdata3 = readMap("data/map3.txt");

    auto map3 = RoadGraph(x, 5, 10);
    map3.SetMapData(mapdata3);
    Dijkstra d3 = Dijkstra(x);
    d3.SetRoadGraph(map3);
    ShortestPathMap pathmap3 = ShortestPathMap(address3, d3);

    auto solver3 = Solver(pathmap3, address3, 32, 1000);
    solver3.setGoal(-1);

    std::cout << "-------問3-------" << std::endl;
    std::cout << ">>>>> Greedy法(一度，決め打ち)" << std::endl;
    PrintResult(solver3.SolveGreedy(1), address3);
    std::cout << ">>>>> Greedy法(ランダム選択を複数回)" << std::endl;
    PrintResult(solver3.SolveGreedyRandom(1, 1000), address3);
    std::cout << ">>>>> 焼きなまし法" << std::endl;
    PrintResult(solver3.SA(), address3);
}
