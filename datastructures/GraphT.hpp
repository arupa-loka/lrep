#ifndef _GRAPH_T_HPP
#define _GRAPH_T_HPP

#include <DoublyLinkedList.hpp>
#include <iostream>

template <class T, class Weight>
class Edge {
	public:
	Edge(T iFrom, T iTo, Weight iWeight) : 
	  _from(iFrom), _to(iTo), _weight(iWeight) {}
	T getFrom()  { return _from; }	  
	T getTo()  { return _to; }
	Weight getWeigth() { return _weight; }
	~Edge() {}
	std::ostream& toStream(std::ostream& oOut);
	
	private:
	T _from;
	T _to;
	Weight _weight;
};

template <class T, class Weight>
class Edge<T*,Weight> {
	public:
	Edge(T* iFrom, T* iTo, Weight iWeight) : 
	  _from(iFrom), _to(iTo), _weight(iWeight) {}
	T* getFrom() { return _from; }	  
	T* getTo() { return _to; }
	Weight getWeigth() { return _weight; }
	~Edge() {}
	std::ostream& toStream(std::ostream& oOut);
	
	private:
	T* _from;
	T* _to;
	Weight _weight;
};


template <class T, class Weight>
class GraphNode {
	public:
	GraphNode( T iObj ) : _obj(iObj) {}
	T getObj()  { return _obj; }
	DoublyLinkedList<Edge<T,Weight>*>& getEdges() { return _edges; }
	~GraphNode() {}
	std::ostream& toStream(std::ostream& oOut);
	
	private:
	T _obj;
	DoublyLinkedList<Edge<T,Weight>*> _edges;
};

template <class T, class Weight>
class GraphNode<T*, Weight> {
	public:
	GraphNode( T* iObj ) : _obj(iObj) {}
	T* getObj() { return _obj; }
	DoublyLinkedList<Edge<T*,Weight>*>& getEdges() { return _edges; }
	~GraphNode() {}
	std::ostream& toStream(std::ostream& oOut);
	
	private:
	T* _obj;
	DoublyLinkedList<Edge<T*,Weight>*> _edges;
};


template <class T, class Weight>
class GraphT {
	public:
	typedef DoublyLinkedList<GraphNode<T,Weight>*> ListNodes;
	typedef DoublyLinkedList<Edge<T,Weight>*> ListEdges;
	
	GraphT();
	void addNode(T iNode);
	void addEdge(T iFrom, T iTo, Weight iWeight);
	ListEdges* getEdges(T iObj);
	~GraphT();	
	std::ostream& toStream(std::ostream& oOut);
	
	private:
	DoublyLinkedList<GraphNode<T,Weight>*> _nodeList;
	DoublyLinkedList<Edge<T,Weight>*> _edgeList;

};

template <class T, class Weight>
class GraphT<T*,Weight> {
	public:
	typedef DoublyLinkedList<GraphNode<T*,Weight>*> ListNodes;
	typedef DoublyLinkedList<Edge<T*,Weight>*> ListEdges;
	
	GraphT();
	void addNode(T* iNode);
	void addEdge(T* iFrom, T* iTo, Weight iWeight);
	ListEdges* getEdges( T* iObj);
	~GraphT();	
	std::ostream& toStream(std::ostream& oOut);
	
	private:
	DoublyLinkedList<GraphNode<T*,Weight>*> _nodeList;
	DoublyLinkedList<Edge<T*,Weight>*> _edgeList;

};


template <class T, class Weight>
GraphT<T,Weight>::GraphT() {}

template <class T, class Weight>
GraphT<T*,Weight>::GraphT() {}

template <class T, class Weight>
GraphT<T,Weight>::~GraphT() {}

template <class T, class Weight>
GraphT<T*,Weight>::~GraphT() {}

template <class T, class Weight>
void GraphT<T,Weight>::addNode(T iNode) 
{
	GraphNode<T,Weight>* aGn = new GraphNode<T,Weight>(iNode);
	_nodeList.pushBack(aGn);
	//std::cout << "New node "<< iNode << " added" << std::endl;
}

