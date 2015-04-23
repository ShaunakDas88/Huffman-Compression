#ifndef HCTREE_HPP
#define HCTREE_HPP

#include <queue>
#include <vector>
#include <fstream>
#include "HCNode.hpp"
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"

using namespace std;

/** A 'function class' for use as the Compare class in a
 *  priority_queue<HCNode*>.
 *  For this to work, operator< must be defined to
 *  do the right thing on HCNodes.
 */
class HCNodePtrComp {
public:
    bool operator()(HCNode*& lhs, HCNode*& rhs) const {
        return *lhs < *rhs; // using operator overloading here
    }   
};

/** :A Huffman Code Tree class.
 *  Not very generic:  Use only if alphabet consists
 *  of unsigned chars.
 */
class HCTree {
private:
    HCNode* root;
    vector<HCNode*> leaves;
    
    /** This is a helper method called on by the store method of HCTree
     *  This is used to make an preorder traversal of the nodes of the
     *  HCTree, and whenever a leaf is encountered, the corresponding symbol 
     *  and its 0-base level are written to the passed in BitOutputStream
     */
    void preorder(HCNode* node, BitOutputStream& out, int level);  
 
    /** This is a helper method called from the HCNode destructor
     *  This will recursively delete all HCNodes that are in the
     *  givne instance of the HCTree
     */
    void deleteNode(HCNode* node);

public:
    explicit HCTree() : root(0) {
	// this initializes a vector of 256 HCNode pointers
	// that are all nullptr 
        leaves = vector<HCNode*>(256, (HCNode*) 0);
    }

    ~HCTree(); // default destructor for HCTree class

    /** Use the Huffman algorithm to build a Huffman coding trie.
     *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is 
     *  the frequency of occurrence of byte i in the message.
     *  POSTCONDITION:  root points to the root of the trie,
     *  and leaves[i] points to the leaf node containing byte i.
     */
    void build(const vector<int>& freqs);

    /** Write to the given BitOutputStream
     *  the sequence of bits coding the given symbol.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    void encode(byte symbol, BitOutputStream& out) const;

    /** Return symbol coded in the next sequence of bits from the stream.
     *  PRECONDITION: build() has been called, to create the coding
     *  tree, and initialize root pointer and leaves vector.
     */
    int decode(BitInputStream& in) const;
    

    /** This method is used for optimization of the header.
     *  This is called from the compress file, and used to get the
     *  preorder traversal order of the leaf nodes and their levels.
     */
    void store(BitOutputStream& out); 

    /** This method is used to reconstruct the Huffman coding from the
     *  information of the leaves (from left to right) and their 
     *  corresponding 0-base levels. This method is called on from 
     *  uncompress.cpp, after it has processed the header appropriately
     *  and assembled vectors of leaves and their corresponding levels.
     */ 
    void rebuild(vector<byte> leaves, vector<int> levels); 

};

#endif // HCTREE_HPP
