#ifndef _NODE_HPP
#define _NODE_HPP

#include <ostream>

class Node 
{
	public:
	Node(const int iKey): _key(iKey) {}
	Node(const Node& iNode): _key(iNode.getKey()) {}
	Node& operator=(const Node& iNode) {
		_key = iNode.getKey();
		return *this;
	}
	int getKey() const {return _key;}
	std::ostream& toStream(std::ostream& oOut) const;
	bool operator==( const Node& b) const {
		return ( this->getKey()==b.getKey() );
	}
	bool operator<( const Node& b) const {
		return ( this->getKey()<b.getKey() );
	}
	private:
	int _key;
};

std::ostream& operator<<(std::ostream& oOut, const Node & iObj);

#endif

