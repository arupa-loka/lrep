#include "BitVector.hpp"
#include <iostream>
#include <assert.h>
#include <stdio.h>
#include <string.h>


int BitVector::dummyHammingDist(const BitVector & iBv)
{	
	int aHammingDist=0;
	void * aVec = iBv.getBitVector();
	int aSlotSize = iBv.getSlotSize();
	int aBitSize = iBv.getBitSize();


	for (int aSlotIdx=0, aBitIdx=0; aSlotIdx<_slotSize; ++aSlotIdx)
	{
		SLOT * aSlot =  (((SLOT*)_bitVector)+aSlotIdx);
		SLOT * aSlot02 =  (((SLOT*)aVec)+aSlotIdx);
		
		//-- just to get a more readable output format
		//if (aSlotIdx>0 && aBitIdx<_bitSize) 
		//{ 
		//oOut << std::endl; 
		//}	

		for (int aBitIdx02 = 0;  
			 aBitIdx02<(sizeof(SLOT)*8) && aBitIdx<_bitSize; 
			 ++aBitIdx, ++aBitIdx02)
		{
			short aBitValue   = 0;
			short aBitValue02 = 0;
			aBitValue = (*aSlot << aBitIdx02) >> ((sizeof(SLOT)*8)-1);
			aBitValue02 = (*aSlot02 << aBitIdx02) >> ((sizeof(SLOT)*8)-1);
			if (aBitValue != aBitValue02)
				++aHammingDist;
		}
	}
	return aHammingDist;

}

int BitVector::hammingDist(const BitVector & iBitVector)
{
	int aHammingDist = 0;
	BitVector aTmp(0);//_bitSize);
	aTmp = *this ^ iBitVector; //-- XOR
	
	//std::cout << "XOR " << aTmp << std::endl;
	
	while ( !aTmp.isZero() )
	{	
		BitVector aTmp02(0);
		aTmp02=aTmp;
		
		void * aVec = aTmp02.getBitVector();
		int aSlotSize = aTmp02.getSlotSize();
		int aBitSize = aTmp02.getBitSize();

		// aTmp &= aTmp - 1;
		for (int i=aSlotSize-1; i>=0; --i)
		{
			SLOT* aSlot = (((SLOT*)aVec)+i);
			if (*aSlot>0)
			{
				--(*aSlot);
				//-- reset to zero the last not used bits
				if (i==aSlotSize-1)
				{
					*aSlot = ( (*aSlot) >> ((sizeof(SLOT)*8)-(aBitSize % (sizeof(SLOT)*8))) ) << ((sizeof(SLOT)*8)-(aBitSize % (sizeof(SLOT)*8)));
				}
				else 
				//if (i<_slotSize-1)
				{
					for (int j=i+1; j<aSlotSize; ++j)
					{
						SLOT* aSlot02 = (((SLOT*)aVec)+j);
						*aSlot02 = !((SLOT)0);
						if (j==aSlotSize-1)
						{
							//--(*aSlot02);
							*aSlot02 = ( (*aSlot) >> ((sizeof(SLOT)*8)-(aBitSize % (sizeof(SLOT)*8))+1) ) << ((sizeof(SLOT)*8)-(aBitSize % (sizeof(SLOT)*8))+1); 
						}
					}
				}
				break;
			}
		}
		
		aTmp = aTmp02 & aTmp;
		++aHammingDist;
	}

	return aHammingDist;
}
 
bool BitVector::isZero() const 
{
	for (int i=0; i<_slotSize; ++i)
	{
		if ( *(((SLOT*)_bitVector)+i) > 0)
			return false;
	}
	return true;
}

BitVector BitVector::operator^(const BitVector& iBV)
{	
	BitVector aTmp(_bitSize);
	
	if (_slotSize==iBV.getSlotSize() && _bitSize==iBV.getBitSize() )
	{	
		for(int i=0; i<_slotSize; ++i)
		{
			SLOT * aSource01 = ((SLOT*)_bitVector)+i;
			SLOT * aSource02 = ((SLOT*)iBV.getBitVector())+i;
			SLOT * aDest     = ((SLOT*)aTmp.getBitVector())+i;
			*aDest = (*aSource01) ^ (*aSource02);
			//std::cout << "XOR " << i << ") " << *aDest << std::endl;
		}
	}
	
	//std::cout << "XOR glob " << aTmp << std::endl;
	return aTmp;
}

BitVector BitVector::operator&(const BitVector& iBV)
{	
	BitVector aTmp(_bitSize);
	
	if (_slotSize==iBV.getSlotSize() && _bitSize==iBV.getBitSize() )
	{	
		for(int i=0; i<_slotSize; ++i)
		{
			SLOT * aSource01 = ((SLOT*)_bitVector)+i;
			SLOT * aSource02 = ((SLOT*)iBV.getBitVector())+i;
			SLOT * aDest     = ((SLOT*)aTmp.getBitVector())+i;
			*aDest = (*aSource01) & (*aSource02);
			//std::cout << "XOR " << i << ") " << *aDest << std::endl;
		}
	}
	
	//std::cout << "AND glob " << aTmp << std::endl;
	return aTmp;
}

