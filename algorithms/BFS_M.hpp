#ifndef _BFS_M_HPP
#define _BFS_M_HPP

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Stack2.hpp"
#include "Queue.hpp"
#include "Pair.hpp"
#include "fstream"

class BFS_M
{
	public:
	BFS_M(GraphM & G);
	void bfs(int iRoot);
	void shortestPath(int iRoot, int iEnd);

	void reset();
	void toGraphViz( const char * iFilePath);
	std::ostream & toString(std::ostream & oOut);
	~BFS_M();
	
	private:
	void alloc();
	
	GraphM & _graph;

	int * _visited;
	int * _parent;
	int * _preorder;
	int * _inorder;
	int * _postorder;
	int * _forest;
	int * _depth;
	char ** _matrix;
	//int ** _matrix;
};

///////////////////////////////////////////////////////////////////////////////

BFS_M::BFS_M(GraphM & G): _graph(G), _visited(0), _parent(0), _preorder(0), _matrix(0)
{
	alloc();
	reset();
}

void BFS_M::bfs(int iRoot)
{
	//Stack2< Pair<int,int> > fringe;
	Queue< Pair<int,int> > fringe;

	fringe.push(Pair<int,int>(-1,iRoot));
	int precount = 0;
	int poscount = 0;
	int inocount = 0;
	int depthcount = 0;
	int P = -1;
	int forest=0;
	
	while (precount < _graph.getVertexSize())
	{	
		while ( !fringe.empty() )
		{
			Pair<int,int> pair = fringe.getTop(); fringe.pop();
			int v0 = pair.first();
			int v1 = pair.second();
			
			if ( !_visited[v1] )
			{
				_preorder[v1]= precount++;
				//_depth[v1] = depthcount;
				_visited[v1]=1;
				_parent[v1]=v0;
				_forest[v1]=forest;
	
				if ( v0 > -1 )
				{
					_matrix[v0][v1]='T';
				} else {
					_depth[v1]=0;
					depthcount=0;
				}
				
				bool noNewEdges = true;
				++depthcount;
				for ( GraphEdgeIter it = _graph.getEdgeIter(v1); !it.end(); ++it )
				{
					int v2 = *it;
					if (!_visited[v2] && _depth[v2]<0 )
					{	
						printf("pushed %i at depth %i from %i\n", v2, depthcount, v1);
						_depth[v2] = depthcount;
						fringe.push(Pair<int,int>(v1,v2));
						noNewEdges=false;
					}
					else
					{
						if ( !_graph.isDigraph() && _parent[v1] == v2 )
							_matrix[v1][v2]='P';
						else if ( _depth[v1] == _depth[v2] )
							_matrix[v1][v2]='C';
						else if ( _preorder[v2] < _preorder[v1] )	{
							/*
							p1 = _parent[v1];
							p2 = _parent[v2];
							int 
							while (p1 != p2 ) {
								if (p1>-1)p1=_parent[p1];	 
								if (p2>-1)p2=_parent[p2];	 
							}
							_matrix[v1][v2]='B';
							*/
						}
					}
				}

				if(noNewEdges){
					--depthcount;	  
					/*
					int p = v1;
					if (!fringe.empty()) {
						do {
							_postorder[p]=poscount; 	
							++poscount;
							p=_parent[p];
						} while ( p>-1 && p != fringe.getTop().first() );
					} else {
						do {
							_postorder[p]=poscount; 	
							++poscount;
							p=_parent[p];
						} while ( p>-1);
					}
					*/
				}
				
			}
			else 
			{	/*
				int p = v0;
				if (!fringe.empty()) {
					while ( p>-1 && p != fringe.getTop().first() ) {
						_postorder[p]=poscount; 	
						++poscount;
						p=_parent[p];
					}
				} else {
					while ( p>-1) {
						_postorder[p]=poscount; 	
						++poscount;
						p=_parent[p];
					}
				}
				*/
				if (_depth[v0] == _depth[v1])
					_matrix[v0][v1]='C';
				else 
					_matrix[v0][v1]='B';
				
			}

		}

		if (fringe.empty() && precount<_graph.getVertexSize())
		{
			for (int i=0; i<_graph.getVertexSize(); ++i)
			{
				  if (!_visited[i])
				  {
						fringe.push(Pair<int,int>(-1,i));
						break;
				  }
			}
			++forest;
		}
	}
}

void BFS_M::alloc()
{
	_visited   = (int*)malloc(sizeof(int)*_graph.getVertexSize());
	_parent    = (int*)malloc(sizeof(int)*_graph.getVertexSize());
	_preorder  = (int*)malloc(sizeof(int)*_graph.getVertexSize());
	_inorder   = (int*)malloc(sizeof(int)*_graph.getVertexSize());
	_postorder = (int*)malloc(sizeof(int)*_graph.getVertexSize());
	_forest    = (int*)malloc(sizeof(int)*_graph.getVertexSize());
	_depth    = (int*)malloc(sizeof(int)*_graph.getVertexSize());
	_matrix = (char**)malloc( sizeof(char*) * _graph.getVertexSize());
	for (int i=0; i<_graph.getVertexSize(); ++i)
	{
		_matrix[i] = (char*)malloc( sizeof(char) * _graph.getVertexSize());
	}
}

