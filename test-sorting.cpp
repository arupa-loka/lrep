#include "QuickSort.hpp"

int main(int argc, char *argv[])
{
  int n = 16;
  int A[n];
  init_random_array(A,n);
  //int n = 16;
  //int A[] = {46, 85, 68, 40, 25, 40, 72, 76, 1, 64, 58, 24, 93, 39, 2, 41 };
  

  dump_array(A,n);
  printf("sorted array:\n");
  //insertion_sort(A,n);
  //selection_sort(A,n);
  //bubble_sort(A,n);
  //heap_sort(A,n);
  quick_sort(A,n);
  dump_array(A,n);
  
  return 0;
}
