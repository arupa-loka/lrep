#ifndef _DFS_M_HPP
#define _DFS_M_HPP

#include "Stack2.hpp"
#include "Queue.hpp"
#include "Pair.hpp"
#include "Vector.hpp"
#include "fstream"
#include "GraphM.hpp" 
#include <cstdio>
#include <cstdlib>
#include <string>

class DFS_M
{
  public:
  DFS_M(GraphM & G);
  void dfs(int iRoot);
  void shortestPath(int iRoot, int iEnd);

  void reset();
  void toGraphViz( const char * iFilePath);
  std::ostream & toString(std::ostream & oOut);
  ~DFS_M();
  
  private:
  void alloc();
  char tagEdge(int v0, int v1);
  void cycleCheck(char, int, int, int &);

  GraphM & _graph;

  int * _visited;
  int * _parent;
  int * _preorder;
  int * _inorder;
  int * _postorder;
  int * _forest;

  struct CycleProperties {
    // B edge associated with this cycle
    int v0, v1;
    CycleProperties() {
      //printf("CP()\n");
    }
    ~CycleProperties() {
      //printf("~CP()\n");
    }
  };

  // cycle index to vector of vetex in cycle
  Vector< CycleProperties > _cycle;
  // the index of this vector represents a vertex, the value mapped is a
  // vector containing the cycle numbers, to which the vertex belongs.
  Vector< Vector<int> > _is_in_cycle;
  char ** _matrix;
  //int ** _matrix;
};

///////////////////////////////////////////////////////////////////////////////

DFS_M::DFS_M(GraphM & G): _graph(G), _visited(0), _parent(0), _preorder(0), _matrix(0)
{
  alloc();
  reset();
}

