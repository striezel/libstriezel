/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2011, 2013  Dirk Stolle

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

#include "Bits.hpp"
#include <cstring>

SmallBitArray16::SmallBitArray16()
: m_Bits(0), m_BitsPresent(0)
{

}

SmallBitArray16& SmallBitArray16::operator=(const SmallBitArray16& other)
{
  m_Bits = other.exposeBits();
  m_BitsPresent = other.getNumberOfBits();
  return *this;
}

SmallBitArray16::SmallBitArray16(const uint16_t theBits, const uint8_t numberOfBits)
: m_Bits(theBits), m_BitsPresent(numberOfBits)
{
  if (m_BitsPresent>16) m_BitsPresent = 16;
}

bool SmallBitArray16::operator==(const SmallBitArray16& other)
{
  return ((m_BitsPresent==other.m_BitsPresent) && (m_Bits==other.m_Bits));
}

uint8_t SmallBitArray16::getNumberOfBits() const
{
  return m_BitsPresent;
}

bool SmallBitArray16::getBit(const uint8_t index) const
{
  if (index>=m_BitsPresent) return false;
  return (((1<<index) & m_Bits)!=0);
}

bool SmallBitArray16::setBit(const uint8_t index, const bool newValue)
{
  if (index>=m_BitsPresent) return false;
  if (newValue)
  {
    //set bit
    m_Bits |= (1<<index);
  }
  else
  {
    //unset bit
    m_Bits &= (~(1<<index));
  }
  return true;
}

bool SmallBitArray16::insertBitAtFront(const bool newBit)
{
  if (m_BitsPresent>=16) return false;
  m_Bits = m_Bits << 1;
  if (newBit) m_Bits |= 1;
  ++m_BitsPresent;
  return true;
}

bool SmallBitArray16::appendBitAtBack(const bool newBit)
{
  if (m_BitsPresent>=16) return false;
  if (newBit) m_Bits = (m_Bits | (1<<m_BitsPresent));
  ++m_BitsPresent;
  return true;
}

bool SmallBitArray16::appendBitsAtBack(const SmallBitArray16& other)
{
  //there can't be more than 16 bits after concatenation
  if (m_BitsPresent+other.getNumberOfBits()>16) return false;
  m_Bits = (m_Bits | (other.m_Bits<<m_BitsPresent));
  m_BitsPresent = m_BitsPresent+other.getNumberOfBits();
  return true;
}

uint16_t SmallBitArray16::exposeBits() const
{
  return m_Bits;
}



/** Large bit array's functions **/

LargeBitArray64k::LargeBitArray64k()
: m_Bits(new uint8_t[256]),
  m_CurrentBytesAllocated(256),
  m_BitsPresent(0)
{
  memset(m_Bits, 0, 256);
}

LargeBitArray64k::LargeBitArray64k(const unsigned char* theBits, const uint16_t numberOfBits)
: m_BitsPresent(numberOfBits)
{
  uint16_t BytesRequired = (numberOfBits+7)/8;
  if (BytesRequired<256) BytesRequired = 256;
  m_Bits = new uint8_t[BytesRequired];
  m_CurrentBytesAllocated = BytesRequired;
  memcpy(m_Bits, theBits, (numberOfBits+7)/8);
}

LargeBitArray64k& LargeBitArray64k::operator=(const LargeBitArray64k& other)
{
  if (this==&other) return *this;
  if (other.m_CurrentBytesAllocated>this->m_CurrentBytesAllocated)
  {
    uint8_t * new_Bits = new uint8_t[other.m_CurrentBytesAllocated];
    memcpy(new_Bits, other.m_Bits, other.m_CurrentBytesAllocated);
    delete [] m_Bits;
    m_Bits = new_Bits;
    m_CurrentBytesAllocated = other.m_CurrentBytesAllocated;
  }
  else
  {
    //no further allocation required
    memcpy(m_Bits, other.m_Bits, other.m_CurrentBytesAllocated);
  }
  m_BitsPresent = other.getNumberOfBits();
  return *this;
}

LargeBitArray64k::~LargeBitArray64k()
{
  m_CurrentBytesAllocated = 0;
  m_BitsPresent = 0;
  delete[] m_Bits;
  m_Bits = 0;
}

bool LargeBitArray64k::operator==(const LargeBitArray64k& other) const
{
  return ((m_BitsPresent==other.getNumberOfBits()) and (memcmp(m_Bits, other.m_Bits, (m_BitsPresent+7)/8)==0));
}

uint16_t LargeBitArray64k::getNumberOfBits() const
{
  return m_BitsPresent;
}

