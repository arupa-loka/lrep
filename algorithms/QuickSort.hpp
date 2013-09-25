#ifndef _QUICK_SORT_HPP
#define _QUICK_SORT_HPP

#include <stdio.h>
#include <stdlib.h>
#include "Stack.hpp"
#include "Pair.hpp"

template<class T>
void swap(T& a, T& b)
{
	int tmp = a;
	a = b;
	b = tmp;
}

void quick_sort(int A[], int N)
{
	Stack< Pair<int,int>* >stack;
	stack.push(new Pair<int,int>(0,N-1));

	while( !stack.empty() )
	{
		Pair<int,int> * pair = stack.getTop();
		stack.pop();
		int l = pair->first();
		int r = pair->second();
		delete pair;
		int i = l; 
		int j = r+1;
		int pivot = A[l];
		
		while (j>i)
		{
			do {++i;} while ( i<=j && A[i]<pivot);
			do {--j;} while ( j>=i && A[j]>pivot);
			if (j>i)
			{
				swap(A[j],A[i]);
			}
		}
		swap(A[j],A[l]);

		if (j-1-l>0) stack.push(new Pair<int,int>(l,j-1) );
		if (r-(j+1)>0) stack.push(new Pair<int,int>(j+1,r));

	}
}

/*
void quick_sort2(int A[], int N)
{
	Stack< Pair<int,int> >stack;
	stack.push( Pair<int,int>(0,N-1) );

	while( !stack.empty() )
	{
		Pair<int,int> pair = stack.getTop();
		stack.pop();
		int l = pair.first();
		int r = pair.second();
		int i = l; 
		int j = r+1;
		int pivot = A[l];
		
		while (j>i)
		{
			do {++i;} while ( i<r && A[i]<pivot);
			do {--j;} while ( j>l && A[j]>pivot);
			if (j>i)
			{
				swap(A[j],A[i]);
			}
		}
		swap(A[j],A[l]);

		if (j-1-l>0) stack.push( Pair<int,int>(l,j-1) );
		if (r-(j+1)>0) stack.push( Pair<int,int>(j+1,r) );

	}
}
*/
void init_random_array(int A[], int n)
{
	int seed=time(NULL);
	printf("seed=%i\n",seed);
	srand(seed);
	
	for(int i=0; i<n; ++i)
	{
		A[i]=rand()%100;
	}

	return;
}

void dump_array(int A[], int n)
{
	for(int i=0; i<n; ++i)
	{
		printf("%i, ", A[i]);
	}
	printf("\n");
	return;
}

void insertion_sort(int A[], int n)
{
	for (int i=1; i<n; ++i)
	{
		int key = A[i];
		int j = i-1;

		while ( j>=0 && key<A[j] )
		{
			A[j+1]=A[j];
			--j;
		}
		A[j+1]=key;
	}
	return;
}

void selection_sort(int A[], int n)
{
	for (int i=0; i<n-1; ++i)
	{
		int z = i;
		for (int j=i+1; j<n; ++j)
		{
			if (A[j]<A[z])
			{
				z = j;
			}
		}
		if (z!=i)
		{
			int min = A[z];
			A[z]=A[i];
			A[i]=min;
		}	
	}
	return;
}

void bubble_sort(int A[], int n)
{
	do 
	{
		int z=n;
		for (int j=0; j<n; ++j)
		{
			if (A[j]>A[j+1])
			{
				int tmp = A[j];
				A[j] = A[j+1];
				A[j+1] = tmp;
				z=j+1;
			}
		}
		n=z;
	} while (n>1);

	return;
}

void heap_sort(int A[], int n)
{
	int l = n/2+1; 
	int r = n+1;
	int K;
	int i,j;
	
	while (l!=r)
	{
		if (l>1) {
			--l; K=A[l-1];			
		} else {
			--r;
			K=A[r-1];
			A[r-1]=A[0];
		}
		
		i=l;
		j=2*i;
		while(j<r)
		{
			if ( j+1<r && A[j-1]<A[j] ) {
				++j;
			}
			if ( K>A[j-1] ) {
				break;
			} else {
				A[i-1]=A[j-1];
				i=j;
			}
			j=2*i;
		}
		
		A[i-1]=K;
	}
}
#endif
