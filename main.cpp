#include <cmath>
#include <iostream>
#include "dijkstra.h"
#include "roadgraph.h"
#include "solver.h"
#include "util.h"

static constexpr int x = 9;

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
    std::cout << "------- 問1 -------" << std::endl;
    
    auto address = readAddress("data/address.txt");
    auto map = RoadGraph(x, 5, 30);
    Dijkstra d = Dijkstra(x);
    d.SetRoadGraph(map);
    ShortestPathMap pathmap = ShortestPathMap();
    pathmap.AddAdress(address, d);

    auto solver = Solver(pathmap, address, 40);
    std::cout << ">>>>> Greedy法(一度，決め打ち)" << std::endl;
    PrintResult(solver.SolveGreedy(), address);
    std::cout << ">>>>> Greedy法(ランダム選択を複数回)" << std::endl;
    PrintResult(solver.SolveGreedyRandom(), address);
    std::cout << ">>>>> 焼きなまし法" << std::endl;
    PrintResult(solver.SA(), address);
    std::cout << ">>>>> Greedy法(複数台，決め打ち)" << std::endl;
    solver.SolveGreedyMulti(2);
    std::cout << ">>>>> Greedy法(複数台，ランダム)" << std::endl;
    solver.SolveGreedyMultiRandom(2);
    
    std::cout << "-------問2-------" << std::endl;
    // 問2
    auto address2 = readAddress("data/address2.txt");
    auto mapdata2 = readMap("data/map2.txt");

    auto map2 = RoadGraph(x, 5, 10);
    map2.SetMapData(mapdata2);
    Dijkstra d2 = Dijkstra(x);
    d2.SetRoadGraph(map2);
    ShortestPathMap pathmap2 = ShortestPathMap();
    pathmap2.AddAdress(address2, d2);

    auto solver2 = Solver(pathmap2, address2, 73);
    std::cout << ">>>>> Greedy法(一度，決め打ち)" << std::endl;
    PrintResult(solver2.SolveGreedy(), address2);
    std::cout << ">>>>> Greedy法(ランダム選択を複数回)" << std::endl;
    PrintResult(solver2.SolveGreedyRandom(), address2);
    std::cout << ">>>>> 焼きなまし法" << std::endl;
    PrintResult(solver2.SA(), address2);
    std::cout << ">>>>> Greedy法(複数台，決め打ち)" << std::endl;
    solver2.SolveGreedyMulti(2);
    std::cout << ">>>>> Greedy法(複数台，ランダム)" << std::endl;
    solver2.SolveGreedyMultiRandom(2);
}
