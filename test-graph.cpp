#include "GraphM.hpp"
//#include "DFS.hpp"
//#include "BFS.hpp"
#include "DFS_M.hpp"
#include "BFS_M.hpp"
#include "Queue.hpp"
#include "Pair.hpp"
#include "Stack2.hpp"
#include "GraphRandomGenerator.hpp"
#include <iostream>

int main(int argc, char * argv[])
{
  int vertex=10;
  int edges=10;
  bool digraph=false;
  char type[4]="dfs";
  int seed = 0;
  
  if ( argc >= 5 )
  {
    vertex = atoi(argv[1]);
    edges = atoi(argv[2]);
    digraph = atoi(argv[3]);
    strncpy(type,argv[4],3);
    seed = atoi(argv[5]);
    printf("- vertex %i\n- edges %i\n- digraph %i\n- type %s\n", vertex, edges, digraph, type);
  } else {
    printf("\nUsage: %s <vertex> <edges> <diagraph [0|1]> [dfs|bfs] <seed>\n", argv[0]);
    printf("Example: %s 10 10 0 dfs 0\n\n", argv[0]);
    return 0;
  } 
  
  GraphM graph(vertex,digraph);
  GraphRandomGenerator * gen=NULL;
  
  
  if (seed) {
    gen = new GraphRandomGenerator(graph,seed);
  } else {
    gen = new GraphRandomGenerator(graph);
  }
  gen->initExactGraph(edges);
  //std::cout << graph << std::endl;
  //gen.initProbGraph(50);
  std::cout << graph << std::endl;
        
  if (strncmp(type, "dfs",3)==0)
  {     
    DFS_M dfs(graph);
    dfs.dfs(0);
    //std::cout << dfs << std::endl;
    dfs.toGraphViz("./graphviz.gv");
  } else {
    BFS_M bfs(graph);
    bfs.bfs(0);
    //std::cout << bfs << std::endl;
    bfs.toGraphViz("./graphviz.gv");
  }

  printf("END\n");
  return 0;
}

