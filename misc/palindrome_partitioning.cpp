#include <string>
//#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <limits>

void print_substr(char * s, int len) {
  // len >= 0
  char * end = s+len;
  printf("\"");
  for (; s<end; ++s) {
    printf("%c", *s);
  }
  printf("\"");
}

bool is_palindrome(char * start, int len) {
  char * end = start+len-1;
  for (; start<=end; ++start, --end) {
    if (*start!=*end) {
      return false;
    }
  }
  return true;
}

bool is_palindrome2(char * s, int start, int end, int ** save) {
  int bk_start = start;
  for (; start<=end; ++start, --end) {
    int val = save[start][end];
    if (val>0) {
      break;
    }
    if (val==0 || s[start]!=s[end]) {
      --start;
      ++end;
      // Record in save[][] that all the previous subproblems were not leading
      // to a palindrome string.
      while (bk_start <= start) {
        save[start][end] = 0;
        --start;
        ++end;
      }
      return false;
    }
    // else val==-1 && s[start]==s[end]
  }
  --start;
  ++end;
  // Record in save[i][j] that all the previous subproblems were leading to a
  // palindrome string.
  while (bk_start <= start) {
    save[start][end] = end-start+1;
    --start;
    ++end;
  }
  return true;
}

void print_palindromes_by_brute_force(char* s, int len) {
  for (int w=1; w<=len; ++w) {
    printf("%d ", w);
    for (int i=0; i+w<=len; ++i) {
      if (is_palindrome(s+i, w)) {
        print_substr(s+i, w);
        printf(" ");
      }
    }
    printf("\n");
  }
}


class BfdFifo {
  public:
    BfdFifo(int size) {
      fifo = new int[size];
      fifo_max_size = size;
      fifo_len = 0;
      fifo_head_pos = 0;
    }
    void push(int elem) {
      int pos = fifo_head_pos + fifo_len;
      if (pos >= fifo_max_size)
        pos-=fifo_max_size;
      fifo[pos] = elem;
      ++fifo_len;
    }
    int pop() {
      if (fifo_len > 0) {
        int res = fifo[fifo_head_pos];
        if (++fifo_head_pos>=fifo_max_size)
          fifo_head_pos -= fifo_max_size;
        --fifo_len;
        return res;
      }
      return -1;
    }
    int len() {
      return fifo_len;
    }
  private:
    int *fifo;
    int fifo_max_size;
    int fifo_len;
    int fifo_head_pos;
};

class DfsLifo {
  public:
    DfsLifo(int size) {
      lifo = new int[size];
      lifo_max_size = size;
      lifo_len = 0;
    }
    void push(int elem) {
      lifo[lifo_len++] = elem;
    }
    int pop() {
      if (lifo_len > 0) {
        return lifo[--lifo_len];
      }
      return -1;
    }
    int len() {
      return lifo_len;
    }
  private:
    int *lifo;
    int lifo_max_size;
    int lifo_len;
};

void print_partitioning(char * s, int len, int * parent) {
  // print out palindrome partitioning
  // target node was #len
  DfsLifo lifo(len+1);
  lifo.push(len);
  for (int i=len; parent[i]!=-1; i=parent[i]) {
    //print_substr(s+parent[i], len-parent[i]);
    //printf(" ");
    lifo.push(parent[i]);
  }
  //printf("\n");
  int from = lifo.pop();
  while (lifo.len()>0) {
    int to = lifo.pop();
    print_substr(s+from, to-from);
    from = to;
    printf(" ");
  };
  printf("\n");

}

// in graph there is a tree roted at node 'len'.
// Do a DFS starting from node 'len' and going up to node '0'.
void print_partitioning2(char * s, int len, bool * graph) {
  // print out palindrome partitioning
  // target node was #len
  DfsLifo lifo(len+1);
  int * parent = new int[len+1];
  parent[len]=-1;
  lifo.push(len);
  while (lifo.len()>0) {
    int curr_node = lifo.pop();
    if (curr_node==0) {
      // root reached, print what's in the parent[]
      while(parent[curr_node]!=-1){
        print_substr(s+curr_node, parent[curr_node]-curr_node);
        curr_node = parent[curr_node];
        printf(" ");
      }
      printf("\n");
      continue;
    }
    for (int i=0; i<len+1; ++i) {
      if (graph[(len+1)*curr_node+i]) {
        lifo.push(i);
        parent[i] = curr_node;
      }
    }
  }
}


