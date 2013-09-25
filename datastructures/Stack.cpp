#ifndef _STACK_CPP
#define _STACK_CPP

#include "Stack.hpp"

template <class T>
std::ostream& Stack<T>::toStream(std::ostream& oOut){
		oOut << "Stack:";
		oOut << std::endl;
		oOut << _list;
		return oOut;
	}

template <class T>
std::ostream& operator<<(std::ostream& oOut, Stack<T> & iObj) { return iObj.toStream(oOut); }

#endif


