#ifndef __B_TREE_HPP
#define __B_TREE_HPP
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <iostream>
#include <fstream>
#include "Stack2.hpp"
#include "Pair.hpp"

template < typename T >
struct BTreeNode
{
	BTreeNode(int iOrder);	  
	~BTreeNode();
	bool find(const T& k, BTreeNode<T>*& c);
    bool find_idx(const T& k, int & i);
	bool insert(T k, BTreeNode<T>* c=0);
	bool insert(T k, BTreeNode<T>* a, BTreeNode<T>* b);
    bool remove(const T& k, int & i, BTreeNode<T>*& c);
    void append( const BTreeNode<T>* p, int & n);
	
	T * key_;              // array containing the keys
	BTreeNode<T>** child_; // array containing the children
	
	bool leaf_;
	int size_;
	int order_; // max_size
	
	private:
	BTreeNode();
};

template < typename T >
BTreeNode<T>::BTreeNode(int iOrder) : 
		  key_(0), 
		  child_(0),
		  leaf_(true),
		  size_(0),
		  order_(iOrder)
{
	key_ = new T[order_];
	memset(key_, 0, sizeof(T)*(order_));
	child_ = new BTreeNode<T>*[order_+1];
	memset(child_, 0, sizeof(BTreeNode<T>*)*(order_+1));
}

template < typename T >
BTreeNode<T>::~BTreeNode() 
{
	if (key_) {
		delete[] key_;
		key_ = 0;
	}

	if (child_) {
		// delete the vector of pointers to children
		// but does not deallocate the children nodes
		delete[] child_;
		child_=0;
	}
}				 

/* k is the new key to insert
 * c is the new child node with keys > k that will be
 *   appended as right child of the new key (if c!=NULL)
 */
template < typename T >
bool BTreeNode<T>::insert(T k, BTreeNode<T>* c)
{
	// insert the key in the right place
	// if c!=0 add it as child pointer
	int i = 0;
	
    if (find_idx(k,i))
		return false; // k== key_[i] no insert, the key already exists

	for ( int j = size_-1; j>=i; --j)
		key_[j+1] = key_[j];
	key_[i]=k;

	if (c) // && !leaf_) 
	{
		for ( int j = size_; j>i; --j)
			child_[j+1] = child_[j];
		child_[i+1]=c;
	}

	++size_;

	return true; // the key has been inserted

}

/* this method is called only to initialize new root nodes
 * where we have just one key and two children
 * So we check at the beginning if the size of the node
 * is zero
 */
template < typename T >
bool BTreeNode<T>::insert(T k, BTreeNode<T>* a, BTreeNode<T>* b)
{
	if (!size_)
	{
		key_[0] = k;
		child_[0] = a;
		child_[1] = b;
		++size_;
		return true;
	}
	return false;
}

/* the remove method applies on a node that cannot go in underflow
 * removing one of the keys.
 */
template < typename T >
bool BTreeNode<T>::remove(const T& k, int & i, BTreeNode<T>*& c)
{
    c = 0;
	i = -1;
    if (find_idx(k,i)) {
        if (!leaf_) {
            c = child_[i];
        } else { // is a leaf node
            for (int j=i; j<size_-1; ++j)
                key_[j] = key_[j+1];
            --size_;
        }
        return true;
    } else {
        c = child_[i];
        return false;
    }
        
}

/* find the index i where the key_[i] is greater or equal
 * to k.
 * return true if key_[i]==k
 * return false if key_[i]>k or i==size_ (that means continue search in child_[i])
 * TODO if the size of the node reaches a certain threshold,
 * binary search should be performed instead of sequential search
 */
template < typename T >
bool BTreeNode<T>::find_idx( const T& k, int & i)
{	  
	i = 0;
	
	while( i < size_ &&  key_[i] < k )
		++i;		  

	if ( i < size_ && !(k < key_[i]) )
		return true; // k == key_[i]
    else
        return false;
}

/* Looks for the input key k in the node. 
 * If the node is an internal node:
 * - return true if the key matches one existing key in the node
 * - return false and c!=NULL if the key does not match any key in the node.
 * If the node is a leaf:
 * - return true if the key matches one existing key in the node
 * - return false and c=0 otherwise.
 *
 */
template < typename T >
bool BTreeNode<T>::find( const T& k, BTreeNode<T>*& c)
{	  
	int i;
	c = 0;
	
    if (find_idx(k,i))
		return true; // k == key_[i]

	if (!leaf_) 
		c = child_[i];

	return false;
}

/* p->size_ must be > 0
 */
