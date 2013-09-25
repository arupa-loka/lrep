#include "Node.hpp"

std::ostream& Node::toStream(std::ostream& oOut) const{
		//oOut << "Node: _key=";
		oOut << _key;
		//oOut << ", ";
		//oOut << std::endl;
		return oOut;
	}

//namespace {
std::ostream& operator<<(std::ostream& oOut, const Node & iObj) { return iObj.toStream(oOut); };
//}

