#include <fstream>   // will be reading from and writing to file 
#include <iostream>
#include "HCTree.hpp"
#include "BitOutputStream.hpp"

using namespace std;

int main(int argc, char* argv[]){
  
  // error checking
  if(argc != 3){
    cerr << "The wrong number of arguments were entered.\n";
    return 0;
  }
  ifstream inStream;   		// this is input filestream
  ofstream outStream;  		// this is output filestream
  vector<int>freq(256, 0);	// will store frequencies, all 0's
  HCTree Tree;			// instance of HCTree class
  byte symbol;			// will store current symbol
  int charcount = 0;		// store number of chars in infile
  int diffsymbols = 0;
  //cout << "Opening 1.\n";
  inStream.open(argv[1]);     	// open the input file
  
  // error checking
  if(inStream.fail() == true){
    cerr << "Had trouble opening input file.\n" << endl;
    return 0;
  }

  outStream.open(argv[2], ios::binary);      // open the output file
  symbol = inStream.get();  	// read first symbol

  // compute frequency of symbols
  while(inStream.eof() == false){
    freq[symbol] = freq[symbol] + 1; 	// increment count
    symbol = inStream.get(); 		// get next symbol
    charcount = charcount + 1;
  }
  for(int i = 0; i < 256; i++){
    if(freq[i] != 0){
      diffsymbols = diffsymbols + 1;
    }
  }

  // output header describing tree to the output file
  BitOutputStream bos(outStream); 	// instance of BitOutputStream
  
  // this will write to the header file 
  bos.writeInt(charcount);	// charcount written to header
  bos.writeInt(diffsymbols);	// store distinct symbols
  Tree.build(freq);             // build the Huffman tree
 
  // optimized header file
  Tree.store(bos);		// store the Huffman tree    

  //rewind the file to the beginning, re-reading each symbol
  inStream.close(); 		// close file
  inStream.open(argv[1], ios::binary);		// re-open, for encoding
  
  symbol = inStream.get();		// read first symbol
  
  while(inStream.eof() == false){	// end of file reached?
    //cout << "\nCOMPRESS: Symbol " << symbol << endl;
    Tree.encode(symbol, bos);   	// call to encode method of Tree
    symbol = inStream.get();    	// get next symbol from infile
    // NOTE: encode handles writing to the file
  }
  bos.flush();
  outStream.close();
  return 0;
}




