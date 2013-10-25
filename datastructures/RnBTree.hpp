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
      return m_parent.m_parent;
    }
    return NULL;
  }

  RnBNode<T> * uncle() {
    RnBNode<T> * grandparent = grandparent();
    if (grandparent != NULL) {
      if (grandparent.m_left == m_parent)
        return grandparent.m_right;
      else
        return grandparent.m_left;
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
    void insert(T & value) {
      RnBNode<T> * new_node = insert_binary_tree(value);
      if (new_node != NULL)
        insert_case_1(new_node);
      // otherwise do nothing, value already exists
    }

    void remove(T & value) {
      // TODO
    }

  private:
    RnBNode<T> * m_root;
    static const RnBNode<T> m_leaf;

    void rotate_left(RnBNode<T> * n) {
      RnBNode<T> * p = n->m_parent; // parent
      RnBNode<T> * c = n->m_right;  // child

      n->m_right = c->m_left;
      n->m_right->m_parent = n;
      c->m_left = n;
      c->m_left->m_parent = c;
      if (p->m_left == n) {
        p->m_left = c;
        p->m_left->m_parent = p;
      } else {
        p->m_right = c;
        p->m_right->m_parent = p;
      }
    }

    void rotate_right(RnBNode<T> * n) {
      RnBNode<T> * p = n->m_parent;
      RnBNode<T> * c = n->m_right;

      n->m_left = c->m_right;
      n->m_left->m_parent = n;
      c->m_right = n;
      c->m_right->m_parent = c;
      if (p->m_left == n) {
        p->m_left = c;
        p->m_left->m_parent = p;
      } else {
        p->m_right = c;
        p->m_right->m_parent = p;
      }
    }

    // insert as in binary tree taking into account the extra leaves nodes at
    // the end
    RnBNode<T> * insert_binary_tree(T & value) {
      RnBNode<T> * new_node = NULL;
      bool insert_left = true;
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
              insert_left = false;
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
        if (insert_left) {
          curr_node->m_left = new_node;
        } else {
          curr_node->m_right = new_node;
        }

      } else {
        // root node must be BLACK
        m_root = new RnBNode<T>(BLACK, NULL, &m_leaf, &m_leaf);
        m_root.m_value = value;
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
      // If a node has a parent then it also has an uncle
      // (this is thanks to the black leaves)
      assert(uncle != NULL);
      if (uncle->m_color == RED) {
        // parent RED and uncle RED
        // set them black and check grandparent
        RnBNode<T> * parent = new_node->m_parent;
        RnBNode<T> * grandparent = new_node->grandparent();
        parent->m_color = BLACK;
        if (uncle != NULL) uncle->m_color = BLACK;
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
      assert(new_node->m_parent == RED);
      assert(grandparent->m_color == BLACK);
      assert((new_node->m_parent == new_node->m_parent->m_left &&
             new_node->m_parent->m_parent == grandparent->m_left) ||
             (new_node->m_parent == new_node->m_parent->m_right &&
              new_node->m_parent->m_parent == grandparent->m_right));
      grandparent->m_color = RED;
      new_node->m_parent = BLACK;
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
const RnBNode<T> RnBTree<T>::m_leaf(BLACK);

#endif
