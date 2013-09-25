#include "Node.hpp"
#include "AVLTree.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	AVLTree<Node*> aQ;
	Node * aNode=NULL;
	
	srand(3);
	
	
	std::cout << "Building Tree from sequence..." << std::endl;
	for (int i=0; i<1000; ++i)
	{
		int a = rand() % 1000;
		aQ.push(new Node(a));
		std::cout << a << ", ";
	}
	std::cout << std::endl;
	
	std::cout << "Visiting in Order..." << std::endl;
	for (AVLTreeInOrderIterator<Node*> i=aQ.getInorderIt(); !i.end(); ++i)
	{
		std::cout<< *(*i) << ", ";
	}
	std::cout << std::endl;
	

	aQ.pop(new Node(40));
/*	
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

	aQ.clean();
	std::cout << "Visiting in Order..." << std::endl;
	for (BinarySearchTreeInOrderIterator<Node*> i=aQ.getInorderIt(); !i.end(); ++i)
	{
		std::cout<< *(*i) << ", ";
	}
	std::cout << std::endl;
*/

	printf("END\n");
	return 0;
}

