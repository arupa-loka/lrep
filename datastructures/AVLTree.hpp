#ifndef _AVL_TREE_HPP
#define _AVL_TREE_HPP

#include "AVLTreeNode.hpp"
#include "Stack.hpp"
#include "Queue.hpp"
#include "Pair.hpp"
#include <iostream>

template<class T> class AVLTreeInOrderIterator;

template<class T>
class AVLTree
{
	friend class AVLTreeInOrderIterator<T>;
	
	public:
	AVLTree();
	AVLTree(const AVLTree<T>& iTree);
	AVLTree<T>& operator=(const AVLTree<T>& iTree);
	~AVLTree();
	void push(T iObj);
	void pop(T iObj);
	bool search(T iObj);
	inline int size();
	void clean();
	AVLTreeInOrderIterator<T> getInorderIt(); 
	std::ostream& toStream(std::ostream& oOut);
	
	private:
	typedef Pair< AVLTreeNode<T>*, char > PairT;
	
	void getInOrderPredecessor( AVLTreeNode<T>* iNode, AVLTreeNode<T>** oPred, AVLTreeNode<T>** oPar);
	void getInOrderSuccessor( AVLTreeNode<T>* iNode, AVLTreeNode<T>** oSucc, AVLTreeNode<T>** oPar, Stack< PairT* >& iStack);
	void singleRotation( AVLTreeNode<T>* S, AVLTreeNode<T>* R, char i);
	void doubleRotation( AVLTreeNode<T>* S, AVLTreeNode<T>* R, char i);

	AVLTreeNode<T> _root;
	int _height;
	int _size;
};

template<class T>
class AVLTreeInOrderIterator
{
	public:
	AVLTreeInOrderIterator(AVLTree<T>& iTree);
	AVLTreeInOrderIterator<T>& operator++();
	T operator*();
	bool end();
	void reset();
	
	private:
	AVLTree<T>* _tree;
	AVLTreeNode<T>* _current;
	Stack< AVLTreeNode<T>* > _stack;
	bool _over;
	bool _visit;
};

///////////////////////////////////////////////////////////////////////////////

template<class T>
AVLTree<T>::AVLTree(): _root(), _size(0), _height(0) {}

template<class T>
AVLTree<T>::AVLTree(const AVLTree<T>& iTree) 
{
	// no pointer check needed
	// no deallocation needed
	// just a deep copy
}

template<class T>
AVLTree<T>& AVLTree<T>::operator=(const AVLTree<T>& iTree)
{
	if (this!=&iTree)
	{
		// deallocation
		// deep copy
	}

	return *this;
}

template<class T>
AVLTree<T>::~AVLTree()
{
	// deallocation
}

template<class T>
void AVLTree<T>::push(T iObj)
{
	AVLTreeNode<T>* aNewNode = new AVLTreeNode<T>(iObj,0);
	if ( _root.getRight()!=NULL )
	{
		// will be the parent of the last unbalanced node
		AVLTreeNode<T>* U = &_root;
		// will be the last unbalanced node
		AVLTreeNode<T>* S = _root.getRight();
		// a cursor, that will be the parent of the new inserted node
		AVLTreeNode<T>* p = _root.getRight();
		// the new inserted node
		AVLTreeNode<T>* Q = NULL;
		// auxiliary node used for re-balancing process
		AVLTreeNode<T>* R = NULL;
		// balancing param
		char i=0;
		
		do {
			if ( *aNewNode < *p ) {
				Q = p->getLeft();
			} else {
				Q = p->getRight();
			}

			if (Q) 
			{
				if (Q->getBalancing()!=0) 
				{
					U=p;
					p=S=Q;
				} 
			} else {
				break;
			}
			p=Q;
		} while (1);
		
			
		if ( *aNewNode < *p )
		{
			p->setLeft(aNewNode);
		}
		else
		{
			p->setRight(aNewNode);
		}
		
		// change balancing attributes from S to Parent of the new node
		if (*aNewNode < *S) {
			i = -1;
		} else {
			i = +1;
		}
			
		R = p = S->getChild(i);

		while (p!=aNewNode)
		{
			if (*aNewNode<*p)
			{
				p->setBalancing(-1);
				p = p->getLeft();
			}
			else //if (*p < *aNewNode)
			{
				p->setBalancing(+1);
				p = p->getRight();
			}
		}

		// do we need any re-balancing ?
		if (S->getBalancing()==0) { S->setBalancing(i); ++_height; }
		else if (S->getBalancing()==-i) { S->setBalancing(0); }
		else /*S->getBalancing()==i*/ 
		{ 
			if (R->getBalancing()==i) {
				p=R;
				singleRotation(S,R,i);
			} else {
				p= R->getChild(-i); 
				doubleRotation(S,R,i);
			}
			// set the U node
			if (S == U->getRight()) {
				U->setRight(p);
			} else {
				U->setLeft(p);
			}
		}
	}
	else
	{
		_root.setRight(aNewNode);
	}
	
	++_size;
	return;
}