void BFS_M::reset()
{
	for (int i=0; i<_graph.getVertexSize(); ++i)
	{
		_visited[i] = 0;
		_parent[i]	= -1;
		_preorder[i]	= -1;
		_inorder[i]		= -1;
		_postorder[i]	= -1;
		_forest[i]		= -1;
		_depth[i]		= -1;

		memset(_matrix[i], '-', sizeof(char) * _graph.getVertexSize());
	}
	
}

void BFS_M::toGraphViz( const char * iFilePath)
{
	std::ofstream out(iFilePath);
	if (out.is_open())
	{	
		char edge[5];
		if (_graph.isDigraph()) {
			out << "digraph BFS_SpanningTree {\n";
			strcpy(edge, " -> ");
		} else {
			out << "graph BFS_SpanningTree {\n";
			strcpy(edge, " -- ");

		}
		
		out << "node [ fixedsize=false];\n";
		out << "{ rankdir=RL; }\n";
		out << "{ rank=same; ";
		for (int i=0; i<_graph.getVertexSize(); ++i)
		{
			if (_parent[i]<0)
				//out << i << " ";
				out << "\"" << i << ":" <<  _preorder[i] << ":" << _postorder[i] << ":" << _depth[i] << "\" " ;
			
		}
		out << "}\n";
		
		for (int i=0; i<_graph.getVertexSize(); ++i)
		{
			for (int j=0; j<_graph.getVertexSize(); ++j)
			{
				if ( _matrix[i][j]!='-' && _matrix[i][j]!='P' )
				{	
					char color[8] = "#FF0000";
					char style[16]= "bold";
				
					if (_matrix[i][j]=='B') { strcpy(color, "#00FF00"); strcpy(style, "dotted");}
					else if (_matrix[i][j]=='D') { strcpy(color, "#0000FF"); strcpy(style, "dotted");}
					else if (_matrix[i][j]=='P') { strcpy(color, "#000000"); strcpy(style, "dotted");}
					else if (_matrix[i][j]=='C') { strcpy(color, "#FF9933"); strcpy(style, "dotted");}
				
					
					out << "\"" << i << ":" <<  _preorder[i] << ":" << _postorder[i] << ":" << _depth[i] << "\"";
					out << edge;
					out << "\"" << j << ":" <<  _preorder[j] << ":" << _postorder[j] << ":" << _depth[j] << "\"";
					out << " [";
					out << " label=\" "<< _matrix[i][j] <<" \"";
					out << " color=\""<< color <<"\"";
					out << " style=\""<< style <<"\"";
					out << "];\n";
				}
			}
		}
		out << "}\n";
		out.close();
	}
	else
	{
		printf("Error: unable to open file: %s\n", iFilePath);
	}
	
}

std::ostream & BFS_M::toString(std::ostream & oOut)
{
	char str[100];
	int color = 37;
	
	for (int i=0; i<_graph.getVertexSize(); ++i)
		oOut << "\t" << i;
	oOut << std::endl;

	for (int i=0; i<_graph.getVertexSize(); ++i)
	{
		oOut << i << "\t";
		for (int j=0; j<_graph.getVertexSize(); ++j)
		{
			if (_matrix[i][j]=='T')      color=31;
			else if (_matrix[i][j]=='B') color=32;	
			else if (_matrix[i][j]=='P') color=33;	
			else if (_matrix[i][j]=='D') color=34;	
			else color=37;
			sprintf( str, "%c[%d;%dm%c%c[%dm\t",0x1B,1,color, _matrix[i][j],0x1B,0); 
			oOut << str;

		}	
		oOut << std::endl;
	}

	oOut << std::endl;
	for (int i=0; i<_graph.getVertexSize(); ++i)
		oOut << "\t" << i;
	oOut << std::endl;
	
	oOut << "par\t";
	for (int i=0; i<_graph.getVertexSize(); ++i)
		oOut << _parent[i] << "\t";
	oOut << std::endl;

	oOut << "pre\t";
	for (int i=0; i<_graph.getVertexSize(); ++i)
		oOut << _preorder[i] << "\t";
	oOut << std::endl;
	oOut << "ino\t";
	for (int i=0; i<_graph.getVertexSize(); ++i)
		oOut << _inorder[i] << "\t";
	oOut << std::endl;
	oOut << "pos\t";
	for (int i=0; i<_graph.getVertexSize(); ++i)
		oOut << _postorder[i] << "\t";
	oOut << std::endl;

	oOut << "for\t";
	for (int i=0; i<_graph.getVertexSize(); ++i)
		oOut << _forest[i] << "\t";
	oOut << std::endl;
	
	oOut << "dep\t";
	for (int i=0; i<_graph.getVertexSize(); ++i)
		oOut << _depth[i] << "\t";
	oOut << std::endl;

	oOut << std::endl;

	return oOut;
}

std::ostream & operator<<(std::ostream & oOut, BFS_M & iObj) { return iObj.toString(oOut); }

BFS_M::~BFS_M()
{
	if (_visited) { free(_visited); }
	if (_parent)  { free(_parent);  }
	if (_preorder)   { free(_preorder);   }
	if (_inorder)   { free(_inorder);   }
	if (_postorder)   { free(_postorder);   }
	if (_depth)   { free(_depth);   }
	if (_matrix) 
	{
		for (int i=0; i<_graph.getVertexSize(); ++i)
		{
			if (_matrix[i])
				free(_matrix[i]);
		}
		free(_matrix);
	}
}

#endif

