#ifndef PROBLEM_H_
#define PROBLEM_H_

#include <iostream>
#include <string>

#include "dijkstra.h"
#include "roadgraph.h"
#include "solver.h"
#include "types.h"
#include "util.h"

class Problem {
    Address address;
    DijkstraSolver dijkstra;
    RoadGraph graph;
    ShortestPathMap pathmap;
    Solver solver;

public:
    Problem() = default;
    Problem(std::string addresfile,
            int cost,
            int deafultspeed,
            int length,
            int start,
            int upperlimit) {
        address = readAddress(addresfile);
        dijkstra = DijkstraSolver(length);
        graph = RoadGraph(length, cost, deafultspeed);
        dijkstra.SetRoadGraph(graph);
        pathmap = ShortestPathMap(address, dijkstra);
        solver = Solver(pathmap, address, start, upperlimit);
    }

    Problem(std::string addresfile,
            int cost,
            int deafultspeed,
            int length,
            int start,
            int upperlimit,
            std::string mapfile) {
        address = readAddress(addresfile);
        auto map = readMap(mapfile);
        dijkstra = DijkstraSolver(length);
        graph = RoadGraph(length, cost, deafultspeed);
        graph.SetMapData(map);
        dijkstra.SetRoadGraph(graph);
        pathmap = ShortestPathMap(address, dijkstra);
        solver = Solver(pathmap, address, start, upperlimit);
    }

    void DisplayResult() {
        std::cout << ">>>>> Greedy法(一度，決め打ち)" << std::endl;
        PrintResult(solver.SolveGreedy(1), address);
        std::cout << ">>>>> Greedy法(ランダム選択を複数回)" << std::endl;
        PrintResult(solver.SolveGreedyRandom(1, 1000), address);
        std::cout << ">>>>> 焼きなまし法" << std::endl;
        PrintResult(solver.SA(), address);
    }
};

#endif