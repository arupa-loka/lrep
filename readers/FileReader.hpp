#ifndef __FILE_READER_HPP
#define __FILE_READER_HPP

#include <stdio.h>
#include <string.h>

#define BUFF_SIZE 1024

class FileReader
{
	public:
	FileReader();
	int open(const char* iFileName);
	int getWord(char * iWord, int iWordSize);
	void close();

	private:
 	bool isNotAlpha(char c);
	bool isAlpha(char c);
	char toLower(char c);

	
	FILE* fd_;
	char buff_[BUFF_SIZE];

	char * word_;
	char * p_;
	char * end_;
	int word_size_;
	int c_;
	
};

#endif
