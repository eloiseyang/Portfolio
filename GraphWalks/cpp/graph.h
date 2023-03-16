/* 
 * 03/15/2023
 * Graph Walks Solution
 * EFJY
 */

#include <cstddef>
#include <string>
#include <vector>
#include <stack>
#include <unordered_set>
#include <unordered_map>

namespace graph{

  struct Vertex{
    int i;
    std::vector<int> parents;
    std::vector<int> children;
    Vertex(int id): i(id){}
    Vertex(): i(-1){}
  };

  class Edge{
  public:
    int tail;
    int head;
    Edge(): head(-1), tail(-1){}
    Edge(int h, int t): head(h), tail(t){}
    bool operator==(const Edge& e) const;
  };
  
  /* Class unordered_map calls Hasher() to hash edges,
   * or head tail pairs. */
  class Hasher{
  public:
    size_t operator()(const Edge& e) const;
  };
  
  // A class for directed acyclic graphs with edge labels.
  class Graph{
    std::stack<int> order;
    std::unordered_set<int> visited;
  public:
    std::unordered_map<int, Vertex> vertexMap;
    std::unordered_map<Edge, char, Hasher> edgeMap;
    std::unordered_map<char, std::vector<Edge>> labelMap;
    Graph(){}
    void add(int head, int tail, char label);
    Vertex next();
    Vertex get(int i);
    char get(int i, int j);
    std::vector<Edge> get(char a);
  };

  int findWalks(Graph g);
  Graph fromCSV(std::string filename);
  int test();
}
