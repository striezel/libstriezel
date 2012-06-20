/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2011, 2012  thoronador

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 -----------------------------------------------------------------------------
*/

#include "Decoder.h"
#include <map>
#include <vector>
#include <iostream>
#include <cmath>
#include "../Bits.h"

typedef std::map<uint16_t, std::vector<uint16_t> >::const_iterator codeTableIterator;

bool decode(const GIFColourTable& colourTable, const GIFTableBasedImage& image)
{
  if (image.getImageData().getNumberOfSubBlocks()==0)
  {
    std::cout << "Error: table based image does not have any data blocks!\n";
    return false;
  }
  const std::vector<GIFDataSubBlock>& blocks = image.getImageData().getBlocks();

  //"code table" : key is code as read from stream, value is sequence of
  //   colour table indices associated with that code
  std::map<uint16_t, std::vector<uint16_t> > codeTable;

  unsigned int i;
  for (i=0; i<colourTable.getNumberOfColourEntries(); ++i)
  {
    codeTable[i] = std::vector<uint16_t>(1, i);
  }//for

  const uint16_t clearCode = pow(2, image.getImageData().getMinCodeSize());
  const uint16_t endOfInformation = clearCode +1;
  uint16_t nextAvailableCode = endOfInformation +1;
  codeTable[clearCode] = std::vector<uint16_t>(0, 0);
  std::cout << "Info: decode: colours: "<<(int)colourTable.getNumberOfColourEntries()
            << "\n      Code size:" << (int) image.getImageData().getMinCodeSize()<<"\n";
  std::cout << "clear code: "<<clearCode<<"; EOI: "<<endOfInformation<<"\n";
  uint16_t currentCodeLength = image.getImageData().getMinCodeSize()+1;
  uint16_t nextBitComingAt = pow(2, currentCodeLength)-1;

  //the stream of indices as we decode it
  std::vector<uint16_t> indexStream;

  LargeBitArray64k bit_arr = LargeBitArray64k(blocks.at(0).getBlockData(), blocks.at(0).getBlockSize()*8);
  std::cout << "Size of first block: "<<(int)blocks.at(0).getBlockSize()<<"\n";
  size_t indexOfLastReadBlock = 0;
  SmallBitArray16 prev_code(0,0);
  SmallBitArray16 small_arr = bit_arr.getSmallBitSequence(0, currentCodeLength);
  uint32_t bits_read = currentCodeLength;
  std::cout << "First code is "<<small_arr.exposeBits()<<"\n";


  std::map<uint16_t, std::vector<uint16_t> >::const_iterator ct_iter;
  std::vector<uint16_t> app_sequence;

  int loop_iterations = 0;
  bool done = (bits_read>=bit_arr.getNumberOfBits());
  while (!done)
  {
    //are we out of data?
    if (bits_read+currentCodeLength>bit_arr.getNumberOfBits())
    {
      std::cout << "Debug: Out of data, trying to append new block!\n";
      LargeBitArray64k tempBitArr;
      if (indexOfLastReadBlock+1<blocks.size())
      {
        //feed next block
        ++indexOfLastReadBlock;
        tempBitArr = LargeBitArray64k(blocks.at(indexOfLastReadBlock).getBlockData(), blocks.at(indexOfLastReadBlock).getBlockSize()*8);
      }
      else
      {
        std::cout << "Error: there are no more data sub-blocks, but EOI was not"
                  << " reached yet!\n";
        return false;
      }
      //is there enough space?
      if (tempBitArr.getNumberOfBits()+bit_arr.getNumberOfBits()>LargeBitArray64k::cMaximumBits)
      {
        std::cout << "Debug: Shortening needed, current length is "<<bit_arr.getNumberOfBits()<<" bits!\n";
        const uint16_t cutOff = bits_read/8;
        std::cout << "Trying to cut off "<<cutOff<<" bytes.\n";
        bit_arr.removeLeadingBytes(cutOff);
        std::cout << "New length is "<<bit_arr.getNumberOfBits()<<" bits!\n";
        bits_read = bits_read -8*cutOff;
      }

      //append it
      if (!bit_arr.appendBitsAtBack(tempBitArr))
      {
        std::cout << "Error: could not append new data block!\nBits in 1st block: "
                  << bit_arr.getNumberOfBits() << "  bits in 2nd block: "<< tempBitArr.getNumberOfBits()<<"\n";
        return false;
      }
    }//if we need more data

    prev_code = small_arr;
    small_arr = bit_arr.getSmallBitSequence(bits_read, currentCodeLength);
    bits_read += currentCodeLength;
    //check table for presence of code
    ct_iter = codeTable.find(small_arr.exposeBits());
    //found?
    if (ct_iter!=codeTable.end())
    {
      // Yes, found it in table.
      // output {CODE} to index stream
      indexStream.insert(indexStream.end(), ct_iter->second.begin(), ct_iter->second.end());
      // let K be the first index in {CODE}
      uint16_t k = ct_iter->second.at(0);
      // add {CODE-1}+K to the code table
      const codeTableIterator prev_iter = codeTable.find(prev_code.exposeBits());
      app_sequence = prev_iter->second;
      app_sequence.push_back(k);
      codeTable[nextAvailableCode] = app_sequence;
      ++nextAvailableCode;
      //do we need to increase the code length?
      if (nextAvailableCode>=nextBitComingAt)
      {
        ++currentCodeLength;
        nextBitComingAt = pow(2, currentCodeLength) -1;
      }
    }
    else
    {
      //not found in table
      std::cout << "next code "<<small_arr.exposeBits()<<" was NOT found!\n";
      // let K be the first index of {CODE-1}
      ct_iter = codeTable.find(/*small_arr.exposeBits()-1*/prev_code.exposeBits());
      uint16_t k = ct_iter->second.at(0);
      // output {CODE-1}+K to index stream
      app_sequence = ct_iter->second;
      app_sequence.push_back(k);
      indexStream.insert(indexStream.end(), app_sequence.begin(), app_sequence.end());
      // add {CODE-1}+K to code table
      codeTable[small_arr.exposeBits()] = app_sequence;
      //do we need to increase the code length?
      if (small_arr.exposeBits()>=nextBitComingAt)
      {
        ++currentCodeLength;
        nextBitComingAt = pow(2, currentCodeLength) -1;
      }
    }
    ++loop_iterations;
    done = (bits_read>=bit_arr.getNumberOfBits());
  }//while



  //just for fun: print the indices we have so far
  std::cout << "The "<<indexStream.size()<<" indices after "<<loop_iterations<<" steps so far:";
  for (i=0; i<indexStream.size(); ++i)
  {
    std::cout <<" "<< indexStream[i];
  }//for
  std::cout << "\nBits read: "<<bits_read<<", bits available: "<<bit_arr.getNumberOfBits()<<"\n";

  #warning Incomplete! Not implemented.
}
