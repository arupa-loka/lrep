#ifndef _QUEUE_HPP
#define _QUEUE_HPP

#include "DoublyLinkedList.hpp"

template<class T>
class Queue {
	public:
	inline void push(T iObj) {
		_list.pushBack(iObj);
	}
	inline void push(const DoublyLinkedList<T>& iList) {
		_list.pushBack(iList);
	}
	inline void push(T iObj, const int iTimes) {
		_list.pushBack(iObj, iTimes);
	}
	inline void pop() {
		_list.popFront();
	}
	inline T getFront() {
		return _list.getFront();
	}
	inline T getTop() {
		return _list.getFront();
	}

	inline T getBack() {
		return _list.getBack();
	}

	inline int size() const { return _list.size(); }
	inline bool empty() const { return _list.empty(); }

	private:
	DoublyLinkedList<T> _list;	
};

#endif

