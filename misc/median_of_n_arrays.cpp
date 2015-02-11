#include <iostream>
#include <cstdlib>
#include <limits>
#define N 2
#define SIZE 22
#define LAST std::numeric_limits<size_t>::max()
//using namespace std;

/* The median of a set of values is computed as follows:
Sort the values
If the number of values is odd, the median is the middle value
If the number of values is even, the median is the average of the two middle values.
TODO: still not finished.
*/

int * generate_random_array(size_t size) {
  int * array = new int[size];
  for (size_t i=0; i<size; ++i) {
    array[i] = rand() % 100;
  }
  return array;
}

template <class T>
T min(const T& a, const T& b) {
  return a < b ? a : b;
}

template<class T>
void swap(T*& a, T*& b) {
  T* tmp = a;
  a = b;
  b = tmp;
}

void print_array(const int* a, size_t n) {
  for (size_t i=0; i<n; ++i) {
    std::cout << a[i] << ", ";
  }
  std::cout << "END" << std::endl;
}

// implement merge sort
void merge_sort(int * array, size_t size) {
  if (size<2 || array==NULL) {
    return;
  }
  int * tmp = new int[size];
  int * p = array;
  //print_array(p, size);

  for (size_t len=1; len<size; len*=2) {
    size_t w = 0;
    swap(tmp, p);
    for (size_t pos=0; pos<size; pos+=2*len) {
      size_t l = pos;
      size_t r = pos+len;
      size_t l_end = min(size, pos+len);
      size_t r_end = min(size, pos+2*len);
      //printf("len=%lu, l=%lu, l_end=%lu, r=%lu, r_end=%lu\n",
      //        len, l, l_end, r, r_end);
      while(l<l_end && r<r_end) {
        // TODO would the algorithm be stable using <= instead of < ?
        if (tmp[l] < tmp[r]) {
          p[w++] = tmp[l++];
        } else {
          p[w++] = tmp[r++];
        }
      }
      while (l<l_end) {
        p[w++] = tmp[l++];
      }
      while (r<r_end) {
        p[w++] = tmp[r++];
      }
    }
    //print_array(p, size);
  }
  if (p!=array) {
    memcpy(array, p, sizeof(int)*size);
    tmp = p;
  }
  delete[] tmp;
}

size_t binary_search(int key, int array[], size_t n) {
  size_t left=0;
  size_t right=n-1;
  size_t i = ((right - left)>>1) + left;
  while (left<=right) {
    if (array[i] < key) {
      left = i+1;
    } else if (key < array[i]) {
      right = i-1;
    } else { // key == array[i]
      return i;
    }
    i = ((right - left)>>1) + left;
  }
  return LAST;
}

// Returns the index of the first value from the sorted array of size n
// that does not compare < than key. That means the index of the first value
// that is >= to key.
// If all the elements in the array compare < than key, then return MAX_INT.
// If all the elemnts compare >= than key then return 0.
size_t lower_bound(int key, int array[], size_t n) {
  int left = 0;
  int right = n - 1;
  int i = ((right - left) >> 1) + left;
  while (left <= right) {
    if (array[i] < key) {
      left = i + 1;
    } else {
      right = i - 1;
    }
    i = ((right - left) >> 1) + left;
  }
  if (left < n) {
    return left;
  } else {
    return LAST;
  }
}


bool is_odd(size_t n) {
  return n & 1;
}

/* Select random value among the ones available. The value is selected from one
of those available over a number of arrays "n_arrays" stored in "array[]" of
size "size" each. "array_interval" size is equal to "size"*2, as it contains
the interval (left, right), of the values still available in each array.
*/
int select_random_value(int * array[], size_t n_arrays, size_t size,
                        size_t * array_interval) {
  size_t idx1;
  do {
    // multiplied by 2 as the index is going to be used in array_interval where
    // every two consecutive elements are the couple of number left, right
    // associated with the corresponding array:
    // array_interval[0] = left of array[0]
    // array_interval[1] = right of array[0]
    // array_interval[2] = left of array[1]
    // array_interval[3] = right of array[1] ... etc...
    // if left>right means the interval on the corresponding array has been
    // exhausted.
    idx1 = (rand() % n_arrays)*2;
    //printf("index 1 selected: %lu\n", idx1);
  } while (array_interval[idx1] > array_interval[idx1+1]);
  // now that we found an array with some values still available, select a
  // random value in [left, right].
  // X X X X L Y Y Y R X X X then return one of the values L Y Y Y R.
  size_t idx2 = array_interval[idx1] + rand() %
                (array_interval[idx1+1]-array_interval[idx1]);
  //printf("index 2 selected: %lu\n", idx2);
  // divide idx1 by 2, as the size of array is n_arrays not n_arrays*2.
  return array[idx1/2][idx2];
}

