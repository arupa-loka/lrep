#ifndef __TRIE_HPP
#define __TRIE_HPP

#include "LinkedList.hpp"

struct TrieNode
{
	TrieNode();
	bool operator<(const TrieNode & b) const; 
	std::ostream& toStream(std::ostream & oOut);

	char key_;
	int value_;
	TrieNode * parent_;
	TrieNode * index_[53];
	
};

class Trie
{
	public:
	Trie();
	void insert(const char * iWord);
	void frequencies(int iTop) const;
	int get_tree_high() const;

	private:
	char hash(char c);
	void reverse_string(char * s, int n) const;
	int get_string_size(char * s) const;

	TrieNode root_;
	mutable LinkedList<TrieNode*> list_;
	int size_;
};

#endif