void DFS_M::dfs(int iRoot)
{
  Stack2< Pair<int,int> > fringe;
  //Queue< Pair<int,int> > fringe;

  fringe.push(Pair<int,int>(-1,iRoot));
  int precount = 0; // pre-order counter
  int poscount = 0; // post-order counter
  int inocount = 0; // in-order counter
  int P = -1;
  int forest=0;
  int cyclecount = 0; // number of cycles found so far
  
  while (precount < _graph.getVertexSize())
  { 
    while ( !fringe.empty() )
    {
      Pair<int,int> pair = fringe.getTop(); fringe.pop();
      int v0 = pair.first();
      int v1 = pair.second();
      
      if (v0==-2) {
        _postorder[v1]=poscount;
        ++poscount;
        continue;
      }

      if ( !_visited[v1] )
      {
        // first time we see this vertex, push a dummy edge on the fringe
        // before pushing all its children. When the dummy edge will be popped
        // that means all the subtrees rooted under this vertex v1, will have been
        // explored completely, and we can assign the postorder index to the vertex
        fringe.push(Pair<int,int>(-2,v1));
        _preorder[v1]=precount; ++precount;
        _visited[v1]=1;
        _parent[v1]=v0;
        _forest[v1]=forest;
  
        if ( v0 > -1 )
        {
          _matrix[v0][v1]='T';
        }
        //std::cout << "(" << v1 << ")" << std::endl;
    
        for ( GraphEdgeIter it = _graph.getEdgeIter(v1); !it.end(); ++it )
        {
          int v2 = *it;
          
          if (!_visited[v2]) {
            fringe.push(Pair<int,int>(v1,v2));
          } else { // already visited
            char edgeType = tagEdge(v1,v2);
            cycleCheck(edgeType, v1, v2, cyclecount);
          }
        }
      } else { // already visited
        char edgeType = tagEdge(v0,v1);
        cycleCheck(edgeType, v0, v1, cyclecount);
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

void DFS_M::cycleCheck(char edgeType, int v0, int v1, int & cyclecount)
{
  if (edgeType=='B') {
    // there is definitely a cycle
    printf("Cycle %d: B(%d, %d) ", cyclecount, v0, v1);
    // new cycle defined by its B edge v0 v1
    CycleProperties new_cycle;
    new_cycle.v0 = v0;
    new_cycle.v1 = v1;
    _cycle.pushBack(new_cycle);
    int p = v0;
    while( p != v1 ) {
      // you don't need to save a comple B cycle.
      // the two vertex are enough to retrace it quickly
      // through the parent array.
      _is_in_cycle[p].pushBack(cyclecount);
      printf("%d is in cycle %d\n", p, cyclecount);
      //printf("%d <- ", p);
      p = _parent[p];
    }
    _is_in_cycle[p].pushBack(cyclecount);
    printf("%d is in cycle %d\n", p, cyclecount);
    //printf("%d <- %d\n", p, v0);
    ++cyclecount;
  } else if (edgeType=='F') {
    // there is a cycle only if:
    // starting from v1 or one of its descendents there is a B edge
    // that is part of a cycle that include v0 or one of his ancestors.
    // That means the possible new cycle is a subcycle of an existing one.
    // Update 2013/09/14: there is a cycle if v0 and v1 are already part
    // of the same cycle.
    // Optimization: If number of 'B' edges is zero, no cycle is possible.
    // TODO
    // cycles to which v0 belongs
    Vector<int> & cv0 = _is_in_cycle[v0];
    // cycles to which v1 belongs
    Vector<int> & cv1 = _is_in_cycle[v1];
    for (int i=0, j=0; i<cv0.size() && j<cv1.size(); ) {
      if (cv0[i] < cv1[j])
        ++i;
      else if (cv0[i] > cv1[j])
        ++j;
      else {
        // sub-cycle found.
        // Both vertex of F edge belong to the same cycle. 
        // The new cycle is defined by its B edge v0 v1
        // that also defines the main-cycle
        // TODO optim, use a reference here
        CycleProperties & main_cycle = _cycle[cv0[i]];
        _cycle.pushBack(main_cycle);
        printf("Cycle %d: F", cyclecount);
        printf("(%d, %d) creates a sub-cycle over: B(%d, %d)\n", v0, v1, main_cycle.v0, main_cycle.v1);
        ++cyclecount;
        ++i; ++j;
      }
    }
  } else if (edgeType=='C') {
    // there is a cycle only if:
    // starting from v1 or one of its descendents there is a B edge
    // that is part of a cycle that include v0 or one of his ancestors.
    // That means the possible new cycle connects two existing cycles,
    // or/and extends an existing one.
    // Update 2013/09/14: there is a cycle if v1 results to be already part of
    // a cycle and v0 has an ancestor that is in the same v1 cycle.
    // TODO
    // v1 is in the following cycles.
    Vector<int> & v1_cycles = _is_in_cycle[v1];
    for (int i=0; i<v1_cycles.size(); ++i) {
      // check if a cycle on this node has an ancestor with null postorder number
      int cycle_number = v1_cycles[i];
      // TODO optim use a reference here
      CycleProperties & cycle_prop = _cycle[cycle_number];
      if (_postorder[cycle_prop.v1] == -1) {
        printf("Cycle %d: C", cyclecount);
        printf("(%d, %d) extends a cycle over: B(%d, %d) TODO\n", v0, v1, cycle_prop.v0, cycle_prop.v1);
        // describe the new cycle with the same B edge of the base-cycle
        _cycle.pushBack(cycle_prop);
        int p = v0;
        while (p != cycle_prop.v1) {
          _is_in_cycle[p].pushBack(cyclecount);
          printf("%d is in cycle %d\n", p, cyclecount);
          p = _parent[p];
        }
        // not necessary to add B.v1 here
        ++cyclecount;
        break;
      }
    }
  }
}

char DFS_M::tagEdge(int v0, int v1)
{
  // In a not directed graph, with DFS, we cannot have
  // Cross or Forward edges, but only Back edges
  char edgeType;
  if ( !_graph.isDigraph() ) {
    if (_parent[v0] == v1 ) {
      // this is a un-directed graph, if we already visited this edge in
      // one direction, don't follow it also in the other direction
      edgeType='P';
    } else {
      edgeType='B';
    }  
  } else { // is Diagraph
    if ( _postorder[v1]==-1 ) {
      // Back edge
      // the edge ends on a partially explored vertex,
      // the exploration it's not finished on that vertex
      // because we didn't assigned to it a postorder index yet.
      edgeType='B';
    } else if ( _preorder[v0] < _preorder[v1] ) {
      // Forward edge
      edgeType='F';
    } else {
      // Cross edge 
      // preorder[v1] > preorder[v2]
      // preorder[v1] == preorder[v2] that is not a Cross edge but a reflexive one
      // is included in the Back edge case, cause that means the vertex was still
      // partially explored
      edgeType='C';
    }
  }
  _matrix[v0][v1]=edgeType;
  return edgeType;
}


void DFS_M::alloc()
{
  _visited   = new int[_graph.getVertexSize()];
  _parent    = new int[_graph.getVertexSize()];
  _preorder  = new int[_graph.getVertexSize()];
  _inorder   = new int[_graph.getVertexSize()];
  _postorder = new int[_graph.getVertexSize()];
  _forest    = new int[_graph.getVertexSize()];
  _matrix  = new char*[_graph.getVertexSize()];
  _is_in_cycle.realloc(_graph.getVertexSize());
  for (int i=0; i<_graph.getVertexSize(); ++i)
  {
    _matrix[i] = new char[_graph.getVertexSize()];
  }
}

void DFS_M::reset()
{
  for (int i=0; i<_graph.getVertexSize(); ++i)
  {
    _visited[i] = 0;
    _parent[i]  = -1;
    _preorder[i]  = -1;
    _inorder[i]   = -1;
    _postorder[i] = -1;
    _forest[i]    = -1;

    memset(_matrix[i], '-', sizeof(char) * _graph.getVertexSize());
  }
}

void DFS_M::toGraphViz( const char * iFilePath)
{
  std::ofstream out(iFilePath);
  if (out.is_open())
  { 
    char edge[5];
    if (_graph.isDigraph()) {
      out << "digraph DFS_SpanningTree {\n";
      strcpy(edge, " -> ");
    } else {
      out << "graph DFS_SpanningTree {\n";
      strcpy(edge, " -- ");

    }
    
    out << "node [ fixedsize=false];\n";
    out << "{ rankdir=RL; }\n";
    out << "{ rank=same; ";
    for (int i=0; i<_graph.getVertexSize(); ++i)
    {
      if (_parent[i]<0)
        //out << i << " ";
        out << "\"" << i << ":" <<  _preorder[i] << ":" << _postorder[i] << "\" " ;
      
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
          else if (_matrix[i][j]=='F') { strcpy(color, "#0000FF"); strcpy(style, "dotted");}
          else if (_matrix[i][j]=='P') { strcpy(color, "#000000"); strcpy(style, "dotted");}
          else if (_matrix[i][j]=='C') { strcpy(color, "#FF9933"); strcpy(style, "dotted");}
        
          
          out << "\"" << i << ":" <<  _preorder[i] << ":" << _postorder[i] << "\"";
          out << edge;
          out << "\"" << j << ":" <<  _preorder[j] << ":" << _postorder[j] << "\"";
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

std::ostream & DFS_M::toString(std::ostream & oOut)
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
      else if (_matrix[i][j]=='F') color=34;  
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

  oOut << std::endl;

  return oOut;
}

std::ostream & operator<<(std::ostream & oOut, DFS_M & iObj) { return iObj.toString(oOut); }

DFS_M::~DFS_M()
{
  if (_visited) { delete[](_visited); }
  if (_parent)  { delete[](_parent);  }
  if (_preorder)   { delete[](_preorder);   }
  if (_inorder)   { delete[](_inorder);   }
  if (_postorder)   { delete[](_postorder);   }
  if (_matrix) 
  {
    for (int i=0; i<_graph.getVertexSize(); ++i)
    {
      if (_matrix[i])
        delete[](_matrix[i]);
    }
    delete[](_matrix);
  }
}

#endif

