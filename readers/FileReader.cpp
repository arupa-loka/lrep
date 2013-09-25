#include "FileReader.hpp"
FileReader::FileReader(): fd_(0)
{

}

int FileReader::open(const char * iFileName)
{
  this->close();

  fd_ = fopen(iFileName, "r");
  if (!fd_) {
    perror("Error in fopen");
    return -1;
  }
  memset(buff_, 0, BUFF_SIZE );

  word_ = 0;
  p_ = 0;
  end_  = 0;
  word_size_ = 0;
  c_ = 0;
}

int FileReader::getWord(char * iWord, int iWordSize )
{
  do {

    while (p_<end_)
    {
      if ( isNotAlpha(*p_) ) 
      {
        if (word_size_>0)
        {
          if (iWordSize>word_size_) 
          {
            strncpy(iWord, word_, word_size_);
            iWord[word_size_]=0;
            iWordSize=word_size_;
            //if (word_size_>19)
            //{
            //  printf("size>19 %s\n", iWord);
            //}
          }
          else
          {
            strncpy(iWord, word_, iWordSize);
            iWord[iWordSize]=0;
          }
          word_size_=0;
          ++p_;
          word_=p_;
          return (iWordSize);
        }  
        else
        {
          ++word_;
          ++p_;
        }
      } 
      else 
      {
        ++word_size_;
        ++p_;
      }
    }
  
    if (word_size_>0)
    {
      p_ = buff_;
      int n = word_size_;
    
      do {
        *p_ = *word_;
        ++p_;
        ++word_;
        --n;
      } while (n>0);
    }

    c_ = fread(buff_+word_size_, 1, BUFF_SIZE-word_size_, fd_); 
    word_ = buff_;
    p_ = buff_+word_size_;
    end_ = p_+c_;
  
  } while ( c_ );

  return 0;
}

void FileReader::close()
{
  if (fd_)
    fclose(fd_);
}

bool FileReader::isNotAlpha(char c)
{
  return ((c>64 && c<91) || (c>96 && c<123) ? false : true);
}

bool FileReader::isAlpha(char c)
{
  return ((c>64 && c<91) || (c>96 && c<123) ? true : false);
}

char FileReader::toLower(char c)
{
  return (c>64 && c<91  ? c+32 : c);  
}
