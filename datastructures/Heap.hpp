#ifndef _HEAP_HPP
#define _HEAP_HPP

#include <iostream>
#include "Vector.hpp"
#include "basic_op.hpp"

template<class T>
class Heap 
{
	public:
	Heap();
	T& min();
	void pop();
	void push(T a);
	int size();
	bool empty();
  void realloc(int new_size);
	~Heap();
	
	std::ostream& toStream(std::ostream& oOut);
	
	private:
	void siftDown();
	void siftUp();
	Vector<T> vector_;	
};

template< class T >
Heap<T>::Heap() : 
		  vector_() {} 

template< class T >
void Heap<T>::push(T a)
{
	vector_.pushBack(a);
	siftUp();
}

template< class T >
void Heap<T>::siftUp()
{
	int i = vector_.size();
	int p = i>>1;
	while (p>0 && vector_[i-1]<vector_[p-1]) // to get min heap
	//while (vector_[i]>vector_[p]) // to get max heap
	{
		swap(vector_[i-1], vector_[p-1]);
		i = p;
		p = i>>1;		  
	}
}

template< class T >
void Heap<T>::pop()
{
	vector_[0] = vector_[vector_.size()-1];
	vector_.popBack();
	siftDown();
}

template< class T >
void Heap<T>::siftDown()
{
	T k = vector_[0];

	int i=1;
	int j=i<<1;

	while(j<=vector_.size())
	{
		//if (j+1<=_heap_size && vector_[j-1]>vector_[j]) // to get min heap
		if (j+1<=vector_.size() && vector_[j-1]>vector_[j]) // to get min heap
		//if (j-1<_heap_size+1 && vector_[j-1]<vector_[j]) // to get max heap
		{
			++j;
		}
		if (vector_[j-1]>k) { // to get min heap
		//if (vector_[j-1]<k) { // to get max heap
			break;
		} else {
			vector_[i-1] = vector_[j-1];
			i=j;
			j=i<<1;
		}
	}
	vector_[i-1]=k;
	return;
}

template< class T >
Heap<T>::~Heap()
{
}

template< class T >
int Heap<T>::size()
{
	return vector_.size();
}

template< class T >
void Heap<T>::realloc(int new_size)
{
	return vector_.realloc(new_size);
}

template< class T >
bool Heap<T>::empty()
{
	return (vector_.empty());
}

template <class T>
std::ostream& Heap<T>::toStream(std::ostream& oOut)
{
	oOut << vector_;
	return oOut;
}


template <class T>
std::ostream& operator<<(std::ostream& oOut, Heap<T> & iObj) { return iObj.toStream(oOut); }

#endif

