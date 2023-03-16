/* 
 * 03/15/2023
 * Graph Walks Solution
 * EFJY
 */

#include "graph.h"
#include <iostream>
#include <fstream>

using namespace graph;

bool Edge::operator==(const Edge& e) const{
  return tail == e.tail && head == e.head;
}

// Multiplicative hash using Knuth's constant
size_t Hasher::operator()(const Edge& e) const{
  unsigned int t = e.tail;
  unsigned int h = e.head;
  t = t *265449151u;
  h = h *265449151u;
  return h^t;
}

void Graph::add(int head, int tail, char label){
  if(vertexMap[head].i == -1){
    vertexMap[head] = Vertex(head);
  }
  if(vertexMap[tail].i == -1){
    vertexMap[tail] = Vertex(tail);
  }
  Edge ht(head, tail);
  edgeMap[ht] = label;
  labelMap[label].push_back(ht);
  vertexMap[head].children.push_back(tail);
  vertexMap[tail].parents.push_back(head);
}

Vertex Graph::next(){
  Vertex v;
  if(visited.size() == 0){
    for(auto p: vertexMap){
      Vertex v = std::get<1>(p);
      if(v.parents.size() == 0){
	order.push(v.i);
      }
    }
  }
  while(order.size() != 0){
    int i = order.top();
    v = vertexMap[i];
    for(int c: v.children){
      if(visited.count(c) == 0){
	visited.insert(c);
	order.push(c);
      }
    }
    if(v.i == order.top()){
      order.pop();
      return v.i;
    }
  }
  visited.clear();
  return -1;
}

Vertex Graph::get(int i){
  return(vertexMap[i]);
}
char Graph::get(int i, int j){
  Edge e(i, j);
  return(edgeMap[e]);
}
std::vector<Edge> Graph::get(char a){
  return(labelMap[a]);
}

Graph graph::fromCSV(std::string filename){
  Graph g;
  std::fstream f("graph_coo.csv");
  char h[50];
  char t[50];
  char l[50];
  
  do{
    f.getline(h, 50, ',');
    f.getline(t, 50, ',');
    f.getline(l, 50);
    if((47 < h[0] && h[0] < 58) &&
       (47 < t[0] && t[0] < 58)){
      g.add(std::stoi(h), std::stoi(t), l[0]);
    }
  }while(h[0] != 0);
  f.close();
  return g;
}

int graph::findWalks(Graph g){
  int i = 0;
  int total = 0;
  std::unordered_map<int, int> counts;
  while(i >= 0){
    i = g.next().i;
    for(int c: g.get(i).children){
      if(g.get(i, c) == 'C'){
	counts[i] += 1;
      }
    }
    for(int p: g.get(i).parents){
      counts[p] += counts[i];
    }
  }
  std::vector<Edge> bEdges = g.get('B');
  for(Edge co: bEdges){
    int c = co.tail;
    total += counts[c];
  }
  return total;
}
