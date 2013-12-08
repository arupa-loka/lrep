/*
  1 - root is everytime black
  2 - leaves are everytime black
  3 - from the root each path toward the leaves includes exactly the same
      number of black nodes
  4 - a red node can have only black children
*/

#ifndef __RED_AND_BLACK_TREE_HPP
#define __RED_AND_BLACK_TREE_HPP

#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <cassert>
// toGraphviz
#include <iostream>
#include <fstream>
#include "Stack2.hpp"


enum NODE_COLOR {
  RED=0,
  BLACK=1
};

template<class T>
struct RnBNode
{
  RnBNode(NODE_COLOR color=RED,
          RnBNode<T> * parent=NULL,
          RnBNode<T> * left=NULL,
          RnBNode<T> * right=NULL) :
    m_color(color), m_parent(parent), m_left(left), m_right(right) {}
  NODE_COLOR m_color;
  T m_value;
  RnBNode<T>* m_parent;
  RnBNode<T>* m_left;
  RnBNode<T>* m_right;

  RnBNode<T> * grandparent() {
   if (m_parent != NULL) {
      return m_parent->m_parent;
    }
    return NULL;
  }

  RnBNode<T> * uncle() {
    RnBNode<T> * grandparent = this->grandparent();
    if (grandparent != NULL) {
      if (grandparent->m_left == m_parent)
        return grandparent->m_right;
      else
        return grandparent->m_left;
    }
    return NULL;
  }

};

template<class T>
class RnBTree {
  public:
    RnBTree(): m_root(NULL) {}
    ~RnBTree() {}

    // call normal insert binary tree and then rebalance the tree
    bool insert(T & value) {
      RnBNode<T> * new_node = insert_binary_tree(value);
      if (new_node != NULL) {
        insert_case_1(new_node);
        return true;
      }
      // otherwise do nothing, value already exists
      return false;
    }

    void remove(T & value) {
      // TODO
    }
    void toGraphViz(const char* iFilePath);

  private:
    RnBNode<T> * m_root;
    static RnBNode<T> m_leaf;

    void rotate_left(RnBNode<T> * n) {
      RnBNode<T> * p = n->m_parent; // parent
      RnBNode<T> * c = n->m_right;  // child

      n->m_right = c->m_left;
      n->m_right->m_parent = n;
      c->m_left = n;
      n->m_parent = c;
      if (p!=NULL) {
        if (p->m_left == n)
          p->m_left = c;
        else
          p->m_right = c;
        c->m_parent = p;
      } else {
        c->m_parent = NULL;
        m_root = c;
      }
    }

    void rotate_right(RnBNode<T> * n) {
      RnBNode<T> * p = n->m_parent;
      RnBNode<T> * c = n->m_left;
      
      n->m_left = c->m_right;
      n->m_left->m_parent = n;
      c->m_right = n;
      n->m_parent = c;
      if (p!=NULL) {
        if (p->m_left == n)
          p->m_left = c;
        else
          p->m_right = c;
        c->m_parent = p;
      } else {
        c->m_parent = NULL;
        m_root = c;
      }
    }

    // insert as in binary tree taking into account the extra leaves nodes at
    // the end
    RnBNode<T> * insert_binary_tree(T & value) {
      RnBNode<T> * new_node = NULL;
      if (m_root != NULL) {
        RnBNode<T> * curr_node = m_root;

        // notice that we cannot rely on the m_leaf.parent value
        // then we have to save the leaf parent in curr_node
        while (1) {
          if (value < curr_node->m_value)
            if (curr_node->m_left != &m_leaf)
              curr_node = curr_node->m_left;
            else
              break; // leaf node reached
          else if (curr_node->m_value < value)
            if (curr_node->m_right != &m_leaf) {
              curr_node = curr_node->m_right;
            } else {
              break; // leaf node reached
            }
          else
            return NULL; // value already exists
        }
        // insert all new nodes as RED nodes
        new_node = new RnBNode<T>(RED,      // color
                                  curr_node,// parent
                                  &m_leaf,  // left child
                                  &m_leaf); // right child
        new_node->m_value = value;
        if (value < curr_node->m_value) {
          curr_node->m_left = new_node;
        } else {
          curr_node->m_right = new_node;
        }

      } else {
        // root node must be BLACK
        m_root = new RnBNode<T>(BLACK, NULL, &m_leaf, &m_leaf);
        m_root->m_value = value;
        new_node = m_root;
      }
      return new_node;
    }

