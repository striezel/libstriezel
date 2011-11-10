/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2011 thoronador

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

#include "Bits.h"
#include <cstring>

SmallBitArray16::SmallBitArray16()
{
  m_Bits = 0;
  m_BitsPresent = 0;
}

SmallBitArray16::SmallBitArray16(const uint16_t theBits, const uint8_t numberOfBits)
{
  m_Bits = theBits;
  m_BitsPresent = numberOfBits;
  if (m_BitsPresent>16) m_BitsPresent = 16;
}

bool SmallBitArray16::operator=(const SmallBitArray16& other)
{
  return ((m_BitsPresent==other.m_BitsPresent) and (m_Bits==other.m_Bits));
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



/** Large bit array's functions **/

LargeBitArray2048::LargeBitArray2048()
{
  m_BitsPresent = 0;
  memset(m_Bits, 0, 256);
}

LargeBitArray2048::LargeBitArray2048(const uint8_t theBits[256], const uint16_t numberOfBits)
{
  m_BitsPresent = numberOfBits;
  memcpy(m_Bits, theBits, 256);
}

bool LargeBitArray2048::operator=(const LargeBitArray2048& other) const
{
  return ((m_BitsPresent==other.getNumberOfBits()) and (memcmp(m_Bits, other.m_Bits, 256)==0));
}

uint16_t LargeBitArray2048::getNumberOfBits() const
{
  return m_BitsPresent;
}

bool LargeBitArray2048::getBit(const uint16_t index) const
{
  if (index>=m_BitsPresent) return false;
  return ((m_Bits[index/8] & (1<<(index % 8))) != 0);
}

bool LargeBitArray2048::setBit(const uint16_t index, const bool newValue)
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

bool LargeBitArray2048::insertBitAtFront(const bool newBit)
{
  if (m_BitsPresent>=cMaximumBits) return false;
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

bool LargeBitArray2048::appendBitAtBack(const bool newBit)
{
  if (m_BitsPresent>=cMaximumBits) return false;
  if (newBit) m_Bits[m_BitsPresent/8] |= (1<<(m_BitsPresent%8));
  else m_Bits[m_BitsPresent/8] &= (~(1<<(m_BitsPresent%8)));
  ++m_BitsPresent;
  return true;
}

bool LargeBitArray2048::operator=(const SmallBitArray16& right) const
{
  if (getNumberOfBits()!=right.getNumberOfBits()) return false;
  unsigned int i;
  for (i=0; i<right.getNumberOfBits(); ++i)
  {
    if (getBit(i)!=right.getBit(i)) return false;
  }
  return true;
}

SmallBitArray16 LargeBitArray2048::getSmallBitSequence(const uint16_t startIndex, const uint8_t length) const
{
  if ((startIndex+length>m_BitsPresent) or (length>16)) return SmallBitArray16(0, 0);

  uint16_t data = 0;
  uint8_t bits_done = 0;
  while (bits_done<length)
  {
    data |= getBit(startIndex+bits_done)<<bits_done;
    ++bits_done;
  }//while
  return SmallBitArray16(data, length);
}
