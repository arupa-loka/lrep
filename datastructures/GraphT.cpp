#ifndef _GRAPH_T_CPP
#define _GRAPH_T_CPP

#include "GraphT.hpp"
#include <iostream>

template <class T, class Weight>
std::ostream& Edge<T,Weight>::toStream(std::ostream& oOut){
	oOut << "Edge: ";
	oOut << "From: ";
	oOut << _from; //-- TODO
	oOut << ", To: ";
	oOut << _to; //-- TODO
	oOut << ", Weight: ";
	oOut << _weight; 
	oOut << std::endl;
	return oOut;
}

template <class T, class Weight>
std::ostream& Edge<T*, Weight>::toStream(std::ostream& oOut){
	oOut << "Edge: ";
	oOut << "From: ";
	oOut << *_from; //-- TODO
	oOut << ", To: ";
	oOut << *_to; //-- TODO
	oOut << ", Weight: ";
	oOut << _weight;
	oOut << std::endl;
	return oOut;
}

template <class T, class Weight>
std::ostream& GraphNode<T,Weight>::toStream(std::ostream& oOut){
	oOut << "GraphNode: ";
	oOut << _obj; //-- TODO
	oOut << _edges;
	return oOut;
}

template <class T, class Weight>
std::ostream& GraphNode<T*, Weight>::toStream(std::ostream& oOut){
	oOut << "GraphNode: ";
	oOut << *_obj; //-- TODO
	oOut << _edges;
	return oOut;
}

template <class T, class Weight>
std::ostream& GraphT<T,Weight>::toStream(std::ostream& oOut){
	oOut << "Graph: ";
	oOut << std::endl;
	
	oOut << "NodeList: ";
	oOut << _nodeList;
	oOut << std::endl;
	oOut << "EdgeList: ";
	oOut << _edgeList;
	return oOut;
}

template <class T, class Weight>
std::ostream& GraphT<T*,Weight>::toStream(std::ostream& oOut){
	oOut << "Graph: ";
	oOut << std::endl;
	
	oOut << "NodeList: ";
	oOut << _nodeList;
	oOut << std::endl;
	oOut << "EdgeList: ";
	oOut << _edgeList;
	return oOut;
}

template <class T, class Weight>
std::ostream& operator<<(std::ostream& oOut, GraphT<T,Weight> & iObj) { return iObj.toStream(oOut); }

template <class T, class Weight>
std::ostream& operator<<(std::ostream& oOut, GraphNode<T,Weight> & iObj) { return iObj.toStream(oOut); }

template <class T, class Weight>
std::ostream& operator<<(std::ostream& oOut, Edge<T,Weight> & iObj) { return iObj.toStream(oOut); }


#endif
