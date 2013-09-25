#ifndef _DOUBLY_LINKED_LIST_CPP
#define _DOUBLY_LINKED_LIST_CPP

#include "DoublyLinkedList.hpp"

template<class T>
void printLR(DoublyLinkedNode<T>* iH)
{
	while (iH!=NULL) { 
	   std::cout << *iH;
		std::cout << ", ";
		iH = iH->getNext();
	}  
}
template<class T>
void printRL(DoublyLinkedNode<T>* iH)
{
	while (iH!=NULL) { 
	   std::cout << *iH;
		std::cout << ", ";
		iH = iH->getPrev();
	}
}

template<class T>
void printLR2( DoublyLinkedNode<T>* iH)
{
	DoublyLinkedNode<T>* aT = iH;
	do { 
	   std::cout << *aT;
		std::cout << ", ";
		aT = aT->getNext();
	} while (iH!=aT); 
}

template<class T>
void printLinks( DoublyLinkedNode<T>* iH)
{
	DoublyLinkedNode<T>* aT = iH;
	do { 
	   std::cout << *aT;
		std::cout << "("<< *(aT->getNext()) <<","<< *(aT->getPrev())<<") - ";
		aT = aT->getNext();
	} while (iH!=aT); 
}

template<class T>
bool DoublyLinkedNode<T>::operator<(DoublyLinkedNode<T>& iNode)
{
	return ( (this->getObj()) < (iNode.getObj()) );
}

template<class T>
bool DoublyLinkedNode<T*>::operator<(DoublyLinkedNode<T*>& iNode)
{
	return ( *(this->getObj()) < *(iNode.getObj()) );
}


template<class T>
void DoublyLinkedList<T>::reverse()
{
	if(_size<=1) return;

	DoublyLinkedNode<T> * l = _head;
	DoublyLinkedNode<T> * r = _head->getPrev();
	DoublyLinkedNode<T> * t = NULL;
	DoublyLinkedNode<T> * newHead = r;
	std::cout << *l << std::endl;
	std::cout << *r << std::endl;

	//for (int i=0; i<(_size>>1); ++i )
	for (int i=0; i<2; ++i )
	{
		//swap
		r->getPrev()->setNext( l );
		l->getNext()->setPrev( r );

		t = l->getNext();
		if (r->getNext() != l) {
			l->setNext( r->getNext() );
			l->getPrev()->setNext( r );
		} else {
			l->setNext( r );
		}
		r->setNext( t );
		
		t = r->getPrev();
		if (l->getPrev() != r) {
			r->setPrev( l->getPrev() );	  
			r->getNext()->setPrev( l );
		} else {
			r->setPrev( l );
		}
		l->setPrev( t );

		t = l;
		l= r->getNext();
		r= t->getPrev();
		
		//std::cout << *l << std::endl;
		//std::cout << *r << std::endl;
		printLinks(l);
	   //printLR2(l);
		std::cout << std::endl;
	}

	_head = newHead;
}

// sort by merge sort
template<class T>
void DoublyLinkedList<T>::sort2()
{

	if (_size<=1)
	{	
		return;
	}		  
	_head->getPrev()->setNext(NULL);
	DoublyLinkedNode<T>* new_list=NULL;
	DoublyLinkedNode<T>* tail=NULL;
	DoublyLinkedNode<T>* left=_head;
	DoublyLinkedNode<T>* right=_head;
	int size_sub_list=1;
	int size_l, size_r;
	int merge_count=0;
	
	do
	{	
		merge_count=0;
		left=_head;
		_head = tail = NULL;

		while ( left!=NULL ) 
		{
			++merge_count;	
			size_l = size_sub_list;
			size_r = 0;
			right=left;		

			while (right!=NULL && size_r<size_l) { ++size_r; right=right->getNext(); }
			
			while ( (right!=NULL && size_r>0) || (left!=NULL && size_l>0))
			{
				if ( (right==NULL || size_r==0) && size_l>0) 
				{
					new_list=left; left=left->getNext(); --size_l;
				}
				else if ( (right!=NULL && size_r>0) && size_l==0) 
				{
					new_list=right; right=right->getNext(); --size_r;
				}
				else if ( right!=NULL && size_r>0 && size_l>0)
				{	
					if ( *left < *right )
					{
						new_list=left; left=left->getNext(); --size_l;	
					} else {
						new_list=right; right=right->getNext(); --size_r;	
					}
				}
				
				if (tail!=NULL) 
				{
					tail->setNext(new_list);
					new_list->setPrev(tail);
				}
				else 
				{
					_head=new_list;	
				}
				tail=new_list;

				//std::cout << *(new_list->getObj()) << ", ";
			}
			
			left=right;
		}
		
		size_sub_list = size_sub_list << 1;
		
		//std::cout << "end_while_left" << std::endl;
		_head->setPrev(tail);
		// tail->getNext() still reamins NULL
		//tail->setNext(_head);
		//std::cout << *this << std::endl;
		
		tail->setNext(NULL);
	}  while ( merge_count>1 );

	_head->getPrev()->setNext(_head);
}