template < typename T >
void BTreeNode<T>::append( const BTreeNode<T>* p, int & n) {
    
    n = size_;
    int i = size_+1;
    int j = 0;
    for (; j<p->size_; ++i, ++j ) {
        key_[i] = p->key_[j];
        child_[i] = p->child_[j];
    }
    child_[i] = p->child_[j];
    size_ += 1+p->size_;
}

template < typename T >
class BTree
{
	public:
	BTree(int iOrder);
	~BTree();
	bool insert(T k);
	bool remove(const T & k);
	bool find(T k);
	void toGraphViz(const char* iFilePath);

	
	private:
	
	BTree(); // we have to define the tree order
    // split only inner nodes
	BTreeNode<T>* split(BTreeNode<T>* a, T& median_key);
    // split only leaf nodes
	BTreeNode<T>* split(BTreeNode<T>* a, T& median_key, const T &current_key, bool &mk_is_k);
    // replace removed key
    void replace_removed_key( BTreeNode<T>*& p, int & i);
    // rebalance child node
    void rebalance_child_node( BTreeNode<T>* p, int i);
			  
	BTreeNode<T>* root_;
	int order_;
    int lb_;
	
};


template < typename T >
BTree<T>::BTree(int iOrder): 
		  order_(iOrder), 
		  root_(0),
          lb_((order_ & 1) ? (order_>>1) : (order_>>1)-1)
{
	root_ = new BTreeNode<T>(order_);
}

/* Deallocate the whole tree in post-order way.
 * TODO what happened if you are destroying an
 * empty btree??? root_ is null?
 */
template < typename T >
BTree<T>::~BTree()
{
	//delete tree in post-order
	std::cout << "~BTree()" << std::endl;
	typedef BTreeNode<T> Node;
	typedef Pair< Node*, int > Pair;
	
	Stack2< Pair > stack;

    if(root_)
	    stack.push( Pair(root_, 0) );

	int lh = -1; // high of the last visited node
	
	while( !stack.empty() ) {
		Pair pair = stack.getTop();
		Node * p = pair.first();
		int h = pair.second();
		bool hasChildren = false;

		if (lh>h || p->leaf_) {
			stack.pop();
			// visit(p)
			lh=h;
			std::cout << "delete node: " << p << std::endl;
			delete p;
			continue;
		}
		
    	for (int i=p->size_; i>=0; --i) {
    		if (p->child_[i])
    			stack.push( Pair(p->child_[i], h+1) );
	    }
	}
}

template < typename T >
BTreeNode<T>* BTree<T>::split(BTreeNode<T>* a, T& median_key)
{
	BTreeNode<T>* b = 0;
	
	if ( a && a->size_ == order_ )
	{
		int mi = a->size_ >> 1;	// median_index  
		median_key = a->key_[mi];

		// create new leaf node
		b = new BTreeNode<T>(order_);
		b->leaf_ = a->leaf_;

		copy( a->key_+mi+1, a->key_+a->size_, b->key_ );
		copy( a->child_+mi+1, a->child_+a->size_+1, b->child_ );

		b->size_ = a->size_-mi-1;
		a->size_ = mi;
	}

	return b;
}

template < typename T >
BTreeNode<T>* BTree<T>::split(BTreeNode<T>* a, T& median_key, const T& current_key, bool &mk_is_k)
{
	BTreeNode<T>* b = 0;
    mk_is_k = false;
	
	if ( a && a->size_ == order_ )
	{
        // create new leaf node
		b = new BTreeNode<T>(order_);
		b->leaf_ = a->leaf_;

		int mi = a->size_ >> 1;	// median_index  
		median_key = a->key_[mi];
        
        // if size_ is even AND ...
        // means that left child has already one key more than right child
        // and it will include also the new current_key
        if ( !(a->size_ & 1) && (current_key<median_key) ) 
        {
            if (current_key<a->key_[mi-1]) {
                // TODO check mi>0 before doing mi-1
                // New median_key is a->key_[mi-1]
                // and new mi is mi-1
                --mi;
                median_key = a->key_[mi];
            } else {
                // current_key will be the new median_key
                median_key = current_key;
                mk_is_k = true;
                // 
                copy( a->key_+mi, a->key_+a->size_, b->key_ );
		        copy( a->child_+mi, a->child_+a->size_+1, b->child_ );
	        	b->size_ = a->size_-mi;
		        a->size_ = mi;
                return b;
            }
        }

		copy( a->key_+mi+1, a->key_+a->size_, b->key_ );
		copy( a->child_+mi+1, a->child_+a->size_+1, b->child_ );

		b->size_ = a->size_-mi-1;//size_ - (mi+1)
		a->size_ = mi;
	}

	return b;
}

