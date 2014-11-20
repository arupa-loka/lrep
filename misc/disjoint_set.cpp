#include <iostream>
#include <sstream>
#include <cstdio>
#include <cstdlib>
#include <cassert>
#define DSET_SIZE 64

// https://en.wikipedia.org/wiki/Disjoint-set_data_structure
// run it as:
// g++ disjoint_set.cpp -o disjoint_set && ./disjoint_set | dot -Tpng > /tmp/dot_image.png && eog /tmp/dot_image.png
// where eog is your picture viewer.

struct ListNode {
  int val;
  int subtree_size;
  ListNode * parent;
};

class DisjointSet {
  public:
    DisjointSet() {
      alloc_size = DSET_SIZE;
      array = new ListNode*[alloc_size]();
      for (int i=0; i<alloc_size; ++i) {
        assert(array[i]==NULL);
      }
      set_size = 0;
    }

    ListNode * Find(int x) {
      assert(x<alloc_size);
      ListNode * node = array[x];
      while (node != node->parent) {
        node = node->parent;
      }
      return node;
    }

    // This variant keeps the tree flat applying path compression.
    ListNode * Find2(int x) {
      assert(x<alloc_size);
      ListNode * node = array[x];
      if (node != node->parent) {
        node->parent = Find2(node->parent->val);
      }
      return node->parent;
    }

    // This version tries to determine the new subtree high at the root.
    // TODO: It doesn't work, as you might compress one branch up to the root, but
    // there might still be another branch for the same root that is longer.
    // Then you cannot infer from the compressed branch only if the subtree high from
    // the root has changed or not.
    /*
    ListNode * Find3(int x) {
      assert(x<alloc_size);
      ListNode * node = array[x];
      int node_compressed = 0;
      if (node != node->parent) {
        ListNode * parent = InternalFind3(node->parent->val, node_compressed);
        node->parent = parent;
        // A child attached to the root has subtree high==1.
        node->subtree_high = 1;
      }
      if (node_compressed > 0) {
        node->subtree_high -= node_compressed;
      }
      return node;
    }
    */

    bool Union(int x, int y) {
      assert(x<alloc_size);
      assert(y<alloc_size);
      ListNode * root_x = Find2(x);
      ListNode * root_y = Find2(y);
      if (root_x->val != root_y->val) {
        // merge them
        if (root_x->subtree_size > root_y->subtree_size) {
          root_y->parent = root_x;
          root_x->subtree_size += root_y->subtree_size;
          // root_x->subtree_size unchanged as root_y->subtree_size is shorter than
          // root_x->subtree_size, then this doesn't increase the longest path
          // from root_x to any of its leaves.
        } else { //if (root_y->subtree_size > root_x->subtree_size) {
          root_x->parent = root_y;
          root_y->subtree_size += root_x->subtree_size;
          // root_y->subtree_size doesn't change for the same reasons as above.
        }
        /*
        else {
          // root_x->subtree_size == root_y->subtree_size
          root_x->parent = root_y;
          root_y->subtree_size += 1;
        }
        */
        //root_x->parent = root_y;
        set_size -= 1;
        return true;
      }
      return false;
    }

    void MakeSet(int x) {
      assert(x<alloc_size);
      if (array[x]!=NULL) return;
      ListNode * node = new ListNode;
      node->val = x;
      node->subtree_size = 1;
      node->parent = node;
      array[x] = node;
      set_size += 1;
      return;
    }

    std::string toDot() {
      std::ostringstream out;
      bool * visited = new bool[alloc_size];
      for (int i=0; i<alloc_size; ++i) visited[i] = false;

      out << "digraph DisjointSet {\n";
      for (int i=0; i<alloc_size; ++i) {
        ListNode * node = array[i];
        while (node != node->parent && !visited[node->val]) {
          out << node->val << " -> " << node->parent->val << ";\n";
          visited[node->val] = true;
          //visited[node->parent->val] = true;
          node = node->parent;
        }
      }
      for (int i=0; i<alloc_size; ++i) {
        ListNode * node = array[i];
        if (!visited[node->val]) {
          out << node->val << ";\n";
          visited[node->val] = true;
        }
        out << node->val << "[label=\"" << node->val <<
                            " size=" << node->subtree_size <<
                            "\"];\n";
      }
      delete[] visited;
      out << "}";
      return out.str();
    }

    int size() {
      return set_size;
    }

  private:
    // As explained above this is not working at the moment.
    /*
    ListNode * InternalFind3(int x, int & node_compressed) {
      assert(x<alloc_size);
      ListNode * node = array[x];
      if (node != node->parent) {
        node_compressed += 1;
        node->parent = InternalFind3(node->parent->val, node_compressed);
        // A child attached to the root has subtree_high==1.
        node->subtree_high = 1;
      }
      // TODO how do you determine the new subtree_high at the root?
      // It's not necessarily 2, as you might start searching the list from the
      // middle, then attaching directly to the root only the nodes above that
      // point, but not the ones below it.
      return node->parent;
    }
    */
    ListNode ** array;
    size_t set_size;
    size_t alloc_size;
};

int main(int argc, char *argv[]) {
  DisjointSet ds;
  for (int set_number=0; set_number<DSET_SIZE; ++set_number) {
    ds.MakeSet(set_number);
  }
  srand(time(NULL));
  int unions = 0;
  int attempts = 0;
  while (ds.size() > 1) {
    // generate random couples and do Union operations, up to when there is
    // only a single set left.
    int x = rand() % DSET_SIZE;
    int y = rand() % DSET_SIZE;
    if (ds.Union(x,y)) {
      unions += 1;
    } else {
      attempts += 1;
    }
    //std::cout << ds.toDot() << std::endl;
  }
  std::cerr << "unions=" << unions << ", attempts=" << attempts << std::endl;
  std::cout << ds.toDot() << std::endl;
  return 0;
}