    // case 1 root is NULL
    void insert_case_1(RnBNode<T> * new_node) {
      if (m_root==new_node) {
        // This is redoundant only the first time we create a root
        // but then can be called recursively from insert_case_3()
        // and in that case the root could be RED again
        m_root->m_color = BLACK;
      } else {
        insert_case_2(new_node);
      }
    }
    // case 2 parent is black
    void insert_case_2(RnBNode<T> * new_node) {
      assert(m_root->m_color == BLACK);
      if (new_node->m_parent->m_color == BLACK) {
        // do nothing
        return;
      }
      insert_case_3(new_node);
    }

    // insert_case_3
    // parent and uncle are RED and grandparent is BLACK
    void insert_case_3(RnBNode<T> * new_node) {
      //assert(m_root->m_color == BLACK);
      assert(new_node->m_parent->m_color == RED);
      assert(new_node->grandparent()->m_color == BLACK);
      RnBNode<T> * uncle = new_node->uncle();
      // If parent is RED then it cannot be the root,
      // then it must have a grandparent that is BLACK,
      // then it must have an uncle as well (thanks to the black leaves)
      assert(uncle != NULL);
      if (uncle->m_color == RED) {
        // parent RED and uncle RED
        // set them black and check grandparent
        RnBNode<T> * grandparent = new_node->grandparent();
        new_node->m_parent->m_color = BLACK;
        uncle->m_color = BLACK;
        grandparent->m_color = RED;
        // TODO recursion, can make it iterative
        insert_case_1(grandparent);
      } else {
        // uncle is BLACK
        insert_case_4(new_node);
      }
    }
    // insert_case_4
    void insert_case_4(RnBNode<T> * new_node) {
      RnBNode<T> * grandparent = new_node->grandparent();
      RnBNode<T> * parent = new_node->m_parent;
      assert(grandparent->m_color == BLACK);
      assert(parent->m_color == RED);
      assert(new_node->uncle()->m_color == BLACK);
      if (new_node == parent->m_right && parent == grandparent->m_left) {
        // apply left rotation on parent
        rotate_left(parent);
        // now new_node is the node that was parent in the line above
        new_node = new_node->m_left;
      } else if (new_node == parent->m_left &&
                 parent == grandparent->m_right) {
        // apply right rotation on parent
        rotate_right(parent);
        // now new_node is the node that was parent in the line above
        new_node = new_node->m_right;
     }
     insert_case_5(new_node);
    }

    // insert_case_5
    void insert_case_5(RnBNode<T> * new_node) {
      RnBNode<T> * grandparent = new_node->grandparent();
      assert(new_node->m_color == RED);
      assert(new_node->m_parent->m_color == RED);
      assert(grandparent->m_color == BLACK);
      assert((new_node == new_node->m_parent->m_left &&
             new_node->m_parent == grandparent->m_left) ||
             (new_node == new_node->m_parent->m_right &&
              new_node->m_parent == grandparent->m_right));
      grandparent->m_color = RED;
      new_node->m_parent->m_color = BLACK;
      // TODO check this check
      //if (new_node == new_node->m_parent->m_left) {
      if (new_node->m_parent == grandparent->m_left) {
        // rotate right on grandparent
        rotate_right(grandparent);
      } else {
        // rotate left on grandparent
        rotate_left(grandparent);
      }

    }

};

template<typename T>
RnBNode<T> RnBTree<T>::m_leaf(BLACK);

template < typename T >
void RnBTree<T>::toGraphViz(const char * iFilePath)
{
  std::ofstream out(iFilePath);
  if (!out.is_open()) {
    printf("Error: unable to open file: %s\n", iFilePath);
    return;
  }
  Stack2<RnBNode<T>*> stack;
    
  out << "digraph RnBTree {\n";
  out << "node [ shape=record, fixedsize=false];\n";
  //out << "{ rankdir=RL; }\n";
  //out << "{ rank=same; ";
        
  if (m_root)
    stack.push(m_root);

  while( !stack.empty() )
  {
    RnBNode<T>* p = stack.getTop();
    stack.pop();
      
    out << "\"" << p << "\"";
    out << "[";
    if (p->m_color==RED)
      out << "color=red ";
    else
      out << "color=black ";
    out << "label=\"" << p << " | ";
      
    out << "<f" << 0 << "> | ";
    out << "<f" << 1 << "> " << p->m_value << " | ";
    if (p->m_left)
      stack.push(p->m_left);
    out << "<f" << 2 << ">\"];\n";
    if (p->m_right)
      stack.push(p->m_right);

    if (p->m_left) 
    {
      out << "\"" << p << "\"";
      out << ":";
      out << "f" << 0 << " -> ";
      out << "\"" << p->m_left << "\":f1;\n";
    }
    if (p->m_right) 
    {
      out << "\"" << p << "\"";
      out << ":";
      out << "f" << 2 << " -> ";
      out << "\"" << p->m_right << "\":f1;\n";
    }

  }
  out << "}\n";
  out.close();
}
#endif
