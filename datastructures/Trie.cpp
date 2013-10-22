#include "Trie.hpp"
#include "Stack2.hpp"
#include "Pair.hpp"
#include <cstring>
#include <cstdio>
#include <iostream>

TrieNode::TrieNode(): parent_(0), value_(0), key_(0)
{
  memset(index_, 0, sizeof(TrieNode*)*53);
}

bool TrieNode::operator<(const TrieNode & b) const
{
  return (b.value_ < this->value_);
}

std::ostream& TrieNode::toStream(std::ostream& oOut)
{
    oOut << key_;
    oOut << ":";
    oOut << value_; 
    return oOut;
}

std::ostream& operator<<(std::ostream& oOut, TrieNode & iObj) { return iObj.toStream(oOut); }


Trie::Trie(): size_(0) {}

void Trie::insert(const char * iWord)
{
  TrieNode * p = &root_;
  while (*iWord!=0)
  {
    char i = hash(*iWord);
    if ( p->index_[i] ) 
    {
      p = p->index_[i];
    } 
    else 
    {
      TrieNode * node = new TrieNode();
      node->key_ = *iWord;
      node->parent_ = p;
      p->index_[i] = node;
      p = node;
      ++size_;
    } 
    ++iWord;
  }
  if (p->value_==0)
    list_.pushBack(p);
  ++(p->value_);
}

void Trie::frequencies(int iTop) const
{
  printf("\tTrie_size= %i\n", size_);
  printf("\tMemory_space= %i\n", size_*sizeof(TrieNode));
  list_.sort2();
  //std::cout << list_ << std::endl;
  
  LinkedListIterator<TrieNode*> it = list_.getIterator();
  int i = 0;
  
  for (; i<iTop && !it.end(); ++i, ++it)
  {
    const TrieNode * p = *it;
    char s[64];
    memset(s,0,64);
    char * q = s;
    int freq = p->value_; 
    
    while (p!=&root_)
    {
      *q=p->key_;
      ++q;
      p = p->parent_;
    }
    // we have to reverse the string before printing it
    reverse_string(s, get_string_size(s));
    printf("\t%i\t%s\n", freq, s);
  }
  
}

char Trie::hash(char c)
{
  if (c>64 && c<91)
    return c-65;
  else if (c>96 && c<123)
    return c-71;
  return 52;  
}

void Trie::reverse_string(char * s, int n) const
{
  char * e = s+n-1;
  char t;

  for (; s<e; ++s, --e)
  {
    t = *s;
    *s = *e;
    *e = t;
  }

  return;
}

int Trie::get_string_size(char * s) const
{
  char * p = s;
  while(*p) 
    ++p;    
  return (p-s); 
}

int Trie::get_tree_high() const
{
  typedef Pair<const TrieNode*, int> Dub; 
  
  Stack2< Dub > stack;
  int max = 0;
  int cur = 0;
  const TrieNode* p = 0;
  
  stack.push( Dub(&root_,cur) );
  
  while( !stack.empty() )
  {
    Dub d = stack.getTop();
    stack.pop();
    p = d.first();
    cur = d.second();
    
    if (cur>max) 
    { 
      max=cur; 
    }
    
    for (int i=52; i>=0; --i)
    {
      if ( p->index_[i] ) 
      {     
        stack.push( Dub(p->index_[i], cur+1) );
      }
    }
  }
  
  return max;
}