// sort by merge sort
template<class T>
void DoublyLinkedList<T>::sort3()
{

	if (_size<=1)
	{	
		return;
	}		  
	DoublyLinkedNode<T>* new_list=NULL;
	DoublyLinkedNode<T>* left=_head;
	DoublyLinkedNode<T>* right=_head->getPrev();
	int size_sub_list=1;
	int size_l, size_r;
	int merge_count=0;
	DoublyLinkedNode<T>* head_l=NULL;
	DoublyLinkedNode<T>* head_r=NULL;
	DoublyLinkedNode<T>* tail_l=NULL;
	DoublyLinkedNode<T>* tail_r=NULL;
	int i=0;
	int val = _size % 2 == 0 ? _size/2 : (_size/2)+1; 
	std::cout << "split point on " << val << std::endl;
	while (i<val-1) { ++i; left=left->getNext(); }
	left->getNext()->setPrev(NULL);
	left->setNext(NULL);		  
	left=_head;
	printLR(left);
	std::cout << "  |  ";
	printRL(right);
	std::cout << std::endl;

	do
	{	
		merge_count=0;
		head_l = tail_l = tail_r = head_r = NULL;
		bool isleft = true;
		while ( left || right ) 
		{
			++merge_count;	
			std::cout << merge_count << std::endl;
			size_r = size_l = size_sub_list;
			
			while ( (right && size_r>0) || (left && size_l>0) )
			{
				if ( (!right || size_r==0) && (left && size_l>0)) 
				{
					new_list=left; left=left->getNext(); --size_l;
				}
				else if ( (!left || size_l==0) && (right && size_r>0)) 
				{
					new_list=right; right=right->getPrev(); --size_r;
				}
				else if ( right && left && size_r>0 && size_l>0)
				{	
					if ( *left < *right )
					{
						new_list=left; left=left->getNext(); --size_l;	
					} else {
						new_list=right; right=right->getPrev(); --size_r;	
					}
				}

				if (isleft)
				{
					if (tail_l!=NULL) 
					{
						tail_l->setNext(new_list);
						new_list->setPrev(tail_l);
					}
					else 
					{
						head_l=new_list;
						//head_l->setNext(head_l);
					}
					tail_l=new_list;
				}
				else
				{
					if (tail_r!=NULL) 
					{
						tail_r->setPrev(new_list);
						new_list->setNext(tail_r);
					}
					else 
					{
						head_r=new_list;	
						//head_r->setNext(head_r);
					}
					tail_r=new_list;
				}

				std::cout << *(new_list->getObj()) << ", ";
			} 
			
			std::cout << std::endl;
			isleft = !isleft;
		}
		
		size_sub_list = size_sub_list << 1;
		if (head_l)	{
		  	head_l->setPrev(NULL);
			tail_l->setNext(NULL);
		}
		if (head_r)	{
			tail_r->setPrev(NULL);
			head_r->setNext(NULL);
		}

		printLR(head_l);
		std::cout << "  |  ";
		printLR(tail_r);
		std::cout << std::endl;
		left=head_l;
		right=head_r;
		//std::cout << *this << std::endl;
		
	}  while ( merge_count>1 );
	
	if (head_l){
		_head = head_l;
		head_l->setPrev(tail_l);
		tail_l->setNext(head_l);
	} else if (head_r){
		// list is sorted in reverse order	  
		_head = tail_r;
		head_r->setNext(tail_r);
		tail_r->setPrev(head_r);
	}

}

template <class T>
std::ostream& DoublyLinkedNode<T>::toStream(std::ostream& oOut){
		//oOut << "DoublyLinkedNode: ";
		oOut << _obj; 
		return oOut;
	}

template <class T>
std::ostream& DoublyLinkedNode<T*>::toStream(std::ostream& oOut){
		//oOut << "DoublyLinkedNode: ";
		oOut << *_obj; 
		return oOut;
	}

template <class T>
std::ostream& DoublyLinkedList<T>::toStream(std::ostream& oOut){
		oOut << "DoublyLinkedList: ";
		oOut << std::endl;
	   // we not use the iterator so we can delegate the deref
		// of object to the DLNode
		if (_head!=NULL) {
			DoublyLinkedNode<T>* aTmpHead = _head; 
			do { 
				oOut << *aTmpHead;
				oOut << ", ";
				aTmpHead = aTmpHead->getNext();
			} while ( aTmpHead != _head); 
		}
		
		return oOut;
	}

template <class T>
std::ostream& operator<<(std::ostream& oOut, DoublyLinkedNode<T> & iObj) { return iObj.toStream(oOut); }
template <class T>
std::ostream& operator<<(std::ostream& oOut, DoublyLinkedList<T> & iObj) { return iObj.toStream(oOut); }
#endif

