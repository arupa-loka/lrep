#ifndef _VECTOR_HPP
#define _VECTOR_HPP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include "basic_op.hpp"

template<class T>
class Vector 
{
  public:
  Vector(int iSize=0);
  Vector(const Vector<T> & iV);
  const Vector<T> & operator=(const Vector<T> & iV);
  int size() const;
  bool empty() const;
  //const T& operator[](int iIdx) const;
  T& operator[](int i) const;
  void pushBack(T & iElem);
  void popBack();
  ~Vector();
  
  std::ostream& toStream(std::ostream& oOut);
  void realloc(int new_size);
  
  private:
  
  T* vector_; 
  int used_size_;
  int alloc_size_;
};

template< class T >
Vector<T>::Vector(int iSize) : vector_(NULL), used_size_(0), alloc_size_(0)
{
  this->realloc(iSize);
  used_size_ = iSize;
}

template< class T >
Vector<T>::Vector(const Vector<T> & iV) : vector_(NULL), used_size_(0), alloc_size_(0)
{
  this->realloc(iV.size());
  copy(&(iV[0]), &(iV[0])+iV.size(), vector_);
  used_size_ = iV.size();
}

template< class T >
void Vector<T>::realloc(int new_size)
{
  // TODO improve allocation process,
  // to be aligned to powers of 2??
  if (new_size > alloc_size_)
  {
    T * new_vector = new T[new_size];
    // initialize memory to zero
    memset(new_vector, 0, sizeof(T)*new_size);

    if (vector_)
    {
      copy(vector_, vector_+used_size_, new_vector);
      delete[] vector_;
      vector_ = NULL;
    }
    vector_ = new_vector;
    // TODO alloc_size_ could be different if we decide to align the real memory
    // allocated to powers of 2
    alloc_size_ = new_size;
  }
}

template< class T>
const Vector<T>& Vector<T>::operator=(const Vector<T> & iV)
{
  if (this != &iV)
  {
    // realloc if needed    
    this->realloc(iV.size());
    //copy
    copy(&(iV[0]), &(iV[0])+iV.size(), vector_);
    used_size_ = iV.size();
  }
  return *this;
}

/*
template< class T >
const T& Vector<T>::operator[](int iIdx) const
{
  return *(vector_+iIdx);
}
*/

template< class T >
T& Vector<T>::operator[](int i) const 
{
  // TODO if i>alloc_size_ trigger exception??
  return *(vector_+i);
}

template< class T >
Vector<T>::~Vector()
{
  if (vector_) 
  {
    delete[] vector_;
    vector_=NULL;
    used_size_ = alloc_size_ = 0;
    
  }
}

template< class T >
int Vector<T>::size() const
{
  return used_size_;
}

template< class T >
bool Vector<T>::empty() const
{
  return (used_size_==0);
}

template <class T>
void Vector<T>::pushBack(T & iElem)
{
  this->realloc(used_size_+1);
  // TODO bug, here if iElem already belongs to this vector, there are chances
  // that the corresponding space will be deallocated in realloc, and iElem will
  // point to something invalid.
  vector_[used_size_] = iElem;
  ++used_size_;
}

template <class T>
void Vector<T>::popBack()
{
  // Reduce vector size by one
  // but actually we don't release allocated memory
  // TODO release allocated memory by powers of 2??
  --used_size_;
}

template <class T>
std::ostream& Vector<T>::toStream(std::ostream& oOut)
{
  for (int i = 0; i< used_size_; ++i) 
  {
      oOut << (*this)[i];
      oOut << ", ";
  }
  return oOut;
}

template <class T>
std::ostream& operator<<(std::ostream& oOut, Vector<T> & iObj) 
{ 
  return iObj.toStream(oOut); 
}

#endif

