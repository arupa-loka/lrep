#ifndef _STACK_2_HPP
#define _STACK_2_HPP

#include "LinkedList.hpp"

template<class T>
class Stack2 {
	public:
	inline void push(T iObj) {
		_list.pushFront(iObj);
	}

	inline void push(T iObj, const int iTimes) {
		_list.pushFront(iObj, iTimes);
	}

	inline void pop() {
		_list.popFront();
	}
	inline T getTop() {
		return _list.getFront();
	}

	inline int size() { return _list.size(); }
	inline bool empty() { return _list.empty(); }

	std::ostream& toStream(std::ostream& oOut);
	private:
	LinkedList<T> _list;	
};

template <class T>
std::ostream& Stack2<T>::toStream(std::ostream& oOut){
	oOut << _list;
	return oOut;
}

template <class T>
std::ostream& operator<<(std::ostream& oOut, Stack2<T> & iObj) { return iObj.toStream(oOut); }

#endif

