#include "Factorial.hpp"

long long int factorial(int n)
{
  if (n<0)
    return 0;
  long long int res = 1;
  while(n>1) {
    res *= n;
    --n;
  }
  return res;
}


// TODO this version has bug for k==0
int factorial(int n, int k)
{
  if (k<0 || n<k)// n<0 is implicit
    return 0;
  if (n-k==0)
    return 1;

  int res = 1;
  k = n-k;
  for (int i=n; i>=k; --i)
    res *= i;
  return res;
}
