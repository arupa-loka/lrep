// Longest Increasing Subsequence

#include <cstring>
#include <cstdio>

void dump_array(char A[], int size)
{
  for (int i=0; i<size; ++i)
        printf("%c\t", A[i]);
  printf("\n");
  return;
}

void dump_array(int A[], int size)
{
  for (int i=0; i<size; ++i)
        printf("%i\t", A[i]);
  printf("\n");
  return;
}

int binarySearch(int X[], int M[], int L, int i)
{
  int l=1;
  int r=L;

  while(l<r)
  {
    int p= ((r-l)>>1) + l;
    if (X[i]<X[M[p]])
          r=p-1;
    else
          l=p+1;
  }

  if (l<=L && X[i] >= X[M[l]])
  //if ( X[i] >= X[M[l]])
        return l;
  else
        return l-1;
}

// Longest Increasing Subsequence
void LIS(int X[], int m)
{
  int* M = new int[m+1];
  memset(M, 0, sizeof(int)*(m+1));
  int* P = new int[m];
  memset(P, 0, sizeof(int)*m);

  M[0]=-1;
  int L = 0;

  for(int i=0; i<m; ++i)
  {
    int j= binarySearch(X,M,L,i);
    P[i]=M[j];

    if ( j==L || X[i]<X[M[j+1]] )
    {
      M[j+1]=i;
      if (j+1>L) L=j+1;
    }
  }

  if (L>0)
  {
    int * R = new int[L];
    int k = M[L];
    for (int i=L-1; i>=0; --i)
    {
      R[i]=X[k];
      k=P[k];
    }

    dump_array(R,L);
    printf("L= %i\n",L);
    delete[] R;
  }

  delete[] M;
  delete[] P;
}

int main(int argc, char* argv[])
{
  //int seed = 1281811359;//time(NULL);   
  //srand(seed);
  //printf("seed= %i\n",seed);

  int X[] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};
  //int lenx = sizeof(X)-1;
  int lenx = 16;
  // THE LIS is: 0, 2, 6, 9, 13, 15.

  dump_array(X,lenx);

  LIS(X,lenx);
  printf("\n");
  return 0;
}