BitVector& BitVector::operator=(const BitVector& iBV)
{
	if (this==&iBV)
	{
		return *this;
	}
	
	//std::cout << "op=(a) " << iBV << std::endl;
	//std::cout << "op=(b) " << *this << std::endl;

	if ( _slotSize==iBV.getSlotSize() )
	{
		//strncpy((char*)_bitVector, (char*)iBV.getBitVector(), (_slotSize*sizeof(SLOT))+1 );
		for (int i=0; i<_slotSize; i++)
		{
			*(((SLOT*)_bitVector)+i) = *(((SLOT*)iBV.getBitVector())+i);
		}
		_bitSize=iBV.getBitSize();
	}
	else
	{
		if (_bitVector!=NULL) {
			//free(_bitVector);
		  delete[] (SLOT*)_bitVector;
    }
		init_space(iBV.getBitSize());
		//strncpy((char*)_bitVector, (char*)iBV.getBitVector(), _slotSize*sizeof(SLOT));
		for (int i=0; i<_slotSize; i++)
		{
			*(((SLOT*)_bitVector)+i) = *(((SLOT*)iBV.getBitVector())+i);
		}
	}
	//std::cout << "op= " << *this << std::endl;
	
	return *this;
}

BitVector::BitVector(const BitVector& iBv):
	_slotSize(0), _bitSize(0), _bitVector(NULL)
{
	*this = iBv;
};


BitVector::BitVector(const int iBitSize)//:
	//_slotSize(0), _byteSize(0), _bitSize(0), _bitVector(NULL)
{
	init_space(iBitSize);
};


BitVector::BitVector(const char * iBitString, const int iBitStringSize)
{
	init_space(iBitStringSize);
	assert(_bitSize==iBitStringSize);

	int aSlotNumber=0;
	//int aSlotNumber=_slotSize-1;
	int aBitInSlot=0;

	for(int i=0; i<iBitStringSize; ++i)
	{
		if (iBitString[i]=='1')
		{
			SLOT * aSlot = ((SLOT*)_bitVector)+aSlotNumber;
			*aSlot = (*aSlot) | ((((SLOT)1) << (sizeof(SLOT)*8-1)) >> aBitInSlot);
		}
		else if (iBitString[i]=='0')
		{
			// do nothing
		}
		else
		{
			printf("Error: BitVector input character is not 0 or 1");
			// error
		}

		if (++aBitInSlot>=sizeof(SLOT)*8) {
			aBitInSlot=0;
			++aSlotNumber;
			//--aSlotNumber;
		}
		//std::cout << *this << std::endl;		
	}
	//std::cout << std::endl;		
};

std::ostream& BitVector::toStream(std::ostream& oOut) const{
	
	oOut << "BitVector: _bitSize=";
	oOut << _bitSize;
	oOut << ", _val=" << std::endl;
	
	for (int aSlotIdx=0, aBitIdx=0; aSlotIdx<_slotSize; ++aSlotIdx)
	//for (int aSlotIdx=_slotSize-1, aBitIdx=0; aSlotIdx>=0; --aSlotIdx)
	{
		SLOT * aSlot =  (((SLOT*)_bitVector)+aSlotIdx);
		
		//-- just to get a more readable output format
		//if (aSlotIdx>0 && aBitIdx<_bitSize) 
		//{ 
		//oOut << std::endl; 
		//}	

		for (int aBitIdx02 = 0;  
			 aBitIdx02<(sizeof(SLOT)*8) && aBitIdx<_bitSize; 
			 ++aBitIdx, ++aBitIdx02)
		{
			short aBitValue = 0;
			aBitValue = (*aSlot << aBitIdx02) >> ((sizeof(SLOT)*8)-1);
			oOut <<	aBitValue;	
		}
	}
	oOut << std::endl;
	return oOut;
}


void BitVector::init_space(const int iBitSize)
{
	_slotSize=0; _bitSize=0; _bitVector=NULL;

	if (iBitSize>0) 
	{
		_bitSize = iBitSize;
		_slotSize = _bitSize / (sizeof(SLOT) * 8);  
		
		if ( _bitSize % (sizeof(SLOT)*8) > 0)
		{
			++_slotSize;
		}

		//_bitVector = malloc(sizeof(SLOT)*_slotSize);
		_bitVector = new SLOT[_slotSize];
		memset(_bitVector, 0, _slotSize * sizeof(SLOT));

		//std::cout << "BitVector: allocated_slots=" << _slotSize << std::endl; 
	}

};

BitVector::~BitVector() 
{
	if (_bitVector!=NULL) {
		//free(_bitVector);
		delete[] (SLOT*)_bitVector;
  }
	//std::cout << "BitVector destroyed" << std::endl;	
};

std::ostream& operator<<(std::ostream& oOut, const BitVector & iObj) { return iObj.toStream(oOut); };

