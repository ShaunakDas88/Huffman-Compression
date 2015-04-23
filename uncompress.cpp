#include <fstream>   // will be reading from and writing to file 
#include <iostream>
#include "HCTree.hpp"
#include "BitInputStream.hpp"
#include "BitOutputStream.hpp"

using namespace std;

int main(int argc,  char* argv[]){
 
  //error checking 
  if(argc != 3){
    cerr << "The wrong number of arguments were entered.\n";
    return 0;
  }
  ifstream inStream;		// input filestream
  ofstream outStream;		// output filestream	
  HCTree Tree;			// instance of HCTree class
  byte symbol;		// will store current symbol
  
  inStream.open(argv[1],ios::binary);		// open the input file
  // error checking
  if(inStream.fail()){
    cerr << "Had trouble opening input file.\n";
  }
  outStream.open(argv[2]);		        // open the output file
  BitInputStream bis(inStream);			// call to constructor
  

  // handle header processing
  int charcount = bis.readInt();	// total number of chars 
  cout << endl;
  int diffsymbols = bis.readInt(); 	// number of distinct symbols
  
  if(charcount == 0){
    return 0;
  }
  vector<byte>symbols(diffsymbols, 0);		// vector of levels
  vector<int>levels(diffsymbols, 0);		// vector of symbols 
  
  // this is for the clever header
  for(int i = 0; i < diffsymbols; i++){
    symbols[i] = bis.readByte();
    levels[i] = bis.readByte();		

  }
  
  // this is for the optimized header
  // the leaves and (corresponding levels) are stored in pre-order
  // traversal order
  Tree.rebuild(symbols, levels);
  BitOutputStream bos(outStream);
  
  // read remaining bits, to find the original value
  symbol = Tree.decode(bis);
  outStream.put(symbol);
  int j = 1;
  
  while(j < charcount){
    symbol = Tree.decode(bis);
    outStream.put(symbol);
    j = j+1;
  }
  cout << endl;
  return 0;
}
