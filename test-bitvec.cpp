#include "BitVector.hpp"
#include "DoublyLinkedList.hpp"
#include "BinomialCoefficient.hpp"
#include "Factorial.hpp"
#include <fstream>
#include <iostream>
#include <cstdio>
#include <cmath>

#define MAXLINE 12000

class HammingValue {

	public:
	HammingValue(int iLineA, int iLineB, int iHamm):
		_line_id_A(iLineA), _line_id_B(iLineB), _hamming_dist(iHamm) {}
	
  std::ostream& toStream(std::ostream& oOut) const {
		oOut << _line_id_A;
		oOut << "\t";
		oOut << _line_id_B;
		oOut << "\t";
		oOut << _hamming_dist;
		oOut << std::endl;
		return oOut;
	}
	int _line_id_A;
	int _line_id_B;
	int _hamming_dist;
	bool operator<(HammingValue& iHamm) {
		return (this->_hamming_dist < iHamm._hamming_dist);
	}
};

std::ostream& operator<<(std::ostream& oOut, const HammingValue & iObj) { return iObj.toStream(oOut); };

int compute_probs(double p, int n)
{
  double q=1-p;
  double ptot = 0.0;
  for (int k=0; k<=n; ++k) {
    double res = pow(p,k) * pow(q,n-k) * binomial_coeff(n,k);
    //double res = binomial_coeff(n,k);
    std::cout << k << " - " << res << std::endl;
    ptot+=res;
  }
  std::cout <<"ptot = " << ptot << std::endl;
  std::cout <<"mean = " << n*p << std::endl;
  return 1;
}

int main()
{
  (void)compute_probs(0.8, 500);
  return 0;
}

int main2()
{
	//unsigned long long int t1 = 0x0807060504030201;
	//char * p = NULL;
	//p = (char*)(&t1);
	//for (int i=0; i<sizeof(unsigned long long int); ++i) {
	//	printf("%i -> %x\n", i, *(p+i));
	//}

	std::FILE * aFile = NULL;
	char aLine[MAXLINE];
	int aLineNumber=0;
	int aLineSize=0;
	char iFileName[] = "./data/bitvectors-genes.data.small";
	int aChar;
	
	aFile = fopen(iFileName, "r");
	
	if(aFile==NULL) {
		printf("Error: in file opening %s\n", iFileName);
		return -1;
	}
	
	//-- get line size
	do {
		aChar = fgetc(aFile);
		++aLineSize;
	} while ( aChar!=EOF && aChar!='\n' );
	--aLineSize;
	//-- reset the file seek
	fseek(aFile, 0, SEEK_SET);
	//-- dump the expected line size
	std::cout << "aLineSize=" << aLineSize << std::endl;
	
	if( MAXLINE<aLineSize ) {
		printf("Error: lines too long in %s\n", iFileName);
		return -1;
	}

	int aCount;
	DoublyLinkedList<BitVector*> aListOfBitVector;
	DoublyLinkedList<HammingValue*> aListOfHammingValue;
	
	while( (aCount = fread(aLine, 1, aLineSize, aFile)) )//&& aLineNumber<1 )
	{
		if (aCount != aLineSize) {
			printf( "Error: something wrong while reading %s,"
					"expected_line_size=%d, read line_size=%d\n", 
					iFileName, aLineSize, aCount);
			return -1;
		}
		
		aChar = fgetc(aFile);
		
		if (aChar == '\n') {
			;// do nothing
		} else if (aChar == EOF) {
			break;
		} else {
			printf("Error: wrong line size in file %s\n", iFileName);
			return -1;
		}

		BitVector* aBv = new BitVector(aLine,aLineSize);
		
		int aLineIdxInList=0;
		for (DoublyLinkedListIterator<BitVector*> aIt1 = aListOfBitVector.getIterator(); !aIt1.end(); ++aIt1)
		{
			BitVector * aBv2 = *aIt1;
			//int aHammingDist = aBv->hammingDist(*aBv2);
			int aHammingDist = aBv->dummyHammingDist(*aBv2);
			aListOfHammingValue.pushBack( new HammingValue(aLineNumber, aLineIdxInList, aHammingDist) );
			std::cout << aHammingDist << std::endl;
			++aLineIdxInList;
		}
		
		aListOfBitVector.pushBack(aBv);
		++aLineNumber;
	}

	// remove debug from sort3
	aListOfHammingValue.sort3();
	std::cout << aListOfHammingValue << std::endl;

	std::cout << "aListOfBitVector size: " << aListOfBitVector.size() << std::endl;

	printf("END\n");
	return 0;
}

