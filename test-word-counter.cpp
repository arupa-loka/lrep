#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "FileReader.hpp"
#include "Trie.hpp"

int main(int argc, char *argv[] )
{
  int word_size = 64;
  char word[64];
  int freq;
  const char * file = 0;
  
  if (argc < 3)
  {
    printf("Usage: %s <file> <number_of_top_freq>\n", argv[0]);
    printf("Example: %s ./data/pg2489.txt 20\n", argv[0]);
    return -1;
  }     
  file = argv[1];
  freq = atoi(argv[2]);
  
  FileReader aFR;
  Trie trie;

  if (!aFR.open(file))
    return 0;
    
  while (aFR.getWord(word, word_size))
  {
    //printf("%s\n", word);
    trie.insert(word);
  }
  aFR.close();

  trie.frequencies(freq);
  
  printf("Trie high: %i\n", trie.get_tree_high() );

  return 0;
}

