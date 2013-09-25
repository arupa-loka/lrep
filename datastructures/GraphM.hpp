#ifndef _GRAPH_M_HPP
#define _GRAPH_M_HPP

#include <ostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class GraphVertexIter;
class GraphEdgeIter;

class GraphM
{
	friend class GraphVertexIter;
	friend class GraphEdgeIter;
	
	public:
	GraphM(int iSize, bool digraph=false);
	bool addEdge(int a , int b, int w=1);
	int getVertexSize();
	int getEdgeSize();
	~GraphM();
	std::ostream & toString( std::ostream& oOut );
	GraphVertexIter getVertexIter();
	GraphEdgeIter getEdgeIter(int V);
	bool isDigraph();
	
	
	private:
	int **_matrix;
	int _vertex_size;
	int _edge_size;
	bool _digraph;
};

GraphM::GraphM(int iSize, bool digraph): _matrix(NULL), _vertex_size(0), _edge_size(0), _digraph(digraph)
{
	_matrix = (int**)malloc(sizeof(int*)*iSize);
	for(int i=0; i<iSize; ++i) {
		_matrix[i] = (int*)malloc(sizeof(int)*iSize);
		memset(_matrix[i], -1, sizeof(int)*iSize);
	}	
	_vertex_size=iSize;	
}

bool GraphM::isDigraph()
{
	return _digraph;
}

bool GraphM::addEdge(int a, int b, int w)
{
	if (a<0 || a>=_vertex_size || b<0 || b>=_vertex_size)
		return false;
	if (_matrix[a][b] > -1)
		return false;
	if (!_digraph && _matrix[b][a] > -1)
		return false;	  
	
	_matrix[a][b]=w; 
	if (!_digraph) {
		_matrix[b][a]=w; // its not a direct graph
	}
	++_edge_size;
	return true;
}

int GraphM::getVertexSize()
{
	return _vertex_size;
}

int GraphM::getEdgeSize()
{
	return _edge_size;
}

std::ostream& GraphM::toString(std::ostream& oOut)
{
	char str[100];
	
	for (int i=0; i<_vertex_size; ++i)
		oOut << "\t" << i;
	oOut << std::endl;

	for (int i=0; i<_vertex_size; ++i)
	{
		oOut << i << "\t";
		for (int j=0; j<_vertex_size; ++j)
		{
			if (_matrix[i][j]>-1)
			{
				sprintf( str, "%c[%d;%dm%i%c[%dm\t",0x1B,1,31, _matrix[i][j],0x1B,0); 
				oOut << str;
			} else {
				oOut << _matrix[i][j] << "\t";
			}	
		}	
		oOut << std::endl;
	}
	oOut << " edges= " << _edge_size << std::endl;
	oOut << "vertex= " << _vertex_size << std::endl;

	return oOut;
}



GraphM::~GraphM()
{
	for(int i=0; i<_vertex_size; ++i)
		free(_matrix[i]);
	free(_matrix);
}

std::ostream& operator<<(std::ostream & oOut, GraphM & iObj) {return iObj.toString(oOut);}

class GraphVertexIter
{
	public:
	GraphVertexIter(GraphM & G);
	GraphVertexIter & operator++();
	int operator*();
	bool end();
	void reset();
	
	private:
	GraphM & _graph;
	int _count;
};

GraphVertexIter::GraphVertexIter(GraphM & G): _graph(G), _count(0)
{
	
}	

int GraphVertexIter::operator*()
{
	return _count;
}

GraphVertexIter & GraphVertexIter::operator++()
{
	++_count;
	return *this;
}

bool GraphVertexIter::end()
{
	return (_count<_graph.getVertexSize());
}

void GraphVertexIter::reset()
{
	_count=0;
}

class GraphEdgeIter
{
	public:
	GraphEdgeIter(GraphM & G, int V);
	GraphEdgeIter & operator++();
	int operator*();
	bool end();
	void reset();
	
	private:
	GraphM & _graph;
	int _count;
	int _vertex;
};

GraphEdgeIter::GraphEdgeIter(GraphM & G, int V): _graph(G), _count(0), _vertex(V)
{
	while ( _count<_graph.getVertexSize() && _graph._matrix[_vertex][_count]<0 )
	{
		++_count;
	}
}	

int GraphEdgeIter::operator*()
{
	return _count;
}

GraphEdgeIter & GraphEdgeIter::operator++()
{
	do {
		++_count;
	} while (_count<_graph.getVertexSize() && _graph._matrix[_vertex][_count]<0 );
	
	return *this;
}

bool GraphEdgeIter::end()
{
	return (_count>=_graph.getVertexSize());
}

void GraphEdgeIter::reset()
{
	_count=0;
	while ( _count<_graph.getVertexSize() && _graph._matrix[_vertex][_count]<0 )
	{
		++_count;
	}

}

GraphVertexIter GraphM::getVertexIter()
{
	return GraphVertexIter(*this);
}

GraphEdgeIter GraphM::getEdgeIter(int V)
{
	return GraphEdgeIter(*this, V);
}
#endif
