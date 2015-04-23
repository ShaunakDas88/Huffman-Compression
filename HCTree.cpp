#include <iostream>
#include <vector>
#include <queue>
#include "HCTree.hpp"
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"

using namespace std;

  /** Use the Huffman algorithm to build a Huffman coding trie.
   *  PRECONDITION: freqs is a vector of ints, such that freqs[i] is 
   *  the frequency of occurrence of byte i in the message.
   *  POSTCONDITION:  root points to the root of the trie,
   *  and leaves[i] points to the leaf node containing byte i.
   */
  
  HCTree::~HCTree(){
    deleteNode(root);
    root = nullptr;
  }
  
  /** This is a helper method called from the HCNode destructor
   *  This will recursively delete all HCNodes that are in the
   *  givne instance of the HCTree
   */

  void HCTree::deleteNode(HCNode* node){
    if(node == nullptr){
      return;
    }
    if(node->c0 != nullptr){
      deleteNode(node->c0);
    }
    if(node->c1 != nullptr){
      deleteNode(node->c1);
    }
    delete node;
    return; 
  }

  void HCTree::build(const vector<int>& freqs){
    HCNode* temp1; // used when consolidating nodes
    HCNode* temp2; // used when consolidating nodes
    HCNode* internal; // used when consolidating nodes

    // using C++ STL to instantiate a priority_queue
    std::priority_queue<HCNode*, vector<HCNode*> , HCNodePtrComp> minheap;
   
    // initialize each HCNode* in leaves vector
    for(int i = 0; i < 256; i++){
      //cout << i << endl;
      if(freqs[i] != 0){
	leaves[i] = new HCNode(freqs[i], i);
      }
    }
     
    // now use minheap to store pointers to node (STL)
    for(int i = 0; i < 256; i++){
      // if the byte actually occurs in the message
      if(leaves[i] != nullptr){
        minheap.push(leaves[i]);
      }
    } // closing for loop

    // our minheap stores all bits that show up. At this point, we 
    // begin to consolidate nodes.
    if(minheap.size() == 0 ){
      root = nullptr;
      return;
    }
    if(minheap.size() == 1){
      root = minheap.top();
      // encode an int 
    }
    while(minheap.size() > 1){
      temp1 = minheap.top(); // access top of the min heap
      minheap.pop(); // remove it from heap
      temp2 = minheap.top(); //access top of the min heap
      minheap.pop(); // remove it from heap
      // create new instance of HCNode, return pointer to it
      internal = new HCNode(temp1->count + temp2->count, temp1->symbol);
     
      //update appropriate pointers
      internal->c0 = temp1; 
      internal->c1 = temp2;
      temp1->p = temp2->p = internal;
   
      // push this internal node back onto the minheap
      minheap.push(internal);
    }  // closing of while loop
    // At this point, we have that minheap has one element
    root = minheap.top(); // set root pointer to element in minheap
    minheap.pop(); // remove this element from minheap

  }


  /** Write to the given BitOutputStream
   *  the sequence of bits coding the given symbol.
   *  PRECONDITION: build() has been called, to create the coding
   *  tree, and initialize root pointer and leaves vector.
   */
  void HCTree::encode(byte symbol, BitOutputStream& out) const{
    HCNode* tempNode = leaves[symbol]; // pointer to appropriate leaf
    std::vector<int> reverse; // to store symbols
    std::vector<int> forward; // store reverse-order of array
    int bits;

    // parent all the way up to the root
    while(tempNode != root){
      // if tempNode is a 0-child
      if (tempNode->p->c0 == tempNode){
        reverse.push_back(0); // append a 0 to array vector
      }
      // if tempNode is a 1-child
      else{
        reverse.push_back(1); // append a 1 to array vector
      }
      tempNode = tempNode->p; // parent up the tree
    }
    // now reverse the vector
    bits = reverse.size();
    for(int i = 0; i < bits; i++){
      forward.push_back(reverse[bits-1-i]);
      
    }    
    for(int i = 0; i < bits; i++){
      out.writeBit(forward[i]); // call to writeBit method of out
     }   
  }


  /** Return symbol coded in the next sequence of bits from the stream.
   *  PRECONDITION: build() has been called, to create the coding
   *  tree, and initialize root pointer and leaves vector.
   */
  int HCTree::decode(BitInputStream& in) const{
    //cout << "\nThis is the decode method of HCTree.\n";
    HCNode* tempNode = root; // traverse the tree
    int currbit; 	   // will store current bit
  
    // while tempNode is still an internal node...
    while(tempNode->c0 != nullptr){
      //cout << "DECODE METHOD: Reading next bit.\n";
      currbit = in.readBit();  // read next bit from the BitInputStream
      if(currbit == 0){
        //cout << "DECODE:This is a 0-child.\n";
        tempNode = tempNode->c0;
      }
      else if(currbit == 1){
        //cout << "DECODE:This is a 1-child\n";
        tempNode = tempNode->c1;
      }
      else{
        return -1;
      }
    } //closing of while loop 
  
    // tempNode is a pointer to the appropriate leaf node
    return tempNode->symbol;
  }


  /** This method is used for optimization of the header.
   *  This is called from the compress file, and used to get the
   *  preorder traversal order of the leaf nodes and their levels.
   */

  void HCTree::store(BitOutputStream& out){
    preorder(root, out, 0);
  }
 
  
  /** This is a helper method called on by the store method of HCTree
   *  This is used to make an preorder traversal of the nodes of the
   *  HCTree, and whenever a leaf is encountered, the corresponding symbol 
   *  and its 0-base level are written to the passed in BitOutputStream
   */
  void HCTree::preorder(HCNode* node, BitOutputStream& out, int level){
    int leaflevel = level;
    if(node == nullptr){ // the case of an empty tree
      return;
    }
    if(node->c0 == nullptr){ // we have reached a leaf node
      out.writeByte(node->symbol);	// this will write the leaf symbol
      out.writeByte(leaflevel);    	// this will write the leaf level
      return;
    }
    // go down to the next level
    preorder(node->c0, out, leaflevel+1);	//recursive call
    preorder(node->c1, out, leaflevel+1);	// recursvie call
  }
  
  /** This method is used to reconstruct the Huffman coding from the
   *  information of the leaves (from left to right) and their 
   *  corresponding 0-base levels. This method is called on from 
   *  uncompress.cpp, after it has processed the header appropriately
   *  and assembled vectors of leaves and their corresponding levels.
   */ 
  void HCTree::rebuild(vector<byte> leaves, vector<int> levels){
    HCNode* currNode = new HCNode(0,0);
    root = currNode;
    HCNode* tempNode;
    int currlevel = 0;
    int diffsymbols = leaves.size();

    // going to iterate through the vector of leaf symbols
    for(int i= 0; i < diffsymbols; i++){
      while(currlevel < levels[i]){
        if(currNode->c0 == nullptr){
          tempNode = new HCNode(0,0);
          currNode->c0 = tempNode;
          tempNode->p = currNode;
          currNode = tempNode;
          currlevel = currlevel + 1;
        }
        else{ // the case that currnode has a left child
          tempNode = new HCNode(0, 0);
          currNode->c1 = tempNode;
          tempNode->p = currNode;
          currNode = tempNode;
          currlevel = currlevel + 1;  
        }
      }
      // fill the leaf  
      currNode->symbol = leaves[i];
      // parent back up to something without two children
      while(currNode != root){
        if(currNode->p->c1 != nullptr){
          currNode = currNode->p;
          currlevel = currlevel - 1;
     
        } // closing while loop
        else{
          break;
        }
      } //closing outer while loop 
      if(currNode != root){
        currNode = currNode->p;
        currlevel = currlevel - 1;
      }
    } // closing for loop
  }//closing method

