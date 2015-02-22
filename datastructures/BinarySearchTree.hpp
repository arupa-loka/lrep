#ifndef _BINARY_SEARCH_TREE_HPP
#define _BINARY_SEARCH_TREE_HPP

#include "BinaryTreeNode.hpp"
#include "Stack.hpp"
#include <iostream>
#include <fstream>

template<class T> class BinarySearchTreeInOrderIterator;

template<class T>
class BinarySearchTree
{
  friend class BinarySearchTreeInOrderIterator<T>;

  public:
  BinarySearchTree();
  BinarySearchTree(const BinarySearchTree<T>& iTree);
  BinarySearchTree<T>& operator=(const BinarySearchTree<T>& iTree);
  ~BinarySearchTree();
  void push(T iObj);
  void pop(T iObj);
  bool search(T iObj);
  inline int size();
  void clean();
  BinarySearchTreeInOrderIterator<T> getInorderIt();
  std::ostream& toStream(std::ostream& oOut);
  void toGraphViz(char * iFile);
  void PreOrder();
  void InOrder();
  void InOrder2();
  void PostOrder();
  void PostOrder2();

  private:
  BinaryTreeNode<T> * predecessor(BinaryTreeNode<T> * iNode);
  void getInOrderPredecessor( BinaryTreeNode<T>* iNode, BinaryTreeNode<T>** oPred, BinaryTreeNode<T>** oPar);
  void getInOrderSuccessor( BinaryTreeNode<T>* iNode, BinaryTreeNode<T>** oSucc, BinaryTreeNode<T>** oPar);
  BinaryTreeNode<T>* _root;
  int _size;
};

template<class T>
class BinarySearchTreeInOrderIterator
{
  public:
  BinarySearchTreeInOrderIterator(BinarySearchTree<T>& iTree);
  BinarySearchTreeInOrderIterator<T>& operator++();
  T operator*();
  bool end();
  void reset();

  private:
  BinarySearchTree<T>* _tree;
  BinaryTreeNode<T>* _current;
  Stack< BinaryTreeNode<T>* > _stack;
  bool _over;
  bool _visit;
};


///////////////////////////////////////////////////////////////////////////////


template<class T>
void BinarySearchTree<T>::PreOrder()
{
  BinaryTreeNode<T>* p = _root;
  Stack<BinaryTreeNode<T> *> s;

  while(1) {
    while(p) {
      std::cout << *p << ", ";
      if (p->getRight())
        s.push(p->getRight());
      p=p->getLeft();
    }

    if (!s.empty()) {
      p = s.getTop(); s.pop();
    } else {
      break;
    }
  }
  std::cout << std::endl;
}

template<class T>
void BinarySearchTree<T>::InOrder()
{
  BinaryTreeNode<T>* p = _root;
  Stack<BinaryTreeNode<T> *> s;

  while(1) {
    while(p) {
      s.push(p);
      p=p->getLeft();
    }

    if (!s.empty()) {
      p = s.getTop(); s.pop();
      std::cout << *p << ", ";
      p = p->getRight();
    } else {
      break;
    }
  }
  std::cout << std::endl;
}

/* This implementation of InOrder doesn't use any recursion or stack,
it only relies on the right NULL pointers available in the tree leaves.
*/
template<class T>
void BinarySearchTree<T>::InOrder2()
{
  BinaryTreeNode<T>* p = _root;

  while(p != NULL) {
    BinaryTreeNode<T> * pred = predecessor(p);
    if (pred != NULL) {
      pred->setRight(p);
      p = p->getLeft();
    } else {
      // leaf node or node with no left subtree
      while (p == predecessor(p->getRight())) {
        std::cout << *p << ", ";
        BinaryTreeNode<T> * tmp = p;
        p = tmp->getRight();
        tmp->setRight(NULL);
      }
      std::cout << *p << ", ";
      p = p->getRight();
    }
  }
  std::cout << std::endl;
}

/* Retrieve the predecessor of a node in the tree. This is to work with
InOrder2 then it also checks for loops, in case we are looking for the
predecessor of a node for the second time (in that case the right
pointer of the predecessor already points to the node from where the
search started).
*/
template<class T>
BinaryTreeNode<T> * BinarySearchTree<T>::predecessor(BinaryTreeNode<T>* p) {
  if (p==NULL || p->getLeft() == NULL)
    return NULL;
  BinaryTreeNode<T> * r = p->getLeft();
  while (r->getRight() != NULL && r->getRight() != p) {
    r = r->getRight();
  }
  return r;
}

