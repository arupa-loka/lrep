#include "Node.hpp"
#include "RnBTree.hpp"
#include <iostream>
#include <cstdio>
#include <cstdlib>

int main()
{
  int max_nodes = 60;
  int key;
  
  int seed = 1289650943; //time(0);
  std::cout << "seed= " << seed << std::endl;
  srand(seed);
  
  RnBTree<int> rnbtree;
  char command[128];
  int i;
  for (i=0; i<max_nodes; ++i)
  {
    key = rand() % 100;
    printf("insert(%i)\n", key); 
    if (!rnbtree.insert(key)) {
      --i;
      continue;
    }

    rnbtree.toGraphViz("rnbtree.gv");
    sprintf(command, "dot -Tgif -ornbtree%02i.gif rnbtree.gv", i);
    system(command);
    //sprintf(command, "pqiv -i btree%02i.gif", i);
    //sprintf(command, "open -a Preview rnbtree%02i.gif", i);
    //system(command);
  } 
    --i;
  /*
  rnbtree.toGraphViz("rnbtree.gv");
  sprintf(command, "dot -Tgif -ornbtree%02i.gif rnbtree.gv", i);
  system(command);
  //sprintf(command, "pqiv -i rnbtree%02i.gif", i);
  sprintf(command, "open -a Preview rnbtree%02i.gif", i);
  system(command);
  */
  printf("END\n");
  return 0;
}

