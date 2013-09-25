#ifndef _LINKED_LIST_HPP
#define _LINKED_LIST_HPP

#include <iostream>
#include <stdlib.h>

template<class T>
class LinkedNode {
	
	public:
	LinkedNode();
	LinkedNode(T iObj);
	inline T getObj() const;
	inline void setObj(T iObj);
	inline LinkedNode<T>* getNext();
	inline void setNext(LinkedNode<T>* iNode);
	inline bool operator<(LinkedNode<T> & iNode);
	std::ostream& toStream(std::ostream& oOut);
	private:
	LinkedNode<T>* _next;
	T _obj;

};

template<class T>
class LinkedNode<T*> {
	
	public:
	LinkedNode();
	LinkedNode(T* iObj);
	inline T* getObj() const;
	inline void setObj(T* iObj);
	inline LinkedNode<T*>* getNext();
	inline void setNext(LinkedNode<T*>* iNode);
	inline bool operator<(LinkedNode<T*> & iNode);
	std::ostream& toStream(std::ostream& oOut);

	private:
	LinkedNode<T*>* _next;
	T* _obj;

};

template<class T> class LinkedListIterator;

template<class T>
class LinkedList {

	friend class LinkedListIterator<T>;

	public:
	LinkedList();
	LinkedList(const LinkedList& iList);
	LinkedList& operator=(const LinkedList& iList);
	~LinkedList();
	void pushFront(T iObj);
	void pushBack(T iObj);
	void popBack();
	void popFront();
	T getFront();
	LinkedListIterator<T> getIterator();
	void sort(); // insertion sort
	void sort2(); // merge sort
	LinkedListIterator<T> find(T & iObj); // sequential search
	LinkedListIterator<T> find2(T & iObj); // binary search
	std::ostream& toStream(std::ostream& oOut);
	bool empty();
	int size();

	// Just for exercise
	LinkedNode<T>* getRandomHead();
	LinkedNode<T>* getHead();
	LinkedNode<T>* getBack();
	
	private:
	LinkedNode<T>* _head;
	LinkedNode<T>* _back;
	int _size;
	
};

template<class T>
class LinkedListIterator {
	
	public:
	LinkedListIterator(LinkedList<T>& iList);
	LinkedListIterator& operator++();
	T operator*();
	bool end();
	void reset();
	
	private:
	LinkedList<T>& _list;
	LinkedNode<T>* _current;
	bool _over;
	
};

///////////////////////////////////////////////////////////////////////////////
template<class T>
LinkedNode<T>::LinkedNode(): _obj(NULL), _next(NULL) {}

template<class T>
LinkedNode<T>::LinkedNode(T iObj): _obj(iObj), _next(NULL) {}

template<class T>
T LinkedNode<T>::getObj() const
{
	return _obj;
}

template<class T>
void LinkedNode<T>::setObj(T iObj)
{
	_obj = iObj;
}

template<class T>
LinkedNode<T>* LinkedNode<T>::getNext()
{
	return _next;
}

template<class T>
void LinkedNode<T>::setNext( LinkedNode<T>* iNode)
{
	_next = iNode;
}

template<class T>
bool LinkedNode<T>::operator<(LinkedNode<T>& iNode)
{
	return ( (this->getObj()) < (iNode.getObj()) );
}

///////////////////////////////////////////////////////////////////////////////
template<class T>
LinkedNode<T*>::LinkedNode(): _obj(NULL), _next(NULL) {}

template<class T>
LinkedNode<T*>::LinkedNode(T* iObj): _obj(iObj), _next(NULL) {}

template<class T>
T* LinkedNode<T*>::getObj() const
{
	return _obj;
}

template<class T>
void LinkedNode<T*>::setObj(T* iObj)
{
	_obj = iObj;
}

template<class T>
LinkedNode<T*>* LinkedNode<T*>::getNext()
{
	return _next;
}

template<class T>
void LinkedNode<T*>::setNext( LinkedNode<T*>* iNode)
{
	_next = iNode;
}

