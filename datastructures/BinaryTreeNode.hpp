#ifndef _BINARY_TREE_NODE_HPP
#define _BINARY_TREE_NODE_HPP

#include <iostream>

template<class T>
class BinaryTreeNode 
{
	public:
	BinaryTreeNode();
	BinaryTreeNode(T iObj);
	inline T getObj() const;
	inline void setObj(T iObj);
	inline BinaryTreeNode<T>* getLeft();
	inline void setLeft(BinaryTreeNode<T>* iNode);
	inline BinaryTreeNode<T>* getRight();
	inline void setRight(BinaryTreeNode<T>* iNode);
	inline bool operator<(const BinaryTreeNode<T>& iNode) const;
	std::ostream& toStream(std::ostream& oOut);
	
	private:
	BinaryTreeNode<T>* _left;
	BinaryTreeNode<T>* _right;
	T _obj;
};

template<class T>
class BinaryTreeNode<T*> 
{
	public:
	BinaryTreeNode();
	BinaryTreeNode(T* iObj);
	inline T* getObj() const;
	inline void setObj(T* iObj);
	inline BinaryTreeNode<T*>* getLeft();
	inline void setLeft(BinaryTreeNode<T*>* iNode);
	inline BinaryTreeNode<T*>* getRight();
	inline void setRight(BinaryTreeNode<T*>* iNode);
	inline bool operator<(const BinaryTreeNode<T*>& iNode) const;
	std::ostream& toStream(std::ostream& oOut);
	
	private:
	BinaryTreeNode<T*>* _left;
	BinaryTreeNode<T*>* _right;
	T* _obj;
};

template<class T>
BinaryTreeNode<T>::BinaryTreeNode(): _left(NULL), _right(NULL) {}

template<class T>
BinaryTreeNode<T>::BinaryTreeNode(T iObj): _obj(iObj), _left(NULL), _right(NULL) {}

template<class T>
T BinaryTreeNode<T>::getObj() const {return _obj;}

template<class T>
void BinaryTreeNode<T>::setObj(T iObj) {_obj=iObj;}

template<class T>
BinaryTreeNode<T>* BinaryTreeNode<T>::getLeft() {return _left;}

template<class T>
void BinaryTreeNode<T>::setLeft(BinaryTreeNode<T>* iNode) {_left=iNode;}

template<class T>
BinaryTreeNode<T>* BinaryTreeNode<T>::getRight() {return _right;}

template<class T>
void BinaryTreeNode<T>::setRight(BinaryTreeNode<T>* iNode) { _right=iNode;}
	
template<class T>
bool BinaryTreeNode<T>::operator<(const BinaryTreeNode<T>& iNode) const { return this->getObj() < iNode.getObj(); }

template <class T>
std::ostream& BinaryTreeNode<T>::toStream(std::ostream& oOut)
{
	oOut << _obj; 
	return oOut;
}

///////////////////////////////////////////////////////////////////////////////
template<class T>
BinaryTreeNode<T*>::BinaryTreeNode(): _obj(NULL), _left(NULL), _right(NULL) {}

template<class T>
BinaryTreeNode<T*>::BinaryTreeNode(T* iObj): _obj(iObj), _left(NULL), _right(NULL) {}

template<class T>
T* BinaryTreeNode<T*>::getObj() const {return _obj;}

template<class T>
void BinaryTreeNode<T*>::setObj(T* iObj) {_obj=iObj;}

template<class T>
BinaryTreeNode<T*>* BinaryTreeNode<T*>::getLeft() {return _left;}

template<class T>
void BinaryTreeNode<T*>::setLeft(BinaryTreeNode<T*>* iNode) {_left=iNode;}

template<class T>
BinaryTreeNode<T*>* BinaryTreeNode<T*>::getRight() {return _right;}

template<class T>
void BinaryTreeNode<T*>::setRight(BinaryTreeNode<T*>* iNode) { _right=iNode;}

template<class T>
bool BinaryTreeNode<T*>::operator<(const BinaryTreeNode<T*>& iNode) const { return *(this->getObj()) < *(iNode.getObj()); }

template <class T>
std::ostream& BinaryTreeNode<T*>::toStream(std::ostream& oOut)
{
	oOut << *_obj; //-- TODO
	return oOut;
}


///////////////////////////////////////////////////////////////////////////////
template <class T>
std::ostream& operator<<(std::ostream& oOut, BinaryTreeNode<T> & iObj) { return iObj.toStream(oOut); }


#endif
