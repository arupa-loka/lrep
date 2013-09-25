#ifndef _DOUBLY_LINKED_LIST_HPP
#define _DOUBLY_LINKED_LIST_HPP

#include <iostream>
#include "Node.hpp"
#include "Pair.hpp"

template< class T >
class DoublyLinkedNode
{
	public:
	DoublyLinkedNode(): _next(NULL), _prev(NULL){}
	T getObj() const {return _obj;}
	DoublyLinkedNode<T>* getNext() const {return _next;}
	DoublyLinkedNode<T>* getPrev() const {return _prev;}
	void setObj( T iObj){_obj=iObj;}
	void setNext( DoublyLinkedNode<T>* iNext){_next=iNext;}
	void setPrev( DoublyLinkedNode<T>* iPrev){_prev=iPrev;}
	inline bool operator<(DoublyLinkedNode<T> & iNode);

	std::ostream& toStream(std::ostream& oOut);

	private:
	DoublyLinkedNode<T> * _next;
	DoublyLinkedNode<T> * _prev;
	T _obj;
};


template< class T >
class DoublyLinkedNode<T *>
{
	public:
	DoublyLinkedNode():_obj(NULL), _next(NULL), _prev(NULL){}
	T* getObj() const {return _obj;}
	DoublyLinkedNode<T*>* getNext() const {return _next;}
	DoublyLinkedNode<T*>* getPrev() const {return _prev;}
	void setObj( T* iObj){_obj=iObj;}
	void setNext( DoublyLinkedNode<T*>* iNext){_next=iNext;}
	void setPrev( DoublyLinkedNode<T*>* iPrev){_prev=iPrev;}
	inline bool operator<(DoublyLinkedNode<T*> & iNode);

	std::ostream& toStream(std::ostream& oOut);

	private:
	DoublyLinkedNode<T*> * _next;
	DoublyLinkedNode<T*> * _prev;
	T* _obj;
};


template<class T> class DoublyLinkedListIterator;

template< class T >
class DoublyLinkedList
{

	friend class DoublyLinkedListIterator<T>;
	
	public:
	DoublyLinkedList():_head(NULL),_size(0){}
	
	T getFront() { return _head->getObj(); }
	T getBack() const {
		return _head->getPrev()->getObj();
	}
	
	DoublyLinkedListIterator<T> getIterator() { DoublyLinkedListIterator<T> aIt(*this); return aIt; }
	int size() const { return _size; }

	void pushBack( T iObj ){
		DoublyLinkedNode<T>* aDoublyLinkedNode = new DoublyLinkedNode<T>();
		aDoublyLinkedNode->setObj(iObj);
		if (_head!=NULL) {
			if (_head->getPrev() != _head) {
				// add new node at the end of the list
				DoublyLinkedNode<T>* aOldHeadPrev = _head->getPrev();
				_head->setPrev(aDoublyLinkedNode);
				aOldHeadPrev->setNext(aDoublyLinkedNode);
				aDoublyLinkedNode->setPrev(aOldHeadPrev);
				aDoublyLinkedNode->setNext(_head);
			} else {
				_head->setPrev(aDoublyLinkedNode);
				_head->setNext(aDoublyLinkedNode);
				aDoublyLinkedNode->setPrev(_head);
				aDoublyLinkedNode->setNext(_head);
			}
		} else {
			_head = aDoublyLinkedNode;
			_head->setPrev(_head);
			_head->setNext(_head);
		}
		++_size;
	}
	
	void pushFront( T iObj ){
		pushBack(iObj);
		_head = _head->getPrev();
	}
	
	void pushBack( T iObj, const int iTimes ){
		for (int i=iTimes; i>0; --i)
			pushBack(iObj);
	}
	
	void pushFront( T iObj, const int iTimes ){
		for (int i=iTimes; i>0; --i)
			pushFront(iObj);
	}
	
	void pushBack(const DoublyLinkedList<T>& iList) {
		for (DoublyLinkedListIterator<T> aIt = iList.getIterator(); !aIt.end(); ++aIt )
			pushBack(*aIt);
	}

	void pushFront(const DoublyLinkedList<T>& iList) {
		for (DoublyLinkedListIterator<T> aIt = iList.getIterator(); !aIt.end(); ++aIt )
			pushFront(*aIt);
	}
	
	void popBack() {
		if (_head!=NULL) {
			if ( _head->getPrev() != _head ) {
				DoublyLinkedNode<T> * aTmpBack = _head->getPrev();
				_head->getPrev()->getPrev()->setNext(_head);
				_head->setPrev(_head->getPrev()->getPrev());
				delete aTmpBack;
			} else {
				delete _head;
				_head = NULL;
			}
			--_size;
		}
	}

	void popFront() {
		if (_head!=NULL) {
			if ( _head->getPrev() != _head ) {
				DoublyLinkedNode<T>* aTmpFront = _head;
				_head->getPrev()->setNext(_head->getNext());
				_head->getNext()->setPrev(_head->getPrev());
				_head = _head->getNext();
				delete aTmpFront;
			} else {
				delete _head;
				_head = NULL;
			}
			--_size;
		}
	}
	
	void clear() {
		while (_size>0 && _head!=NULL) {
			popBack();
		} 
	}

	bool empty() const  {
		return (_head==NULL);
	}
	
	std::ostream& toStream(std::ostream& oOut);
	void sort2();
	void sort3();
	void reverse();
	
	private:
	DoublyLinkedNode<T> * _head;
	int _size;

};



template <class T>
class DoublyLinkedListIterator
{
	public:
	DoublyLinkedListIterator(DoublyLinkedList<T> & iList): 
		_list(iList),
		_current(_list._head),
		_over(false) {}

	void reset() {
		_current = _list._head;
		_over = false;
	}
	
	bool end() { 
		return (_current==NULL || _over); 
	}

	DoublyLinkedListIterator<T> & operator++() { 
		_current=_current->getNext();
		if (_current == _list._head)
		{
			_over=true;
		}
		return *this; 
	}
	
	const DoublyLinkedListIterator<T> operator++(int) { 
		DoublyLinkedListIterator<T> tmp = *this;
		++(*this); 
		return tmp; 
	}
	
	T operator*() { 
		return _current->getObj(); 
	} 
	
	private:
	DoublyLinkedList<T>& _list;
	DoublyLinkedNode<T>* _current;
	bool _over;
};



#include "DoublyLinkedList.cpp"

#endif

