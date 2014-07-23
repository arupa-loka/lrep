/* LCS - Longest Common Substring 
* between two strings.
*/

#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cstring>


void dump_array(int A[], int size)
{
  for (int i=0; i<size; ++i)
        printf("%i\t", A[i]);
  printf("\n");
  return;
}

void dump_array2(char A[], int size)
{
  for (int i=0; i<size; ++i)
        printf("%c\t", A[i]);
  printf("\n");
  return;
}

void dump_matrix(int** A, int m, int n)
{
  for(int j=0; j<n; ++j)
    printf ("\t%i",j);
  printf("\n");

  for (int i=0; i<m; ++i){
    printf ("%i\t",i);
    for(int j=0; j<n; ++j)
        printf("%i\t", A[i][j]);
    printf("\n");
  }
  printf("\n");
  return;
}

/* Find the occurrence of the first longest common substring between X and Y.
*/
void lcs(char *X, int m, char *Y, int n)
{
  int** C = new int*[m+1];
  for (int i=0; i<m+1; ++i) C[i] = new int[n+1];
  for (int i=0; i<m+1; ++i) C[i][0]=0;
  for (int j=1; j<n+1; ++j) C[0][j]=0;
  int zi=0; // lcs start
  int zl=0; // lcs length

  for (int i=1; i<m+1; ++i) {
    for (int j=1; j<n+1; ++j) {
      if (X[i-1]==Y[j-1]) {
        C[i][j] = C[i-1][j-1]+1;
        if (C[i][j]>zl) {
          zl = C[i][j];
          zi = (i-1)-(zl-1);
        }
      } else {
        C[i][j] = 0;
      }
    }
  }

  printf("\n");
  dump_matrix(C,m+1,n+1);
  if (zl>0) dump_array2(X+zi, zl);
}

/* Find the occurrence of the first longest common substring between X and Y.
You don't actually need the whole matrix m+1 * n+1, you just need the last two
rows. Don't use matrix m+1 * n+1, use just 2 * n+1.
*/
void lcs2(char *X, int m, char *Y, int n, size_t &lcs_start, size_t &lcs_len)
{
  // Init matrix to save already solved sub-problems.
  // We only need the last two row of a (m+1) * (n+1) matrix. 
  // Allocate memory.
  int** C = new int*[2];
  for (int i=0; i<2; ++i) C[i] = new int[n+1];
  // Init first column to zero.
  for (int i=0; i<2; ++i) C[i][0]=0;
  // Init first row to zero.
  for (int j=1; j<n+1; ++j) C[0][j]=0;
  // Variables to store the longest common substring found so far.
  lcs_start = 0; // lcs start
  lcs_len = 0; // lcs length

  for (int i=1; i<m+1; ++i) {
    for (int j=1; j<n+1; ++j) {
      if (X[i-1]==Y[j-1]) {
        C[1][j] = C[0][j-1]+1;
        if (C[1][j]>lcs_len) {
          lcs_len = C[1][j];
          // actual index in the X string
          // to get the index in the Y string replace i with j
          lcs_start = (i-1)-(lcs_len-1);
        }
      } else {
        C[1][j] = 0;
      }
    }
    std::swap(C[1],C[0]);
  }

}


#define MAX 6

int main(int argc, char* argv[])
{
  //int seed = 1281811359;//time(NULL);   
  //srand(seed);
  //printf("seed= %i\n",seed);

  char X[] = "xabxxcdexxx";
  char Y[] = "yyabycdeyy";
  //char X[] = "xabx";
  //char Y[] = "yaby";
  int lenx = sizeof(X)-1;
  int leny = sizeof(Y)-1;

  // -- Print input START
  int leni = std::max(lenx, leny);
  printf("lenx=%d, leny=%d, leni=%d\n", lenx, leny, leni);
  int * I = new int[leni];
  for (int i=0; i<leni; ++i) {
    I[i]=i;
    //X[i]=48+rand()%10;
    //Y[i]=48+rand()%10;
  }
  dump_array(I,leni);
  dump_array2(X,lenx);
  dump_array2(Y,leny);
  // -- Print input END

  // -- Actually find longest common substring.
  size_t lcs_start, lcs_len;
  lcs2(X, lenx, Y, leny, lcs_start, lcs_len);
  printf("\n");
  if (lcs_len>0) dump_array2(X+lcs_start, lcs_len);

  return 0;
}

