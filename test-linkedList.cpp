#include "Node.hpp"
#include "LinkedList.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

int main()
{
	LinkedList<Node*> aQ;
	Node * aNode=NULL;
	//printList(aQ);
	srand(3);
	
	for (int i=0; i<10000; ++i)
	{
		int a = rand() % 1000;
		int w = rand() % 2;

		if (w) {
			aQ.pushBack(new Node(a));
		} else {
			aQ.pushFront(new Node(a));
		}
	}
	//printList(aQ);
	std::cout << aQ << std::endl;
	std::cout << "Sorting..." << std::endl;
	aQ.sort2();
	std::cout << aQ << std::endl;
	//printList(aQ);

	/*
	aNode = new Node(0);
	aQ.pushBack(aNode);
	//printList(aQ);
	
	aNode = new Node(1);
	aQ.pushBack(aNode);
	//printList(aQ);
	
	aNode = new Node(2);
	aQ.pushFront(aNode);
	//printList(aQ);
	
	aNode = new Node(3);
	aQ.pushFront(aNode);
	//printList(aQ);
	
	std::cout << "Sorting..." << std::endl;
	aQ.sort();
	printList(aQ);
	
	//aQ.popBack();
	//printList(aQ);
	
	//aQ.popBack();
	//printList(aQ);
	
	//aQ.popFront();
	//printList(aQ);
	*/
	printf("END\n");
	return 0;
}

