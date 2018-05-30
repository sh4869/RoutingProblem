#include <cmath>
#include <iostream>
#include "dijkstra.h"
#include "problem.h"
#include "roadgraph.h"
#include "solver.h"
#include "util.h"

static constexpr int x = 9;

int main(void) {
    auto no1 = Problem("data/address.txt", 5, 30, x, 40, 40, 1000);
    std::cout << "------- 問1 -------" << std::endl;
    no1.DisplayResult();
    // no1.GreedyRandomGraph();
    // no1.SAGraph();
    auto no2 = Problem("data/address2.txt", 5, 10, x, 73, 73, 1000, "data/map2.txt");
    std::cout << "-------問2-------" << std::endl;
    no2.DisplayResult();
    // no2.GreedyRandomGraph();
    // no2.SAGraph();
    auto no3 = Problem("data/address3.txt", 5, 10, x, 32, -1, 1000, "data/map3.txt");
    std::cout << "-------問3-------" << std::endl;
    no3.DisplayResult();
    // no3.GreedyRandomGraph();
    // no3.SAGraph();
    return 0;
}
