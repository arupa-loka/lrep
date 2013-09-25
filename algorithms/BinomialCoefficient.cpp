#include "BinomialCoefficient.hpp"
#include "Factorial.hpp"

// bnc = factorial(n) / (factorial(n-k) * factorial(k));
double binomial_coeff(int n, int k)
{
  if (k<0 || n<k)
    return 0;
  
  double res = 1;
  if (k<n-k)
    k = n-k;
  double dn = n;
  double dk = k;
  int d = n-k;

  // k+1 ... k+n-k / 1 ... n-k =
  // Prod i=[1,n-k] (1+(k/i))
  for (int i=1; i<=d; ++i) {
    res *= 1+dk/i;
  }

  return res;
}
