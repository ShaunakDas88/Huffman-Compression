#include <iostream>
#include "BitOutputStream.hpp"

  /** Write the least significant bit of the argument into
  *  the bit buffer, and increment the bit buffer index.
  *  Flush to the ostream FIRST if the bit buffer is full.
  *  This must be consistent with BitInputStream::readBit().
  */
  void BitOutputStream::writeBit(int bit){
    //cout << "BitOutputStream class: this is writeBit method\n";
    // if bit buffer is full, then flush it
    if(bufi == 8){
      flush();
    }
    buf[bufi] = bit&1;	//write this bit to the bit buffer
    //cout << "The buffer is storing " << (int)buf << endl;
    bufi = bufi+1; // increment the bit buffer index
  }

  
  /** Write the least significant byte of the argument to the ostream.
   *  THIS FUNCTION DOESN'T TOUCH THE BIT BUFFER.
   *  The client has to manage interaction between writing bits
   *  and writing bytes.
   */
  void BitOutputStream::writeByte(int b){
    out.put(b);
  }

  
  /** Write the argument to the ostream.
  *  THIS FUNCTION DOESN'T TOUCH THE BIT BUFFER.
  *  The client has to manage interaction between writing bits
  *  and writing ints.
  */
  void BitOutputStream::writeInt(int i){
    out.write((char*)&i, sizeof(int));
  }


  /** If the bit buffer contains any bits, flush the bit buffer to the ostream,
   *  clear the bit buffer, and set the bit buffer index to 0.
   *  Also flush the ostream itself.
   */
  void BitOutputStream::flush(){  
    //NOTE: THIS IS WHERE THE ACTUAL WRITING TO A FILE OCCURS 
    out.put((char)buf.to_ulong()); // put to the ostream buffer
    out.flush();        // flush bit buffer to ostream 
    buf = 0;            // clear the bit buffer
    bufi = 0; 		// set bit buffer index to 0
  }
