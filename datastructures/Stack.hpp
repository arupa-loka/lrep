#ifndef _STACK_HPP
#define _STACK_HPP

#include "DoublyLinkedList.hpp"

template<class T>
class Stack {
	public:
	inline void push(T iObj) {
		_list.pushFront(iObj);
	}
	inline void push(const DoublyLinkedList<T>& iList) {
		_list.pushFront(iList);
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
	DoublyLinkedList<T> _list;	
};

#include "Stack.cpp"

#endif

