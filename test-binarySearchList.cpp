#include "LinkedList.hpp"
#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;

//#define MAX 100
#define MAX 5 //1000000
//#define MAX 10000000

typedef LinkedNode<int> T_Node;

T_Node* binarySearchPivot(T_Node* head1, T_Node* tail2, int size)
{
	int s = size;
	
	while(s>3)//head1!=tail2)
	{
		int l = (s) >> 1;
		int r = s-l;
		T_Node* tail1;
		T_Node* head2;
		
		tail1 = head1;
		int i = l-1;
		while(i) {
			tail1 = tail1->getNext();
			--i;
		}
		head2 = tail1->getNext();

		if (*head1<*tail1) {
			if (*head2<*tail2){
				if(*head1<*head2)
					return head1;
				else
					return head2;	  
			} else {
				// take right	  
				head1 = head2;
				s = r;
			}
		} else {
			// take left	  
			tail2=tail1;
			s = l;
		}
	}

	T_Node* min = head1;
	T_Node* cur = head1;
	while (--s) {
		cur = cur->getNext();
		if (*cur < *min)
			min=cur;
	}
		  
	return min;	  
}

T_Node* binarySearchPivot(T_Node* head, int size)
{
	T_Node* tail = head;
	int index = size-1;

	while (index) {
		tail = tail->getNext();
		--index;
	}

	cout << "Head: " << *head << ", Tail: " << *tail << ", size: " << size << endl;
	return binarySearchPivot(head, tail, size);
}

int list_sequential_search(int key, LinkedList<int> & list)
{
	LinkedNode<int> * p = list.getHead();
	int i = 0;
	int size = list.size();
	while(p && i<size)
	{
		if (key == p->getObj() )
		{
			return i;
		}
		p = p->getNext();
		++i;
	}
	return -1;
}

int list_binary_search(int key, LinkedList<int> & list)
{
	LinkedNode<int> * pl = list.getHead();
	//LinkedNode<int> * pr = list.getBack();
	LinkedNode<int> * pp = pl;
	//LinkedNode<int> * pprev = 0;
	int l = 0;
	int r = list.size()-1;
	int p = 0;
	int i = 0;

	while (l<=r)
	{
		p = ((r-l) >> 1) + l;
		//p = (r-l)*((key-pl->getObj())/(pr->getObj()-pl->getObj())) + l;
		/*
		while (i<p-1) { pp = pp->getNext(); ++i; }
		pprev = pp; 
		if (i<p) { pp = pp->getNext(); ++i; }
		*/
		while (i<p) { pp = pp->getNext(); ++i; }
		
		if ( key < pp->getObj() )
		{
			//pr = pprev; // pp - 1
			r = p-1;
			pp = pl;
			i = l;
		} 
		else if ( key > pp->getObj() ) 
		{
			pl = pp->getNext();
			l = p+1;
			//i = p;
		}
		else 
		{
			return p;
		}
	}
	
	return -1;
}

int main()
{
/*	
 	// Binary search starting from a Random Element of the sorte list
	LinkedList<int> aQ;
	int seed = time(NULL);
	srand(seed);
	printf("seed= %i\n",seed);
	
	for (int i=0; i<MAX; ++i)
	{
		int a = rand() % 1000;
		aQ.pushBack(a);
	}
	cout << aQ << endl;
	cout << "Sorting..." << endl;
	aQ.sort2();
	cout << aQ << endl;
	T_Node* H = aQ.getRandomHead();
	cout << "Random Head = " << *H << endl;

	T_Node* min = binarySearchPivot(H, aQ.size());
	cout << "Minimun elem = " << *min << endl;
*/	
	// Comparison between sequential serach and binarysearch on list
	LinkedList<int> list;
	// The list is sorted in ascending order by construction
	for (int i=0; i<MAX; ++i)
	{
		list.pushBack(i);
	}
	// get a random key element that we know is in the list
	int seed = time(0);
	srand(seed);
	int key; // = rand() % MAX;
	int idx1, idx2;
	
	while (cin >> key)
	{		  
		cout << "search key=" << key << endl;
	
		// test the two searching methods
	
		idx1 = list_sequential_search(key, list);
		cout << "seq_search idx=" << idx1 << endl;
	
		idx2 = list_binary_search(key, list);
		cout << "bin_search idx=" << idx2 << endl;
	}
	printf("END\n");
	return 0;
}