bool LargeBitArray64k::getBit(const uint16_t index) const
{
  if (index>=m_BitsPresent) return false;
  return ((m_Bits[index/8] & (1<<(index % 8))) != 0);
}

bool LargeBitArray64k::setBit(const uint16_t index, const bool newValue)
{
  if (index>=m_BitsPresent) return false;
  if (newValue)
  {
    //set bit
    m_Bits[index/8] |= (1<<(index%8));
  }
  else
  {
    //unset bit
    m_Bits[index/8] &= (~(1<<(index%8)));
  }
  return true;
}

bool LargeBitArray64k::hasEnoughSpaceForAnotherBit() const
{
  // possible_maximum bit index with current alloc is := m_CurrentBytesAllocated*8-1;
  return (m_BitsPresent+1<=m_CurrentBytesAllocated*8-1);
}

bool LargeBitArray64k::canAllocateMoreSpace() const
{
  return (m_CurrentBytesAllocated < 8192);
}

bool LargeBitArray64k::increaseInternalArray()
{
  if (!canAllocateMoreSpace()) return false;
  uint16_t new_allocation_length = m_CurrentBytesAllocated * 2;
  if ((new_allocation_length)>8192) new_allocation_length = 8192;
  uint8_t* new_Bits = new uint8_t[new_allocation_length];
  memcpy(new_Bits, m_Bits, m_CurrentBytesAllocated);
  uint8_t* old_bits = m_Bits;
  m_Bits = new_Bits;
  m_CurrentBytesAllocated = new_allocation_length;
  delete[] old_bits;
  return true;
}

bool LargeBitArray64k::insertBitAtFront(const bool newBit)
{
  if (!hasEnoughSpaceForAnotherBit())
  {
    //we need to increase internal array size
    if (!increaseInternalArray()) return false;
  }
  unsigned int i;
  for (i=1+(m_BitsPresent%8); i>0; --i)
  {
    m_Bits[i] = m_Bits[i] << 1;
    if (getBit(i*8-1))
    {
      m_Bits[i] |= 1;
    }
  }
  //for zero-th byte
  m_Bits[0] = m_Bits[0] << 1;
  if (newBit) m_Bits[0] |= 1;
  ++m_BitsPresent;
  return true;
}

bool LargeBitArray64k::appendBitAtBack(const bool newBit)
{
  if (!hasEnoughSpaceForAnotherBit())
  {
    //we need to increase internal array size
    if (!increaseInternalArray()) return false;
  }
  if (newBit) m_Bits[m_BitsPresent/8] |= (1<<(m_BitsPresent%8));
  else m_Bits[m_BitsPresent/8] &= (~(1<<(m_BitsPresent%8)));
  ++m_BitsPresent;
  return true;
}

bool LargeBitArray64k::appendBitsAtBack(const LargeBitArray64k& other)
{
  const uint16_t bitsToAdd = other.getNumberOfBits();
  if (bitsToAdd+m_BitsPresent>cMaximumBits) return false;
  unsigned int i;
  for (i=0; i<bitsToAdd; ++i)
  {
    if (!appendBitAtBack(other.getBit(i))) return false;
  }//for
  return true;
}

bool LargeBitArray64k::operator==(const SmallBitArray16& right) const
{
  if (getNumberOfBits()!=right.getNumberOfBits()) return false;
  unsigned int i;
  for (i=0; i<right.getNumberOfBits(); ++i)
  {
    if (getBit(i)!=right.getBit(i)) return false;
  }
  return true;
}

SmallBitArray16 LargeBitArray64k::getSmallBitSequence(const uint16_t startIndex, const uint8_t length) const
{
  if ((startIndex+length>m_BitsPresent) || (length>16)) return SmallBitArray16(0, 0);

  uint16_t data = 0;
  uint8_t bits_done = 0;
  while (bits_done<length)
  {
    if (this->getBit(startIndex+bits_done))
    {
      data |= (1<<bits_done);
    }
    ++bits_done;
  }//while
  return SmallBitArray16(data, length);
}

uint8_t LargeBitArray64k::exposeByte(const uint16_t byte_index) const
{
  if (byte_index>=m_CurrentBytesAllocated) return 0;
  return m_Bits[byte_index];
}

void LargeBitArray64k::removeLeadingBytes(const uint16_t count)
{
  if (m_CurrentBytesAllocated<=count)
  {
    m_BitsPresent = 0;
    memset(m_Bits, 0, m_CurrentBytesAllocated);
    return;
  }
  memmove(m_Bits, &(m_Bits[count]), m_CurrentBytesAllocated-count);
  if (8*count>=m_BitsPresent)
  {
    m_BitsPresent = 0;
  }
  else
  {
    m_BitsPresent = m_BitsPresent - 8*count;
  }
  return;
}