template<class T>
void BinarySearchTree<T>::PostOrder()
{
  BinaryTreeNode<T>* p = _root;
  Stack<BinaryTreeNode<T> *> s;

  while(1) {
    while(p) {
      s.push(p);
      p=p->getLeft();
    }

    if (!s.empty()) {
      p = s.getTop();
      if (p->getRight()){
        p=p->getRight();
      } else {
        s.pop();
        std::cout << *p << ", ";
        while (!s.empty() && (s.getTop()->getRight()==NULL || s.getTop()->getRight()==p) ) {
          p = s.getTop();
          s.pop();
          std::cout << *p << ", ";
        }
        p=0;
      }
    } else {
      break;
    }
  }
  std::cout << std::endl;

}

template<class T>
void BinarySearchTree<T>::PostOrder2()
{
  BinaryTreeNode<T>* p = _root;
  Stack<BinaryTreeNode<T> *> s;
  BinaryTreeNode<T>* last_visited = NULL;

  while(1) {
    while(p) {
      s.push(p);
      p=p->getLeft();
    }

    if (!s.empty()) {
      p = s.getTop(); s.pop();
      if (p->getRight() &&
          last_visited != p->getRight()){
        s.push(p);
        p=p->getRight();
      } else {
        std::cout << *p << ", ";
        last_visited = p;
        p = NULL;
      }
    } else {
      break;
    }
  }
  std::cout << std::endl;
}

template<class T>
void BinarySearchTree<T>::toGraphViz(char * iFilePath)
{
  std::ofstream out(iFilePath);
  if (out.is_open())
  {
    out << "digraph BinarySearchTree {\n";
    out << "node [ fixedsize=false];\n";
    out << "{ rankdir=RL; }\n";

    BinaryTreeNode<T>* p = _root;
    Stack<BinaryTreeNode<T> *> s;
    while(p || !s.empty() ) {
      while(p) {
        s.push(p);
        p=p->getLeft();
      }

      if (!s.empty()) {
        p = s.getTop(); s.pop();
        out << "\"node" << p << "\"";
        out << " [\n";
        out << " label = \"" << "<f0> | " << "<f1> " << *p << " | <f2>" << "\"";
        out << " shape = \"record\"";
        out << "];\n";

        p = p->getRight();
      }
    }

    p = _root;
    int count = 0;
    while(p || !s.empty() ) {
      while(p) {
        s.push(p);
        p=p->getLeft();
      }

      if (!s.empty()) {
        p = s.getTop(); s.pop();
        if (p->getLeft()) {
          out << "\"node" << p << "\":f0 -> ";
          out << "\"node" << p->getLeft() << "\":f1 ";
          // I don't know if the id is mandatory in dot.
          out << "[ id = " << count++ << " ];\n";
        }
        if (p->getRight()) {
          out << "\"node" << p << "\":f2 -> ";
          out << "\"node" << p->getRight() << "\":f1 ";
          out << "[ id = " << count++ << " ];\n";
        }
        p = p->getRight();
      }
    }

    out << "}\n";
    out.close();
  }
  else
  {
    printf("Error: unable to open file: %s\n", iFilePath);
  }

}


template<class T>
BinarySearchTree<T>::BinarySearchTree(): _root(NULL), _size(0) {}

template<class T>
BinarySearchTree<T>::BinarySearchTree(const BinarySearchTree<T>& iTree)
{
  // no pointer check needed
  // no deallocation needed
  // just a deep copy
}

template<class T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(const BinarySearchTree<T>& iTree)
{
  if (this!=&iTree)
  {
    // deallocation
    // deep copy
  }

  return *this;
}

template<class T>
BinarySearchTree<T>::~BinarySearchTree()
{
  // deallocation
}

template<class T>
void BinarySearchTree<T>::push(T iObj)
{
  BinaryTreeNode<T>* aNewNode = new BinaryTreeNode<T>(iObj);
  if (_root!=NULL)
  {
    BinaryTreeNode<T>* p = _root;
    BinaryTreeNode<T>* parent = NULL;
    do {
      parent = p;
      if ( *aNewNode < *p )
      {
        p = p->getLeft();
      }
      else
      {
        p = p->getRight();
      }
    } while (p);

    if ( *aNewNode < *parent )
    {
      parent->setLeft(aNewNode);
    }
    else
    {
      parent->setRight(aNewNode);
    }

  }
  else
  {
    _root=aNewNode;
  }

  ++_size;
  return;
}

