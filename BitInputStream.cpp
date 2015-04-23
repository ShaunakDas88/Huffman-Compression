#include <iostream>
#include "BitInputStream.hpp"

using namespace std;

  /** Read the next bit from the bit buffer.
   *  If the bit buffer is currently empty,
   *  fill the bit buffer by reading a char from the istream first.
   *  Return the bit read as the least signficant bit of an int.
   *  Return -1 on EOF.
   *  This must be consistent with BitOutputStream::writeBit(), in terms
   *  of ordering of bits in the stream.
   */  
  int BitInputStream::readBit(){
    //cout << "This is the readBit() method!\n";
    
    if(bufi == 8){   // the bit buffer is empty  
      buf = in.get(); // fill the bit buffer
      bufi = 0;		// bit buffer has been filled
    }
    // return bit read as least significant bit of an int
    bufi = bufi + 1; // increment the number of bits read
    return buf[bufi - 1];
  }

  /** Read a byte firom the istream.
   *  Return -1 on EOF.
   *  This function doesn't touch the bit buffer.
   *  The client has to manage interaction between reading bits
   *  and reading bytes.
   */
  int BitInputStream::readByte(){
    //cout << "This is the readByte method!\n";  
    int byte;	// read before EOF flag is set
    byte = in.get();
   
    // return -1 on EOF
    if(in.eof() == true){
      return -1;
    } 
    return byte; // return the byte that was read
  }

  /** Read a non-negative int from the istream.
   *  Return -1 on EOF.
   *  This function doesn't touch the bit buffer.
   *  The client has to manage interaction between reading bits
   *  and reading ints.
   */
  int BitInputStream::readInt(){
    if(in.eof() == true){
      return -1;
    }
    int integer;
    in.read((char*)&integer, sizeof(int));
    return integer; 
  }
