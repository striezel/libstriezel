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

#include "DataSubBlock.h"
#include <cstring>
#include <iostream>

/***** GIFDataSubBlock functions *****/

GIFDataSubBlock::GIFDataSubBlock()
: m_Size(0),
  m_DataPointer(NULL)
{

}

GIFDataSubBlock::GIFDataSubBlock(const GIFDataSubBlock& op)
{
  m_Size = op.getBlockSize();
  if (m_Size!=0)
  {
    m_DataPointer = new unsigned char [m_Size];
    memcpy(m_DataPointer, op.getBlockData(), m_Size);
  }
  else
  {
    m_DataPointer = NULL;
  }
}

GIFDataSubBlock& GIFDataSubBlock::operator=(const GIFDataSubBlock& op)
{
  if (this == &op) return *this;
  m_Size = op.getBlockSize();
  delete[] m_DataPointer;
  if (m_Size!=0)
  {
    m_DataPointer = new unsigned char [op.getBlockSize()];
    memcpy(m_DataPointer, op.getBlockData(), m_Size);
  }
  else
  {
    m_DataPointer = NULL;
  }
  return *this;
}

GIFDataSubBlock::~GIFDataSubBlock()
{
  m_Size = 0;
  delete[] m_DataPointer;
}

uint8_t GIFDataSubBlock::getBlockSize() const
{
  return m_Size;
}

const unsigned char* GIFDataSubBlock::getBlockData() const
{
  return m_DataPointer;
}

bool GIFDataSubBlock::readFromStream(std::ifstream& inputStream)
{
  if (!inputStream.good())
  {
    std::cout << "GIFDataSubBlock::readFromStream: Error: bad input stream!\n";
    return false;
  }

  /*
      7 6 5 4 3 2 1 0        Field Name                    Type
     +---------------+
  0  |               |       Block Size                    Byte
     +---------------+
  1  |               |
     +-             -+
  2  |               |
     +-             -+
  3  |               |
     +-             -+
     |               |       Data Values                   Byte
     +-             -+
 up  |               |
     +-   . . . .   -+
 to  |               |
     +-             -+
     |               |
     +-             -+
255  |               |
     +---------------+
  */

  uint8_t new_Size = 0;
  inputStream.read((char*) &new_Size, 1);
  if (!inputStream.good())
  {
    std::cout << "GIFDataSubBlock::readFromStream: Error: could not read block size!\n";
    return false;
  }
  //allocate new block
  unsigned char * new_Data = new unsigned char [new_Size];
  inputStream.read((char*) new_Data, new_Size);
  if (!inputStream.good())
  {
    std::cout << "GIFDataSubBlock::readFromStream: Error: could not read data values!\n";
    delete[] new_Data;
    return false;
  }
  //reading done, update data
  m_Size = 0;
  delete[] m_DataPointer;
  m_DataPointer = new_Data;
  m_Size = new_Size;
  return true;
}
