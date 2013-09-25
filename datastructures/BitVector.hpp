#ifndef _BIT_VECTOR_HPP
#define _BIT_VECTOR_HPP

#include <string>

class BitVector
{
	public:
	typedef unsigned long long int SLOT;
	//typedef unsigned char SLOT;
	BitVector(const int iBitSize);
	BitVector(const char * iBitString, const int iBitStringSize);
	BitVector(const BitVector& iBv);
	~BitVector();
	BitVector& operator=(const BitVector& iBitVector);
	BitVector operator^(const BitVector& iBitVector);
	BitVector operator&(const BitVector& iBitVector);
	int hammingDist(const BitVector & iBitVector);
	int dummyHammingDist(const BitVector & iBitVector);
	bool isZero() const;

	std::ostream& toStream(std::ostream& oOut) const;
	inline int getSlotSize() const { return _slotSize; }
	inline int getBitSize() const { return _bitSize; }
	inline void* getBitVector() const { return _bitVector; } 
	
	private:
	void init_space(const int iBitSize);

	void * _bitVector;
	int _slotSize;
	int _bitSize;
	
};

std::ostream& operator<<(std::ostream& oOut, const BitVector & iObj);

#endif

