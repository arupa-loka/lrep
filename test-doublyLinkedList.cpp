#include "Node.hpp"
#include "DoublyLinkedList.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

int main()
{
	DoublyLinkedList<Node*> aQ;
	Node * aNode=NULL;
	srand(3);
	
	for (int i=0; i<5; ++i)
	{
		int a = rand() % 10;
		int w = rand() % 2;

		if (w) {
			aQ.pushBack(new Node(a));
		} else {
			aQ.pushFront(new Node(a));
		}
	}
	std::cout << aQ << std::endl;
	//std::cout << "Sorting..." << std::endl;
	//aQ.sort3();
	//std::cout << aQ << std::endl;
	std::cout << "Reversing..." << std::endl;
	aQ.reverse();

	std::cout << aQ << std::endl;

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

