#include "graph.h"
#include <iostream>
#include <string>

using namespace graph;

int main(){
  Graph g = fromCSV("graph_coo.csv");
  std::cout << findWalks(g) << std::endl;
}