template<class T>
void AVLTree<T>::pop(T iObj)
{
	if (_root.getRight()!=NULL)
	{	
		// here we should use automatic allocation (stack)
		AVLTreeNode<T>* aNewNode = new AVLTreeNode<T>(iObj);
		AVLTreeNode<T>* p = _root.getRight();
		AVLTreeNode<T>* parent = NULL;
		Stack< PairT* > stack; // stack for nodes and directions

		// find the key
		do {
			if ( *aNewNode < *p )
			{
				stack.push(new PairT(p,-1));
				parent = p;
				p = p->getLeft();
			}
			else if ( *p < *aNewNode )
			{
				stack.push(new PairT(p,+1));
				parent = p;
				p = p->getRight();
			}
			else 
			{
				// key matching
				break;
			}
			
		} while (p);

		// key has been found ?
		if (p!=NULL)
		{
			// founded not to remove is not the root
			if (parent!=NULL)
			{
				// leaf node
				if ( p->getLeft()==NULL && p->getRight()==NULL )
				{
					if ( p == parent->getLeft() ) parent->setLeft(NULL); else parent->setRight(NULL);
				}
				// inner node with two children
				else if ( p->getLeft()!=NULL && p->getRight()!=NULL )
				{
					AVLTreeNode<T>* q = NULL;
					AVLTreeNode<T>* qparent = NULL;
					getInOrderSuccessor(p, &q, &qparent, stack);
					
					if (qparent!=p) // is not the right node of the found node
					{
						qparent->setLeft(q->getRight());
						if ( p == parent->getLeft() ) parent->setLeft(q); else parent->setRight(q);
						q->setLeft(p->getLeft());
						q->setRight(p->getRight());
					}
					else
					{
						if ( p == parent->getLeft() ) parent->setLeft(q); else parent->setRight(q);
						q->setLeft(p->getLeft()); 
					}	
					
				}
				// inner node with just one left child
				else if ( p->getLeft()!=NULL )
				{
					if ( p == parent->getLeft() ) parent->setLeft(p->getLeft()); else parent->setRight(p->getLeft());
				}
				// inner node with just one right child
				else 
				{
					if ( p == parent->getLeft() ) parent->setLeft(p->getRight()); else parent->setRight(p->getRight());
				}
			}
			// remove root 
			else
			{
				AVLTreeNode<T>* q = NULL;
				AVLTreeNode<T>* qparent = NULL;
				getInOrderSuccessor(p, &q, &qparent, stack);
				
				if (qparent!=p) // is not the right node of the found node
				{
					qparent->setLeft(q->getRight());
					q->setLeft(p->getLeft());
					q->setRight(p->getRight());
					_root.setRight(q);
				}
				else
				{
					q->setLeft(p->getLeft()); 
					_root.setRight(q);
				}	
			}	
			delete p;
			p=NULL;

			//-- PERFORM REBALANCING
			while (!stack.empty())
			{
				PairT * pair = stack.getTop();
				p = pair->first();
				char a = pair->second();

				// add speciale case to handle the root
				
				if (p->getBalancing() == a) {p->setBalancing(0);}
				else if (p->getBalancing()==0) {p->setBalancing(-a); break;}
				else /*p->getBalancing()==-a*/ // NEED TO PERFORM REBALANCING
				{
					// two cases + one case 
				}
			}

		}
		// else key not found
		
		delete aNewNode;
		--_size;
	}
	
	return;
}

template<class T>
bool AVLTree<T>::search(T iObj)
{
	return false;
}

template<class T>
int AVLTree<T>::size()
{
	return _size();
}

template<class T>
void AVLTree<T>::clean()
{
	//implemented as a post order visit	  
	std::cout<< "Cleaning in post order...\n";
	AVLTreeNode<T>* current = _root.getRight();	  
	AVLTreeNode<T>* tmp = NULL;	  
	Stack< AVLTreeNode<T>* > stack;
	
	do
	{
		if(current!=NULL)
		{
			stack.push(current);
			current=current->getLeft();
		}
		else if (!stack.empty())
		{
			if ( stack.getTop()->getRight()!=0 )
			{
				current= stack.getTop()->getRight();
			}
			else 
			{		  
				current = stack.getTop();
				stack.pop();
				tmp = current;
				std::cout<< *(current->getObj()) << ", ";
				delete current;	  
				current=NULL;
				while ( !stack.empty() && 
						  ( stack.getTop()->getRight()==0 || stack.getTop()->getRight()==tmp) )
				{
					current = stack.getTop();
					stack.pop();
					tmp = current;
					std::cout<< *(current->getObj()) << ", ";
					delete current;	  
					current=NULL;
				}
			}
		}
		else
		{
			break;
		}
	} while (1);
	std::cout << std::endl;
	_root.setRight(NULL);
	_size=0;
}