template<class T>
void BinarySearchTree<T>::pop(T iObj)
{
  if (_root!=NULL)
  {
    BinaryTreeNode<T>* aNewNode = new BinaryTreeNode<T>(iObj);
    BinaryTreeNode<T>* p = _root;
    BinaryTreeNode<T>* parent = NULL;

    do {
      if ( *aNewNode < *p )
      {
        parent = p;
        p = p->getLeft();
      }
      else if ( *p < *aNewNode )
      {
        parent = p;
        p = p->getRight();
      }
      else
      {
        // key matching
        break;
      }

    } while (p);

    if (p!=NULL)
    {
      // parent is not root
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
          BinaryTreeNode<T>* q = NULL;
          BinaryTreeNode<T>* qparent = NULL;
          getInOrderSuccessor(p, &q, &qparent);

          //if (q->getRight()==NULL) // is a leaf node
          //{
          //  if (qparent!=p) // is not the right node of the found node
          //  {
          //    qparent->setLeft(NULL);
          //    if ( p == parent->getLeft() ) parent->setLeft(q); else parent->setRight(q);
          //    q->setLeft(p->getLeft());
          //    q->setRight(p->getRight());
          //  }
          //  else
          //  {
          //    if ( p == parent->getLeft() ) parent->setLeft(q); else parent->setRight(q);
          //    q->setLeft(p->getLeft());
          //  }
          //}
          //else
          //{
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
          //}

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
      // remove root, inner node with two children
      else
      {
        BinaryTreeNode<T>* q = NULL;
        BinaryTreeNode<T>* qparent = NULL;
        getInOrderSuccessor(p, &q, &qparent);

        if (q->getRight()==NULL) // is a leaf node
        {
          if (qparent!=p) // is not the right node of the found node
          {
            qparent->setLeft(NULL);
            q->setLeft(p->getLeft());
            q->setRight(p->getRight());
            _root=q;
          }
          else
          {
            q->setLeft(p->getLeft());
            _root=q;
          }
        }
        else
        {
          if (qparent!=p) // is not the right node of the found node
          {
            qparent->setLeft(q->getRight());
            q->setLeft(p->getLeft());
            q->setRight(p->getRight());
            _root=q;
          }
          else
          {
            q->setLeft(p->getLeft());
            _root=q;
          }
        }
      }
      delete p;
      p=NULL;
    }
    // else key not found

    delete aNewNode;
    --_size;
  }

  return;
}

template<class T>
bool BinarySearchTree<T>::search(T iObj)
{
  return false;
}

template<class T>
int BinarySearchTree<T>::size()
{
  return _size();
}

template<class T>
void BinarySearchTree<T>::clean()
{
  //implemented as a post order visit
  std::cout<< "Cleaning in post order...\n";
  BinaryTreeNode<T>* current = _root;
  BinaryTreeNode<T>* tmp = NULL;
  Stack< BinaryTreeNode<T>* > stack;

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
  _root=NULL;
  _size=0;
}

template<class T>
void BinarySearchTree<T>::getInOrderPredecessor(
    BinaryTreeNode<T>* iNode,
    BinaryTreeNode<T>** oPred,
    BinaryTreeNode<T>** oPar)
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
void BinarySearchTree<T>::getInOrderSuccessor(
    BinaryTreeNode<T>* iNode,
    BinaryTreeNode<T>** oSucc,
    BinaryTreeNode<T>** oPar)
{
  *oPar = iNode;
  *oSucc = iNode->getRight();

  while ((*oSucc)->getLeft()!=NULL)
  {
    *oPar = *oSucc;
    *oSucc = (*oSucc)->getLeft();
  }
  return;
}


template <class T>
BinarySearchTreeInOrderIterator<T> BinarySearchTree<T>::getInorderIt()
{
  BinarySearchTreeInOrderIterator<T> i(*this);
  return i;
}

template <class T>
std::ostream& BinarySearchTree<T>::toStream(std::ostream& oOut)
{
  oOut << "BinarySearchTree: ";
  for ( BinarySearchTreeInOrderIterator<T> i = getInorderIt(); !i.end(); ++i )
  {
    oOut << *i << ", ";
  }
  //oOut << std::endl;
  return oOut;
}

template <class T>
std::ostream& operator<<(std::ostream& oOut, BinarySearchTree<T> & iObj) { return iObj.toStream(oOut); }


///////////////////////////////////////////////////////////////////////////////

template <class T>
BinarySearchTreeInOrderIterator<T>::BinarySearchTreeInOrderIterator(BinarySearchTree<T>& iTree):
  _tree(&iTree)
{
  reset();
}

template <class T>
void BinarySearchTreeInOrderIterator<T>::reset()
{
  _visit=false;
  if ( _tree->_root==NULL)
  {
    _over=true;
    _current=NULL;
  }
  else
  {
    _current=_tree->_root;
    _over=false;
    ++(*this);
  }
}

template <class T>
BinarySearchTreeInOrderIterator<T>& BinarySearchTreeInOrderIterator<T>::operator++()
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
T BinarySearchTreeInOrderIterator<T>::operator*()
{
  return _current->getObj();
}

template <class T>
bool BinarySearchTreeInOrderIterator<T>::end()
{
  return _over;
}



#endif

