#include "Vector.hpp"
#include "Heap.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

#define MAX 17

struct my_struct {
  int a;
  int b;
};

void test_1() {
  Vector<int> v;
  v.realloc(MAX);
  Heap<int> h;
  //int seed = 1282566146; 
  int seed = time(NULL);
  printf("seed= %i\n",seed);
  srand(seed);
  
  for (int i=0; i<MAX; ++i)
  {
    int a = rand() % 100;
    v.pushBack(a);
    h.push(a);
  }
  cout << "Print vector: " << endl;
  cout << v << endl;

  cout << "Test Max Heap:" << endl;
  cout << h << endl;
  while (h.size())
  {
    h.pop();
    cout << h << endl;
  }
  cout << endl;

  cout << "Test operator=() :" << endl;
  Vector<int> v2(1);
  v2 = v;
  cout << v << endl;
  cout << v2 << endl;

  cout << "Test copy constructor:" << endl;
  Vector<int> v3(v);
  cout << v3 << endl;

  cout << "Test operator[]: " << endl;
  Vector<int> v4(10);
  for (size_t i = 0; i<v4.size(); ++i)
    v4[i] = (int)i;
  cout << v4 << endl;


}

void test_2() {
  Vector< my_struct > v;
  for (int i = 0; i<32; ++i) {
    my_struct ms;
    ms.a = i;
    ms.b = i;
    v.pushBack(ms);
    // this is a ref to something allocated dinamically
    my_struct & ms2 = v[i];
    // the push back now delete that object ms2,
    // because allocaty/copy/delete the vector memory
    // and after deletion tries to use ms2 again!!!
    v.pushBack(ms2);
  }
}

int main()
{
  test_2();
  printf("END\n");
  return 0;
}

