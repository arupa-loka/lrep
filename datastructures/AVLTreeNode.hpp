#ifndef _AVL_TREE_NODE_HPP
#define _AVL_TREE_NOCE_HPP

#include <iostream>

template<class T>
class AVLTreeNode
{
	public:
	AVLTreeNode();
	AVLTreeNode(T iObj);
	AVLTreeNode(T iObj, char iBalancing);

	inline T getObj() const;
	inline void setObj(T iObj);
	inline AVLTreeNode<T>* getLeft();
	inline void setLeft(AVLTreeNode<T>* iNode);
	inline AVLTreeNode<T>* getRight();
	inline void setRight(AVLTreeNode<T>* iNode);
	inline bool operator<(const AVLTreeNode<T>& iNode) const;

	inline char getBalancing() const;
	inline void setBalancing(char iBalancing);
	inline AVLTreeNode<T>* getChild(char i);
	inline void setChild(char i, AVLTreeNode<T>* iNode);
	std::ostream& toStream(std::ostream& oOut);

	private:
	char _balancing;
	AVLTreeNode<T>* _left;
	AVLTreeNode<T>* _right;
	T _obj;

};

template<class T>
class AVLTreeNode<T*>
{
	public:
	AVLTreeNode();
	AVLTreeNode(T* iObj);
	AVLTreeNode(T* iObj, char iBalancing);

	inline T* getObj() const;
	inline void setObj(T* iObj);
	inline AVLTreeNode<T*>* getLeft();
	inline void setLeft(AVLTreeNode<T*>* iNode);
	inline AVLTreeNode<T*>* getRight();
	inline void setRight(AVLTreeNode<T*>* iNode);
	inline bool operator<(const AVLTreeNode<T*>& iNode) const;

	inline char getBalancing() const;
	inline void setBalancing(char iBalancing);
	inline AVLTreeNode<T*>* getChild(char i);
	inline void setChild(char i, AVLTreeNode<T*>* iNode);
	std::ostream& toStream(std::ostream& oOut);

	private:
	char _balancing;
	AVLTreeNode<T*>* _left;
	AVLTreeNode<T*>* _right;
	T* _obj;

};

template<class T>
AVLTreeNode<T>::AVLTreeNode(): _left(NULL), _right(NULL), _balancing(0) {}

template<class T>
AVLTreeNode<T>::AVLTreeNode(T iObj): _obj(iObj), _left(NULL), _right(NULL), _balancing(0) {}

template<class T>
AVLTreeNode<T>::AVLTreeNode(T iObj, char iBalancing): _obj(iObj), _left(NULL), _right(NULL), _balancing(iBalancing) {}

template<class T>
T AVLTreeNode<T>::getObj() const {return _obj;}

template<class T>
void AVLTreeNode<T>::setObj(T iObj) {_obj=iObj;}

template<class T>
AVLTreeNode<T>* AVLTreeNode<T>::getLeft() {return _left;}

template<class T>
void AVLTreeNode<T>::setLeft(AVLTreeNode<T>* iNode) {_left=iNode;}

template<class T>
AVLTreeNode<T>* AVLTreeNode<T>::getRight() {return _right;}

template<class T>
void AVLTreeNode<T>::setRight(AVLTreeNode<T>* iNode) { _right=iNode;}
	
template<class T>
bool AVLTreeNode<T>::operator<(const AVLTreeNode<T>& iNode) const { return this->getObj() < iNode.getObj(); }

template<class T>
char AVLTreeNode<T>::getBalancing() const { return _balancing; }

template<class T>
void AVLTreeNode<T>::setBalancing(char iBalancing) { _balancing=iBalancing; }

template<class T>
AVLTreeNode<T>* AVLTreeNode<T>::getChild(char i) { return i<0 ? getLeft() : getRight(); }

template<class T>
void AVLTreeNode<T>::setChild(char i, AVLTreeNode<T>* iNode) { i<0 ? setLeft(iNode) : setRight(iNode); }

template <class T>
std::ostream& AVLTreeNode<T>::toStream(std::ostream& oOut)
{
	oOut << _obj; 
	return oOut;
}
///////////////////////////////////////////////////////////////////////////////
template<class T>
AVLTreeNode<T*>::AVLTreeNode(): _obj(NULL), _left(NULL), _right(NULL), _balancing(0) {}

template<class T>
AVLTreeNode<T*>::AVLTreeNode(T* iObj): _obj(iObj), _left(NULL), _right(NULL), _balancing(0) {}

template<class T>
AVLTreeNode<T*>::AVLTreeNode(T* iObj, char iBalancing): _obj(iObj), _left(NULL), _right(NULL), _balancing(iBalancing) {}

template<class T>
T* AVLTreeNode<T*>::getObj() const {return _obj;}

template<class T>
void AVLTreeNode<T*>::setObj(T* iObj) {_obj=iObj;}

template<class T>
AVLTreeNode<T*>* AVLTreeNode<T*>::getLeft() {return _left;}

template<class T>
void AVLTreeNode<T*>::setLeft(AVLTreeNode<T*>* iNode) {_left=iNode;}

template<class T>
AVLTreeNode<T*>* AVLTreeNode<T*>::getRight() {return _right;}

template<class T>
void AVLTreeNode<T*>::setRight(AVLTreeNode<T*>* iNode) { _right=iNode;}
	
template<class T>
bool AVLTreeNode<T*>::operator<(const AVLTreeNode<T*>& iNode) const { return *(this->getObj()) < *(iNode.getObj()); }

template<class T>
char AVLTreeNode<T*>::getBalancing() const { return _balancing;}

template<class T>
void AVLTreeNode<T*>::setBalancing(char iBalancing) { _balancing=iBalancing; }

template<class T>
AVLTreeNode<T*>* AVLTreeNode<T*>::getChild(char i) { return i<0 ? getLeft() : getRight(); }

template<class T>
void AVLTreeNode<T*>::setChild(char i, AVLTreeNode<T*>* iNode) { i<0 ? setLeft(iNode) : setRight(iNode); }

template <class T>
std::ostream& AVLTreeNode<T*>::toStream(std::ostream& oOut)
{
	oOut << *_obj; 
	return oOut;
}

///////////////////////////////////////////////////////////////////////////////
template <class T>
std::ostream& operator<<(std::ostream& oOut, AVLTreeNode<T> & iObj) { return iObj.toStream(oOut); }


#endif