/* A full node is a node that have reached an amount of keys equal to order.
 * We use an improved algorithm to make insertions. It supports a single pass
 * down the tree, from the root node to the node where the insertion will take
 * place, splitting any full nodes encountered on the way. This prevents the 
 * need to recall the parent nodes into memory, which may be expensive if the 
 * nodes are on secondary storage.
 */
template < typename T >
bool BTree<T>::insert(T k)
{
	BTreeNode<T> * p = root_;
	BTreeNode<T> * c = 0;
	BTreeNode<T> * parent = 0;
	
	while (p && !p->leaf_)
	{
		if (p->find(k,c))
			return false;	  
		
		T mk;	  
		BTreeNode<T>* new_node = split(p, mk);	
		
		if (!new_node) {
			parent = p;
        } else {    
			if (parent) {		  
				parent->insert(mk, new_node);
			} else {
				BTreeNode<T>* new_root = new BTreeNode<T>(order_);
				new_root->leaf_=false;
				new_root->insert(mk, p, new_node);
				root_ = new_root;
			}

			if(k<mk)
				parent = p;
			else
				parent = new_node;	  
		}
		
		p = c;
	}
	
    // p is a leaf p==0
	if (p) {
        // p is a leaf and c will be null in any case
        if (p->find(k,c))
			return false;	  
		
		T mk;
        bool mk_is_k;
		BTreeNode<T>* new_node = split(p, mk, k, mk_is_k);
        // TODO write a split that takesa 3rd arg, k.
        // If node is split, knowing that k has to be added
        // to one of the resulting nodes, use its value to
        // split the original node not just in the middle.
		
		if (!new_node) {
            p->insert(k, 0);
        } else {    
			if (parent) {		  
				parent->insert(mk, new_node);
			} else {
				BTreeNode<T>* new_root = new BTreeNode<T>(order_);
				new_root->leaf_=false;
				new_root->insert(mk, p, new_node);
				root_ = new_root;
			}
            
            if (!mk_is_k) {
    			if(k<mk) 
                    p->insert(k, 0);       
                else 
                    new_node->insert(k, 0);    
            }
        }
	}
	return true;
}

template < typename T >
void BTree<T>::replace_removed_key( BTreeNode<T>*& p, int & i)
{
    if (!p->leaf_) {
        BTreeNode<T>* l = p->child_[i];
        BTreeNode<T>* r = p->child_[i+1];
        // get the max size child
        BTreeNode<T> * m = l->size_ < r->size_ ? r : l;
        if (m->size_ > lb_ ) {
            if (m==l) {
                p->key_[i] = l->key_[l->size_-1];
                p = l;
                i = l->size_-1;
            } else { //m==r
                p->key_[i] = r->key_[0];
                p = r;
                i = 0;
            }
        } else {
            // merge nodes
            // append content of r to l
            int n = i;
            l->append(r,n);
            // shift left content of p
            for (int j=i; j<p->size_-1; ++j) {
                p->key_[j] = p->key_[j+1];
            }
            for (int j=i+1; j<p->size_; ++j) {
                p->child_[j] = p->child_[j+1];
            }
            --(p->size_);
            // TODO now the p node has reduced its size
            // rebalancing of the tree should start from this node
            // in case p was not merged in the precedent step

            // set p to the merged node
            p=l;
            i=n;
        }
    } else { // p is a leaf node and l==NULL and r==NULL
        // shift left content of p
        for (int j=i; j<p->size_-1; ++j) {
            p->key_[j] = p->key_[j+1];
        }
        --(p->size_);
        p=0;
    }

}

template < typename T >
void BTree<T>::rebalance_child_node( BTreeNode<T>* p, int i) {
    // sibling node of p->child_[i]
    BTreeNode<T> * s = 0;
    BTreeNode<T> * c = p->child_[i];
    
    if ( i-1 > 0 ) {
        s = p->child_[i-1];
        if ( i+1<=p->size_ && s->size_ < p->child_[i+1]->size_)
            s = p->child_[i+1];
    } else {
        s = p->child_[i+1];
    } 

    if (s == p->child_[i-1]) {
        if (s->size_ > lb_) {
            // shift right keys and children
            for (int j=c->size_; j>0; --j)
                c->key_[j] = c->key_[j-1];
            for (int j=c->size_+1; j>0; --j)
                c->child_[j] = c->child_[j-1];
            c->key_[0] = p->key_[i-1]; // TODO testing!
            c->child_[0] = s->child_[s->size_];
            p->key_[i-1] = s->key_[s->size_-1];
            ++(c->size_);
            --(s->size_);
        } else {
            // merge the two nodes
            int j;
            s->append(c,j);
            s->key_[j] = p->key_[i-1]; // TODO testing!
            // shift left keys and children
            for ( j=i-1; j<p->size_-1; ++j)
                p->key_[j] = p->key_[j+1];
            for ( j=i; j<p->size_; ++j)
                p->child_[j] = p->child_[j+1];
            --(p->size_);
        }
    } else { //s == p->child_[i+1]
        if (s->size_ > lb_) {
            c->key_[c->size_] = p->key_[i];
            c->child_[c->size_+1] = s->child_[0];
            p->key_[i] = s->key_[0];
            ++(c->size_);
            // shift left keys and children
            for ( int j=i; j<s->size_-1; ++j)
                s->key_[j] = s->key_[j+1];
            for ( int j=i+1; j<s->size_; ++j)
                s->child_[j] = s->child_[j+1];
            --(s->size_);
        } else {
            // merge the two nodes
            int j;
            c->append(s,j);
            s->key_[j] = p->key_[i];
            // shift left keys and children
            for ( j=i; j<p->size_-1; ++j)
                p->key_[j] = p->key_[j+1];
            for ( j=i+1; j<p->size_; ++j)
                p->child_[j] = p->child_[j+1];
            --(p->size_);
        }

    }
}

