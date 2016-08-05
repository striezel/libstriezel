/*
 -----------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2012, 2014, 2015  Dirk Stolle

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

#include "BufferSource.hpp"
#include <cstring>
#include <stdexcept>

namespace SHA512
{

/* BufferSource functions */
BufferSource::BufferSource(uint8_t* data, uint64_t data_length_in_bits)
: MessageSource(), m_BufferPointer(data),
  m_BufferSize((data_length_in_bits/8) + ((data_length_in_bits%8)>0)) //we want full bytes only
{
  //do the padding here
  //m_BufferSize = (data_length_in_bits/8) + ((data_length_in_bits%8)>0); //we want full bytes only
  const unsigned int remainingDataBits = (m_BufferSize*8)%1024;
  if (remainingDataBits>888)
  {
    m_PaddingBuffer = new uint8_t[2048/8];
    //zero out all bits
    memset(m_PaddingBuffer, 0, 2048/8);
    //copy remainder
    memcpy(m_PaddingBuffer, &m_BufferPointer[m_BufferSize-(m_BufferSize%128)], m_BufferSize%128);
    //add data length in bits
    data_length_in_bits = m_BufferSize * 8;
    #if BYTE_ORDER == LITTLE_ENDIAN
    reverse64(data_length_in_bits, data_length_in_bits);
    #endif
    const uint64_t cZero = 0;
    memcpy(&m_PaddingBuffer[(2048-128)/8], &cZero, 8);
    memcpy(&m_PaddingBuffer[(2048-64)/8], &data_length_in_bits, 8);
    m_Status = psPadded2048;
  }
  else
  {
    m_PaddingBuffer = new uint8_t[1024/8];
    //zero out all bits
    memset(m_PaddingBuffer, 0, 1024/8);
    //copy remainder
    memcpy(m_PaddingBuffer, &m_BufferPointer[m_BufferSize-(m_BufferSize%128)], m_BufferSize%128);
    //add data length in bits
    data_length_in_bits = m_BufferSize * 8;
    #if BYTE_ORDER == LITTLE_ENDIAN
    reverse64(data_length_in_bits, data_length_in_bits);
    #endif
    const uint64_t cZero = 0;
    memcpy(&m_PaddingBuffer[(1024-128)/8], &cZero, 8);
    memcpy(&m_PaddingBuffer[(1024-64)/8], &data_length_in_bits, 8);
    m_Status = psPadded1024;
  }
  //add 1-bit
  m_PaddingBuffer[remainingDataBits/8] = 0x80;
  m_BitsRead = 0;
}

BufferSource::~BufferSource()
{
  m_BufferSize = 0;
  m_BufferPointer = NULL;
}

bool BufferSource::getNextMessageBlock(MessageBlock& mBlock)
{
  if (m_BitsRead+1024<=(m_BufferSize*8))
  {
    memcpy(&(mBlock.words[0]), &m_BufferPointer[m_BitsRead/8], 128);
    m_BitsRead += 1024;
    #if BYTE_ORDER == LITTLE_ENDIAN
    unsigned int i;
    for (i=0; i<16; ++i)
    {
      reverse64(mBlock.words[i], mBlock.words[i]);
    }
    #endif
    return true;
  }
  else
  {
    switch (m_Status)
    {
      case psPadded1024:
           memcpy(&(mBlock.words[0]), &m_PaddingBuffer[0], 128);
           m_Status = psPaddedAndAllRead;
           break;
      case psPadded2048:
           memcpy(&(mBlock.words[0]), &m_PaddingBuffer[0], 128);
           m_Status = psPadded2048And1024Read;
           break;
      case psPadded2048And1024Read:
           memcpy(&(mBlock.words[0]), &m_PaddingBuffer[128], 128);
           m_Status = psPaddedAndAllRead;
           break;
      case psPaddedAndAllRead:
           return false;
           break;
      case psUnpadded:
           //should never happen
           throw std::logic_error("BufferSource::getNextMessageBlock(): Code execution should never get to this point!");
           return false;
    }//swi
    m_BitsRead += 1024;
    #if BYTE_ORDER == LITTLE_ENDIAN
    unsigned int i;
    for (i=0; i<16; ++i)
    {
      reverse64(mBlock.words[i], mBlock.words[i]);
    }
    #endif
    return true;
  }//else
}

} //namespace
