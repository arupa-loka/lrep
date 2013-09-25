#include "Node.hpp"
#include "BinarySearchTree.hpp"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>

int main()
{
	BinarySearchTree<Node*> aQ;
	Node * aNode=NULL;
	//printList(aQ);
	//int seed = time(NULL);
	int seed = 1282412786;
	printf("seed= %i\n", seed);
	srand(seed);
	
	
	std::cout << "Building Tree from sequence..." << std::endl;
	for (int i=0; i<17; ++i)
	{
		int a = rand() % 100;
		aQ.push(new Node(a));
		std::cout << a << ", ";
	}
	std::cout << std::endl;
	//aQ.toGraphViz("./binaryTree.gv");
	aQ.InOrder();
	aQ.PreOrder();
	aQ.PostOrder();
	std::cout << "Visiting in Order..." << std::endl;
	for (BinarySearchTreeInOrderIterator<Node*> i=aQ.getInorderIt(); !i.end(); ++i)
	{
		std::cout<< *(*i) << ", ";
	}
	std::cout << std::endl;
/*
	aQ.pop(new Node(40));
	std::cout << "Visiting in Order..." << std::endl;
	for (BinarySearchTreeInOrderIterator<Node*> i=aQ.getInorderIt(); !i.end(); ++i)
	{
		std::cout<< *(*i) << ", ";
	}
	std::cout << std::endl;
	//std::cout << aQ << std::endl;
	
	aQ.pop(new Node(40));
	std::cout << "Visiting in Order..." << std::endl;
	for (BinarySearchTreeInOrderIterator<Node*> i=aQ.getInorderIt(); !i.end(); ++i)
	{
		std::cout<< *(*i) << ", ";
	}
	std::cout << std::endl;
*/
	aQ.clean();
	std::cout << "Visiting in Order..." << std::endl;
	for (BinarySearchTreeInOrderIterator<Node*> i=aQ.getInorderIt(); !i.end(); ++i)
	{
		std::cout<< *(*i) << ", ";
	}
	std::cout << std::endl;


	printf("END\n");
	return 0;
}