template < typename T >
bool BTree<T>::remove(const T & k)
{
    BTreeNode<T> * p = root_;
    BTreeNode<T> * c;
    int i;
    bool key_removed = false;
    
    typedef BTreeNode<T> Node;
	typedef Pair< Node*, int > Pair;
	Stack2< Pair > stack;
    
    while(p) {
        if (!p->remove(k,i,c)) {
            // search key to remove in node l
            stack.push(Pair(p,i));
            p = c; 
        } else if ( c ) { 
            // key removed from inner node
            // now i stores the index of the removed key
            // and p stores the node where the key have been removed
            key_removed = true;
            do { 
                stack.push(Pair(p,i));
                // the following method reduce the size
                // of p if necessary
                replace_removed_key(p, i);
            } while( p ); 

            break;
        } else {
            key_removed = true;
            stack.push(Pair(p,i));
            // key removed from leaf node 
            // p->size_ already reduced in remove method
            break;
        }
    }
    
    // for rebalancing you have to be able to go up
    // in the tree, from the last node where ended the replace_removed_key
    // up to the root if necessary.
    // That means in each node you need a pointer to the parent.
    // Else, you need to store on a stack, the path of nodes and indexes 
    // that led up to the one where the last replace_removed_key operation took place.
    if (key_removed) {
        
        // remove last node visited
        if (!stack.empty()) {
            stack.pop();
        }
        
        while (!stack.empty()) {
            Pair pp = stack.getTop();
            p = pp.first();
            i = pp.second();
            stack.pop();
            printf("0x%x - %d\n",p,i);

            if (p->child_[i]->size_<lb_) {
                rebalance_child_node(p,i);
            }
        }
    } 
    
    return key_removed;
}

template < typename T >
bool BTree<T>::find(T k)
{
	BTreeNode<T> * p = root_;
	BTreeNode<T> * c = 0;
	
	while (p) {
		if (p->find(k,&c))
			return true;
		p = c;
	}
	return false;
}

/* visit the tree in pre-order to dump the graphviz
 * file. Use the memory address of each node as a unique
 * identifier in the graphviz output.
 */
template < typename T >
void BTree<T>::toGraphViz( const char * iFilePath)
{
	std::ofstream out(iFilePath);

	if (out.is_open())
	{	
		Stack2<BTreeNode<T>*> stack;
		
		out << "digraph BTree {\n";
		out << "node [ shape=record, fixedsize=false];\n";
		//out << "{ rankdir=RL; }\n";
		//out << "{ rank=same; ";
        
        if (root_)
		    stack.push(root_);

		while( !stack.empty() )
		{
			BTreeNode<T>* p = stack.getTop();
			stack.pop();
			
			out << "\"" << p << "\"";
			out << "[label=\"" << p << " | ";
			
			int i = 0;
			for ( i=0; i<p->size_; ++i)
			{
				out << "<f" << i*2   << "> | ";
				out << "<f" << i*2+1 << "> " << p->key_[i] << " | ";
				if (p->child_[i])
					stack.push(p->child_[i]);	  
			}

			out << "<f" << p->size_*2 << ">\"];\n";
			if (p->child_[i])
				stack.push(p->child_[i]);	  

			for (int i=0; i<=p->size_; ++i)
			{
				if (p->child_[i]) 
				{
					out << "\"" << p << "\"";
					out << ":";
		 			out << "f" << i*2 << " -> ";
					out << "\"" << p->child_[i] << "\":f0;\n";
				}
			}
		}
		out << "}\n";
		out.close();
	}
	else
	{
		printf("Error: unable to open file: %s\n", iFilePath);
	}
	
}
#endif