template<class T>
bool LinkedNode<T*>::operator<(LinkedNode<T*>& iNode)
{
	return ( *(this->getObj()) < *(iNode.getObj()) );
}
///////////////////////////////////////////////////////////////////////////////
template<class T>
LinkedList<T>::LinkedList(): _head(NULL), _back(NULL), _size(0) {}

template<class T>
LinkedList<T>::LinkedList(const LinkedList& iList): LinkedList()
{
	// deallocation is not needed 
	// cause the object is on going to be initialized
	// make just a deep copy
}

template<class T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList& iList)
{
	if (this!=&iList)
	{
		// deallocation
		// deep copy
	}
	return *this;
}

template<class T>
LinkedList<T>::~LinkedList()
{
	// every time we need a copy constructor
	// cause shallow copy is not enough
	// we have to define also a destructor
	while (_size>0)
	{
		popFront();
	}
}

template<class T>
void LinkedList<T>::pushFront(T iObj)
{
	if (_head!=NULL)
	{
		LinkedNode<T>* aNewHead = new LinkedNode<T>(iObj);
		aNewHead->setNext(_head);
		_back->setNext(aNewHead);
		_head=aNewHead;
	}
	else
	{
		_head = new LinkedNode<T>(iObj);
		_head->setNext(_head);
		_back = _head;
	}
	++_size;
}

template<class T>
void LinkedList<T>::pushBack(T iObj)
{
	pushFront(iObj);
	_head = _head->getNext();
	_back = _back->getNext();
}

template<class T>
void LinkedList<T>::popBack()
{
	if (_head!=NULL)
	{	
		if (_size>1)
		{
			LinkedNode<T>* cursor = _head;
			for(int i=1; i<_size-1; ++i)
			{
				cursor = cursor->getNext();
			}
			cursor->setNext(_head);
			delete _back;
			_back = cursor;
		}
		else
		{
			delete _back;
			_back=NULL;
			_head=NULL;
		}
		--_size;
	}
}

template<class T>
void LinkedList<T>::popFront()
{
	if (_head!=NULL)
	{	
		if (_size>1)
		{
			LinkedNode<T>* aNodeToDelete = _head;
			_head = _head->getNext();
			_back->setNext(_head);
			delete aNodeToDelete;
		}
		else
		{
			delete _back;
			_back=NULL;
			_head=NULL;
		}
		--_size;
	}
}

template<class T>
T LinkedList<T>::getFront()
{
	// call iterator copy constructor
	return _head->getObj();
}

template<class T>
LinkedListIterator<T> LinkedList<T>::getIterator()
{
	LinkedListIterator<T> aIt(*this);
	// call iterator copy constructor
	return aIt;
}

// sort by merge sort
template<class T>
void LinkedList<T>::sort2()
{

if (_size>1)
{	
	_back->setNext(NULL);
	LinkedNode<T>* new_list=NULL;
	LinkedNode<T>* tail=NULL;
	LinkedNode<T>* left=_head;
	LinkedNode<T>* right=_head;
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
		_back=tail;
		//tail->setNext(_head);
		//std::cout << *this << std::endl;
		
		tail->setNext(NULL);
	}  while ( merge_count>1 );

	_back->setNext(_head);
}	
	
}