void print_palindrome_partition_by_bfd(char * s, int len, int **save) {
  // treat the save matrix as a graph matrix. If the entry save[i][j] > 0,
  // that means s[i:j] is a palindrome. Then from s[i] we are connected to s[j+1].
  // We want the solution that starting from s[0], brings to s[len+1], with
  // the minimum number of connections (where
  // len+1 is a virtual node of the graph that represent the end of string).
  // Then we apply a basic version of the Dijkstra shortest path algorithm,
  // where all edges among nodes have weight 1. That corresponds to apply a
  // BFD search. Then we don't need a priority queue, because all weights are
  // the same, and we can work just with a FIFO.
  // The best solutions are saved in the parent2 matrix, and the original
  // subpalindrome strings are reconstructed by running a DFS on it.

  // TODO does new initialize memory to zero by default?
  BfdFifo fifo(len+1);
  bool * visited = new bool[len+1];
  int * node_cost = new int[len+1];
  //int * parent = new int[len+1];
  bool * parent2 = new bool[(len+1)*(len+1)];
  memset(parent2, 0, sizeof(bool)*(len+1)*(len+1));
  //parent[0] = -1; // parent of root node is -1
  for (int i=0; i<len+1; ++i) {
    node_cost[i] = std::numeric_limits<int>::max();
    visited[i] = false;
  }
  node_cost[0] = 0; // cost of root is zero
  // Push the index of the first character in the fifo.
  fifo.push(0);
  while(fifo.len()>0) {
    int curr_node = fifo.pop();
    if (curr_node == len) {
      printf("Solution found: cost=%d, fifo.len=%d\n", node_cost[curr_node]-1,
             fifo.len());
      continue;
    }
    // Explore children of curr_node.
    // Start from curr_node because "save" is a triangular matrix.
    // Each row is a character index in the string, and from it we can only go
    // forward not backward.
    int * children = save[curr_node];
    for (int i=curr_node; i<len; ++i) {
      // is save[curr_node][i] > 0 then the substring s[curr_node:i] is
      // palindrome and we can jump to the start of the next substring if
      // it has not been visited so far: !visited[i+1].
      // Though we only want the best solutions, the one with less substrings
      // (less splits), then we check if the number of splits collected so far
      // to reach curr_node plus 1: node_cost[curr_node]+1,
      // is going to be <= to the number of splits collected so far to reach
      // i+1. We also want to report solutions with equal costs then the
      // comparison is <= and not just <.
      // Only solutions that bring to the node 'len' will be printed.
      if (children[i]>0 && !visited[i+1] &&
          node_cost[curr_node]+1 <= node_cost[i+1]) {
            node_cost[i+1] = node_cost[curr_node]+1;
            //printf("fif.push %d with cost %d\n", i+1, node_cost[i+1]);
            //parent[i+1] = curr_node;
            // save in the matrix parent2, all the links for the best solutios
            // found. The following line means: from curr_node there is an
            // optimal solution to reach i+1.
            parent2[(len+1)*(i+1)+curr_node] = 1;
            // push on the fifo the next node (index of the character in the
            // string) i+1.
            fifo.push(i+1);
      }
    }
    // all the children of curr_node have been visited.
    visited[curr_node] = true;
  }

  // print out palindrome partitioning
  // target node was #len
  // print_partitioning(s, len, parent);
  print_partitioning2(s, len, parent2);
}

void print_palindromes_by_dyn_prog(char *s, int len) {
  // for each string length possible from 1 to len-1, print
  // all the palindrome substring found and save them in a matrix
  // save[][]

  // Allocate and initialize matrix save[][].
  // save[i][j] record if the substring in s, starting at i and ending at j is
  // palindrome or not. If save[i][j] is:
  // - > 0 then it's palindrome and the value stored is the length
  // - ==0 then it's not palindrome
  // - ==-1 then it's still undefined
  int **save = new int*[len];
  for (int i=0; i<len; ++i) {
    save[i] = new int[len];
    for (int j=0; j<len; ++j) {
      if (i!=j) {
        save[i][j] = -1;
      } else {
        // not necessary but should speed up the next phase.
        // When i==j this identify a single character in the input
        // string s. Then a single character it's always palindrome.
        save[i][j] = 1;
      }
    }
  }
  // find all the palindrome substrings, making use of dynamic programming
  // by saving intermediate subproblems in the save[][] matrix.
  for (int w=1; w<=len; ++w) {
    printf("%d ", w);
    for (int i=0; i+w<=len; ++i) {
      if (is_palindrome2(s, i, i+w-1, save)) {
        print_substr(s+i, w);
        printf(" ");
      }
    }
    printf("\n");
  }

  // print the save matrix
  //for (int i=0; i<len; ++i) {
  //  for(int j=0; j<len; ++j) {
  //    printf("%.3d ", save[i][j]);
  //  }
  //  printf("\n");
  //}

  // print the palindrome partitioning with less splits.
  // print more than one if more than one optimal solutions are found.
  print_palindrome_partition_by_bfd(s, len, save);
}

size_t string_len(char * s) {
  size_t i = 0;
  while (*s != '\0') {
    ++i;
    ++s;
  }
  return i;
}


int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cout << "Error: one argument is required." << std::endl;
    return 1;
  }
  //char s[] = "aabaaabbaaabaa";
  //char s[] = "aabaabbaaabaabbaaaabaabababaaabbaabbaabaababaaaaabbaababbababbba";
  //char s[] = "1234567";
  char * s = argv[1];
  int len = string_len(argv[1]); // exclude the \0
  printf("\"%s\" %d\n", s, len);
  /*
  char ***save = new char**[len];
  memset(save, 0, len);
  for (int w=1; w<=len; ++w) {
    save[w-1] = new char*[len-w+1];
    memset(save[w-1], 0, len-w+1);
    for (int i=0; i+w<=len; ++i) {
      save[w-1][i] = new char[w+1]; // plus \0
      memset(save[w-1][i], 0, w+1);
    }
  }
  */
  //print_palindromes_by_brute_force(s, len);
  print_palindromes_by_dyn_prog(s, len);

  return 0;
}