template<class T>
void AVLTree<T>::getInOrderPredecessor(
		AVLTreeNode<T>* iNode, 
		AVLTreeNode<T>** oPred, 
		AVLTreeNode<T>** oPar)
{
	*oPar = iNode;
	*oPred = iNode->getLeft();
	
	while ( (*oPred)->getRight()!=NULL)
	{
		*oPar = *oPred;
		*oPred = (*oPred)->getRight();
	}
	return;
}

template<class T>
void AVLTree<T>::getInOrderSuccessor(
		AVLTreeNode<T>* iNode, 
		AVLTreeNode<T>** oSucc, 
		AVLTreeNode<T>** oPar,
		Stack< PairT* >& iStack	)
{
	*oPar = iNode;
	*oSucc = iNode->getRight(); // cannot be null
	//Queue< PairT* > aux_queue;

	
	while ((*oSucc)->getLeft()!=NULL)
	{
		*oPar = *oSucc;	
		//aux_queue.pushBack(new PairT(oPar, -1))
		*oSucc = (*oSucc)->getLeft();
	}
	/*
	aux_queue.popBack();
	iStack.push(new PairT( *oSucc, +1 ) );
	while (!aux_queue.empty())
	{
		PairT * pair = aux_queue.getFront();
		aux_queue.popFront();
		iStack.push(pair);
	}
	*/
	return;
}

template<class T>
void AVLTree<T>::singleRotation( AVLTreeNode<T>* S, AVLTreeNode<T>* R, char i)
{
	// rotation
	S->setChild(i, R->getChild(-i));
	R->setChild(-i,S);
	// set balancing
	S->setBalancing(0);
	R->setBalancing(0);
	return;
}

template<class T>
void AVLTree<T>::doubleRotation( AVLTreeNode<T>* S, AVLTreeNode<T>* R, char i)
{
	AVLTreeNode<T>* B = R->getChild(-i);
	// first rotation
	R->setChild(-i,B->getChild(i)); 
	B->setChild(i,R);
	S->setChild(i,B);
	// second rotation
	S->setChild(i, B->getChild(-i));
	B->setChild(-i, S);

	// set Balancing
	if (B->getBalancing()==i)
	{
		S->setBalancing(-i);
		R->setBalancing(0);
	} 
	else if (B->getBalancing()==0)
	{
		S->setBalancing(0);
		R->setBalancing(0);
	} 
	else /* B->getBalancing()==-i*/ 
	{
		S->setBalancing(0);
		R->setBalancing(i);
	}
	return;
}


template <class T>
AVLTreeInOrderIterator<T> AVLTree<T>::getInorderIt()
{
	AVLTreeInOrderIterator<T> i(*this);
	return i;
}

template <class T>
std::ostream& AVLTree<T>::toStream(std::ostream& oOut)
{
	oOut << "AVLTree: ";
	for ( AVLTreeInOrderIterator<T> i = getInorderIt(); !i.end(); ++i )
	{
		oOut << *i << ", ";
	}
	//oOut << std::endl;
	return oOut;
}

template <class T>
std::ostream& operator<<(std::ostream& oOut, AVLTree<T> & iObj) { return iObj.toStream(oOut); }


///////////////////////////////////////////////////////////////////////////////

template <class T>
AVLTreeInOrderIterator<T>::AVLTreeInOrderIterator(AVLTree<T>& iTree):
	_tree(&iTree) 
{
	reset();
}

template <class T>
void AVLTreeInOrderIterator<T>::reset()
{
	_visit=false;
	if ( _tree->_root.getRight()==NULL)
	{
		_over=true;
		_current=NULL;
	}
	else
	{
		_current=_tree->_root.getRight();	
		_over=false;
		++(*this);
	}
}

template <class T>
AVLTreeInOrderIterator<T>& AVLTreeInOrderIterator<T>::operator++()
{
	if (_visit)
	{
		_current=_current->getRight();
		_visit=false;
	}
	
	do
	{
		if(_current!=NULL)
		{
			_stack.push(_current);
			_current=_current->getLeft();
		}
		else if (!_stack.empty())
		{
			_current = _stack.getTop();
			_stack.pop();
			_visit=true;
			break;
		}
		else 
		{
			_over=true;
			break;
		}
	} while (1);
}

template <class T>
T AVLTreeInOrderIterator<T>::operator*()
{
	return _current->getObj();
}

template <class T>
bool AVLTreeInOrderIterator<T>::end()
{
	return _over;
}



#endif