// sort by insertion sort
template<class T>
void LinkedList<T>::sort()
{
	if (_size>1)
	{
		LinkedNode<T>* pi_back = _head;
		LinkedNode<T>* pi = _head->getNext();
		for(int i=1; i<_size; ++i)
		{	
			//std::cout << "before head " << _head->getObj()->getKey() << " back " << _back->getObj()->getKey() << std::endl;
			
		   //std::cout << *this << std::endl;
			LinkedNode<T>* pj_back = _back;
			LinkedNode<T>* pj = _head;
			bool inserted = false;

			for(int j=0; j<i; ++j)
			{	
				//std::cout << "loop_j " << j << std::endl;
				if ( (*pi) < (*pj) )
				{	
					//std::cout << "insert " << pi->getObj()->getKey() << " before " << pj->getObj()->getKey() << std::endl;
					LinkedNode<T>* tmp_pi = pi;
					if ( ! (pi==_back && pj==_head) )
					{
						inserted=true;
						// detach pi
						pi_back->setNext(pi->getNext());
						pi = pi->getNext();
						// put pi before pj
						pj_back->setNext(tmp_pi);
						tmp_pi->setNext(pj);

						if (j==0)
						{
							//std::cout << "change head to " << tmp_pi->getObj()->getKey() << std::endl;
							_head = tmp_pi;
						}
						else if (i==_size-1)
						{
							//std::cout << "change back to " << pi_back->getObj()->getKey() << std::endl;
							_back = pi_back;
						}
					}
					else
					{
						_head = pi;
						_back = pi_back;
					}

					break;
				}
				pj_back = pj;
				pj = pj->getNext();
			}

			if (!inserted)
			{
				pi_back = pi;
				pi = pi->getNext();
			}	

			//std::cout << "after head " << _head->getObj()->getKey() << " back " << _back->getObj()->getKey() << std::endl;
			
		   //std::cout << *this << std::endl;
		}
	}
}

// we suppose that the list is sorted
template<class T>
LinkedListIterator<T> LinkedList<T>::find(T & iObj)
{
		  // sequential search
}

// we suppose that the list is sorted
template<class T>
LinkedListIterator<T> LinkedList<T>::find2(T & iObj)
{
		  // binary search
}

template<class T>
bool LinkedList<T>::empty()
{
	return (_head==NULL);
}

template<class T>
int LinkedList<T>::size()
{
	return _size;
}

template<class T>
LinkedNode<T>* LinkedList<T>::getRandomHead()
{
	if (!_head)
		return NULL;
	
	srand(time(NULL));
	int index = rand() % size();
	LinkedNode<T>* aRandomHead = _head;
	while(index) {
		aRandomHead = aRandomHead->getNext();
		--index;
	}
	return aRandomHead;
}

template<class T>
LinkedNode<T>* LinkedList<T>::getHead()
{
	return _head;
}

template<class T>
LinkedNode<T>* LinkedList<T>::getBack()
{
	return _back;
}
///////////////////////////////////////////////////////////////////////////////

template<class T>
LinkedListIterator<T>::LinkedListIterator(LinkedList<T>& iList): _list(iList), _over(false)
{
	_current = _list._head;
}

template<class T>
LinkedListIterator<T>& LinkedListIterator<T>::operator++()
{
	_current = _current->getNext();
	if (_current == _list._head)
		_over=true;
	return *this;
}

template<class T>
T LinkedListIterator<T>::operator*()
{
	return _current->getObj();
}

template<class T>
bool LinkedListIterator<T>::end()
{
	return (_current==NULL | (_current==_list._head && _over) );
}

template<class T>
void LinkedListIterator<T>::reset()
{
	_current=_list._head;
	_over=false;
}

template <class T>
std::ostream& LinkedNode<T>::toStream(std::ostream& oOut){
		oOut << _obj; 
		return oOut;
}

template <class T>
std::ostream& LinkedNode<T*>::toStream(std::ostream& oOut){
		oOut << *_obj; 
		return oOut;
}

template <class T>
std::ostream& LinkedList<T>::toStream(std::ostream& oOut){
		//oOut << "LinkedList: ";
		oOut << std::endl;
	   // we not use the iterator so we can delegate the deref
		// of object to the DLNode
		if (_head!=NULL) {
			LinkedNode<T>* aTmpHead = _head; 
			do { 
				oOut << *aTmpHead;
				oOut << ", ";
				aTmpHead = aTmpHead->getNext();
			} while ( aTmpHead != _head); 
		}
		
		return oOut;
	}

template <class T>
std::ostream& operator<<(std::ostream& oOut, LinkedNode<T> & iObj) { return iObj.toStream(oOut); }
template <class T>
std::ostream& operator<<(std::ostream& oOut, LinkedList<T> & iObj) { return iObj.toStream(oOut); }


#endif