template <class T, class Weight>
void GraphT<T*,Weight>::addNode(T* iNode) 
{
	GraphNode<T*,Weight>* aGn = new GraphNode<T*,Weight>(iNode);
	_nodeList.pushBack(aGn);
	//std::cout << "New node "<< iNode << " added" << std::endl;
}

template <class T, class Weight>
void GraphT<T,Weight>::addEdge(T iFrom, T iTo, Weight iWeight) 
{
	// find the right head node
	//std::cout << "Link "<< iFrom << " to " << iTo << std::endl;
	for (DoublyLinkedListIterator<GraphNode<T,Weight>*> aIt1 = _nodeList.getIterator(); !aIt1.end(); ++aIt1) 
	{
		GraphNode<T, Weight>* aGn = *aIt1; 
		
		//-- if T is a pointer we compare just the pointers values
		//-- add some check exc for NULL pointers
		if ( aGn->getObj() == iFrom ) 
		{
			//std::cout << "Parent found "<< iFrom << std::endl;
			Edge<T,Weight> * aNewEdge = new Edge<T,Weight>(iFrom,iTo,iWeight);
			DoublyLinkedList< Edge<T,Weight>* >& aEdgeList = aGn->getEdges();
			aEdgeList.pushBack(aNewEdge);
			_edgeList.pushBack(aNewEdge);
			return;
		}
	}
	//std::cout << "Parent NOT found "<< iFrom << std::endl;
	return;
}

template <class T, class Weight>
void GraphT<T*,Weight>::addEdge(T* iFrom, T* iTo, Weight iWeight) 
{
	// find the right head node
	//std::cout << "Link "<< iFrom << " to " << iTo << std::endl;
	for (DoublyLinkedListIterator<GraphNode<T*,Weight>*> aIt1 = _nodeList.getIterator(); !aIt1.end(); ++aIt1) 
	{
		GraphNode<T*, Weight>* aGn = *aIt1; 
		
		//-- if T is a pointer we compare just the pointers values
		//-- add some check exc for NULL pointers
		if ( *(aGn->getObj()) == *iFrom ) 
		{
			//std::cout << "Parent found "<< iFrom << std::endl;
			Edge<T*,Weight> * aNewEdge = new Edge<T*,Weight>(iFrom,iTo,iWeight);
			DoublyLinkedList< Edge<T*,Weight>* >& aEdgeList = aGn->getEdges();
			aEdgeList.pushBack(aNewEdge);
			_edgeList.pushBack(aNewEdge);
			return;
		}
	}
	//std::cout << "Parent NOT found "<< iFrom << std::endl;
	return;
}

template <class T, class Weight>
DoublyLinkedList<Edge<T,Weight>*>* GraphT<T,Weight>::getEdges( T iObj)  
{
	for (DoublyLinkedListIterator<GraphNode<T,Weight>*> aIt1 = _nodeList.getIterator(); !aIt1.end(); ++aIt1) 
	{
		GraphNode<T, Weight> * aGn = *aIt1; 
		if (aGn->getObj() == iObj ) 
		{
			return &(aGn->getEdges());
		}
	}
	//-- nothing found
	std::cout << "Nothing found" << std::endl;
	return NULL;
}

template <class T, class Weight>
DoublyLinkedList<Edge<T*,Weight>*>* GraphT<T*,Weight>::getEdges( T* iObj)  
{
	for (DoublyLinkedListIterator<GraphNode<T*,Weight>*> aIt1 = _nodeList.getIterator(); !aIt1.end(); ++aIt1) 
	{
		GraphNode<T*, Weight> * aGn = *aIt1; 
		if ( *(aGn->getObj()) == *iObj ) 
		{
			return &(aGn->getEdges());
		}
	}
	//-- nothing found
	std::cout << "Nothing found" << std::endl;
	return NULL;
}

#include "GraphT.cpp"

#endif