// Find median of N arrays of the same size.
// Slow convergence:
// - if the n_arrays contain the same repeated element, then the lower_bound
// search will return the first element of each array. That will be handled by
// the minor < greater case, then
// dropping the single element empty-minor-side of each array will make the
// algorithm converge slowly.
// TODO: still does not work for cases like:
// - if the number of elements is even, the median is the average of two
// numbers, while this algo works only with an odd number of values.
float median_of_n_arrays(int * array[], size_t n_arrays, size_t size) {
  size_t * array_interval = new size_t[n_arrays*2];
  for (size_t i=0; i<n_arrays*2; i+=2) {
    array_interval[i] = 0;
    array_interval[i+1] = size-1;
  }
  size_t * binary_search_results = new size_t[n_arrays];
  while (true) {
    int random_value = select_random_value(array, n_arrays, size, array_interval);
    printf("random_value=%d\n", random_value);
    size_t minor=0;
    size_t greater =0;
    for (size_t i = 0; i < n_arrays; ++i) {
      binary_search_results[i] = lower_bound(random_value, array[i], size);
      if (binary_search_results[i] == LAST) {
        binary_search_results[i] = size;
      }
      minor += binary_search_results[i];
      greater += size - binary_search_results[i];
      //printf("left=%lu, right=%lu\n",
      //  binary_search_results[i], size-binary_search_results[i]);
    }
    printf("minor=%lu, greater=%lu\n", minor, greater);
    if (minor > greater ) {
      // the median is smaller than random_value
      // then drop the greater-than side of each array
      for (size_t i = 0; i < n_arrays; ++i) {
        array_interval[i*2+1] = binary_search_results[i]-1;
      }
    } else if (minor < greater) {
      // the median is greater than random_value
      // then drop the less-than side of each array
      for (size_t i = 0; i < n_arrays; ++i) {
        size_t idx = binary_search_results[i];
        if (array[i][idx] == random_value) {
          idx += 1;
        }
        array_interval[i*2] = idx;
      }
    } else {
      // median is random_value, end here.
      return random_value;
    }
    for (int i = 0; i < n_arrays; ++i) {
      printf("%lu - %lu: ", array_interval[i*2], array_interval[i*2+1]);
      for (int j = array_interval[i*2]; j <= array_interval[i*2+1]; ++j ) {
        printf("%d, ", array[i][j]);
      }
      printf("\n");
    }

  }
  delete[] binary_search_results;
  delete[] array_interval;
  return 0.0;
}

int main(int argc, char * argv[]) {
  int ** array = new int*[N];
  srand (1234);
  for(int i = 0; i < N; ++i) {
    array[i] = generate_random_array(SIZE);
    merge_sort(array[i], SIZE);
    print_array(array[i], SIZE);
  }

  //printf("Search 7: %lu\n", binary_search(7, array[0], SIZE));
  //printf("Search 49: %lu\n", binary_search(49, array[0], SIZE));
  //printf("Search 11: %lu\n", binary_search(11, array[0], SIZE));
  //printf("Search 100: %lu\n", binary_search(100, array[0], SIZE));

  //printf("Search 7: %lu\n", lower_bound(0, array[0], SIZE));
  //printf("Search 7: %lu\n", lower_bound(7, array[0], SIZE));
  //printf("Search 49: %lu\n", lower_bound(49, array[0], SIZE));
  //printf("Search 11: %lu\n", lower_bound(11, array[0], SIZE));
  //printf("Search 100: %lu\n", lower_bound(100, array[0], SIZE));
  //return 0;
  float median = median_of_n_arrays(array, N, SIZE);
  printf("median=%.2f\n", median);
  //for (int i=0; i<N; ++i) {
  //  delete[] array[i];
  //}
  delete[] array;
  return 0;
}
