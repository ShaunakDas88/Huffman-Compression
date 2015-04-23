#include <iostream>
#include "HCNode.hpp"

/* This is overloading the < operator for the HCNode class
 *
 */

bool HCNode::operator<(const HCNode& other){
  if(count < other.count){
    return false;
  }
  if(count > other.count){
    return true;
  }
  if(count == other.count){
  // break ties using symbols 
    if(symbol < other.symbol){
      return true;
    }    
    else{
      return false;
    }
  }
}

/* This is a definition of the compare method
 * 
 */

bool comp(HCNode* one, HCNode* other){


}



