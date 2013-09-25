#include <iostream>
#include <cstdlib>
#include "BTree.hpp"
using namespace std;

int main(int argc, char* argv[])
{
	int order = 5;
    int max_nodes = 60;
	int key;
	
	int seed = 1289650943; //time(0);
	cout << "seed= " << seed << endl;
	srand(seed);
	
	BTree<int> b(order);
	char command[128];
	int i;
	for (i=0; i<max_nodes; ++i)
	{
		key = rand() % 100;
		if (!b.insert(key)) {
			--i;
			continue;
		}
        printf("insert(%i)\n", key); 

		//b.toGraphViz("btree.gv");
		//sprintf(command, "dot -Tgif -obtree%02i.gif btree.gv", i);
		//system(command);
		//sprintf(command, "pqiv -i btree%02i.gif", i);
		//system(command);
	}	
    --i;
	b.toGraphViz("btree.gv");
	sprintf(command, "dot -Tgif -obtree%02i.gif btree.gv", i);
	system(command);
	sprintf(command, "pqiv -i btree%02i.gif", i);
	system(command);

    b.remove(83);
    b.remove(84);
    b.remove(77);
    b.remove(78);
    b.remove(75);
    b.toGraphViz("btree.gv");
	sprintf(command, "dot -Tgif -obtree%02i.gif btree.gv", ++i);
	system(command);
	sprintf(command, "pqiv -i btree%02i.gif", i);
	system(command);

    b.remove(79);
    b.toGraphViz("btree.gv");
	sprintf(command, "dot -Tgif -obtree%02i.gif btree.gv", ++i);
	system(command);
	sprintf(command, "pqiv -i btree%02i.gif", i);
	system(command);

	/*
    printf ("remove key: ");
	while ( cin >> key && key>-1)
	{
		b.remove(key);
		b.toGraphViz("btree.gv");
        sprintf(command, "dot -Tgif -obtree%02i.gif btree.gv", i);
		system(command);
		sprintf(command, "pqiv -i btree%02i.gif", i);
		system(command);
        ++i;
        printf ("remove key: ");
	}
	*/
	return 0;
}

