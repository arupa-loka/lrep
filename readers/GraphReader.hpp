#ifndef _GRAPH_READER_HPP
#define _GRAPH_READER_HPP

//#include "GraphT.hpp"
#define MAXLINE 512

class Node;
template <class T, class Weight> class GraphT;


class GraphReader {
	public:
	int readGraphT( const char *iFileName, GraphT<Node*,int>** oGraph );
};

#endif

