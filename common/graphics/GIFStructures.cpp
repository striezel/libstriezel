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

#include "GIFStructures.h"
#include <iostream>
#include <cstring>
#include <cmath>

/***** GIFHeader functions *****/

GIFHeader::GIFHeader()
{
  m_Version = NULL;
}

GIFHeader::~GIFHeader()
{
  delete[] m_Version;
}

int GIFHeader::getVersionInt() const
{
  if (m_Version==NULL) return 0;
  if ((m_Version[0]=='8') and (m_Version[2]=='a'))
  {
    switch (m_Version[1])
    {
      case '7':
           return 87;
           break;
      case '9':
           return 89;
           break;
    }//swi
  }
  return 0;
}

const char* GIFHeader::getVersion() const
{
  return m_Version;
}

bool GIFHeader::readFromStream(std::ifstream& inputStream)
{
  if (!inputStream.good())
  {
    std::cout << "GIFHeader::readFromStream: Error: bad input stream!\n";
    return false;
  }
  char * buffer = new char[4];
  memset(buffer, 0, 4);
  //read signature
  inputStream.read(buffer, 3);
  if ((!inputStream.good()) or (std::string(buffer)!="GIF"))
  {
    std::cout << "GIFHeader::readFromStream: Error: stream contains invalid GIF signature!\n";
    delete[] buffer;
    return false;
  }
  //read version
  memset(buffer, 0, 4);
  inputStream.read(buffer, 3);
  if ((!inputStream.good()) or ((std::string(buffer)!="87a") and (std::string(buffer)!="89a")))
  {
    std::cout << "GIFHeader::readFromStream: Error: stream contains invalid GIF"
              << " version, only 87a and 89a are allowed.\n";
    delete[] buffer;
    return false;
  }
  //set new read version
  delete [] m_Version; //delete possibly previously allocated buffer
  m_Version = buffer;
  return true;
}

/***** GIFLogicalScreenDescriptor functions *****/

GIFLogicalScreenDescriptor::GIFLogicalScreenDescriptor()
{
  m_ScreenWidth = m_ScreenHeight = 0;
  m_PackedFields = 0;
  m_BackgroundColourIndex = 0;
  m_PixelAspectRatio = 0;
}

GIFLogicalScreenDescriptor::~GIFLogicalScreenDescriptor()
{
  /* empty */
}

uint16_t GIFLogicalScreenDescriptor::getLogicalScreenWidth() const
{
  return m_ScreenWidth;
}

uint16_t GIFLogicalScreenDescriptor::getLogicalScreenHeight() const
{
  return m_ScreenHeight;
}

uint8_t GIFLogicalScreenDescriptor::getBackgroundColourIndex() const
{
  return m_BackgroundColourIndex;
}

uint8_t GIFLogicalScreenDescriptor::getPixelAspectRatio() const
{
  return m_PixelAspectRatio;
}

bool GIFLogicalScreenDescriptor::readFromStream(std::ifstream& inputStream)
{
  if (!inputStream.good())
  {
    std::cout << "GIFLogicalScreenDescriptor::readFromStream: Error: bad input stream!\n";
    return false;
  }

  /*read Logical Screen Descriptor (7 bytes)

    contents:
        Logical Screen Width    - unsigned (16 bits)
        Logical Screen Height   - unsigned (16 bits)
        Packed Fields           - byte
        Background Colour Index - byte
        Pixel Aspect Ration     - byte
  */

  inputStream.read((char*) &m_ScreenWidth, sizeof(uint16_t));
  inputStream.read((char*) &m_ScreenHeight, sizeof(uint16_t));
  inputStream.read((char*) &m_PackedFields, sizeof(uint8_t));
  inputStream.read((char*) &m_BackgroundColourIndex, sizeof(uint8_t));
  inputStream.read((char*) &m_PixelAspectRatio, sizeof(uint8_t));
  if (!inputStream.good())
  {
    std::cout << "GIFLogicalScreenDescriptor::readFromStream: Error while "
              << "reading logical screen descriptor!\n";
    return false;
  }
  return true;
}

bool GIFLogicalScreenDescriptor::getColourTableFlag() const
{
  return ((m_PackedFields & (1<<7)) != 0);
}

uint8_t GIFLogicalScreenDescriptor::getColourResolution() const
{
  return ((m_PackedFields & ((1<<6) | (1<<5) | (1<<4))) >> 4);
}

bool GIFLogicalScreenDescriptor::getSortFlag() const
{
  return ((m_PackedFields & (1<<3)) != 0);
}

uint8_t GIFLogicalScreenDescriptor::getSizeOfGlobalColourTable() const
{
  return (m_PackedFields & (1 | (1<<1) | (1<<2)));
}

/***** GIFColourTable function *****/

GIFColourTable::GIFColourTable()
{
  m_TableEntries = 0;
  m_TablePointer = 0;
}

GIFColourTable::~GIFColourTable()
{
  m_TableEntries = 0;
  delete[] m_TablePointer;
  m_TablePointer = NULL;
}

uint16_t GIFColourTable::getNumberOfColourEntries() const
{
  return m_TableEntries;
}

bool GIFColourTable::getEntryByIndex(const uint8_t index, uint8_t& red, uint8_t& green, uint8_t& blue) const
{
  if (m_TablePointer==NULL)
  {
    std::cout << "GIFColourTable::getEntryByIndex: Error: Colour table has no entries!\n";
    return false;
  }
  if (index>=m_TableEntries)
  {
    std::cout << "GIFColourTable::getEntryByIndex: Error: Colour table has not enough entries!\n";
    return false;
  }
  //table is present and has enoun entries, put data into referenced vars
  red = m_TablePointer[index*3];
  green = m_TablePointer[index*3+1];
  blue = m_TablePointer[index*3+2];
  return true;
}

const unsigned char* GIFColourTable::getColourTablePointer() const
{
  return m_TablePointer;
}

bool GIFColourTable::readFromStream(std::ifstream& inputStream, const uint8_t sizeOfColourTable)
{
  if (!inputStream.good())
  {
    std::cout << "GIFColourTable::readFromStream: Error: bad input stream!\n";
    return false;
  }
  if (sizeOfColourTable>7)
  {
    std::cout << "GIFColourTable::readFromStream: Error: invalid size given!\n";
    return false;
  }
  const uint16_t actualTableEntries = pow(2, sizeOfColourTable+1);
  const unsigned int actualColourTableSize =3* actualTableEntries;
  //allocate new table
  unsigned char * newTablePointer = new unsigned char[actualColourTableSize];
  inputStream.read((char*) newTablePointer, actualColourTableSize);
  if (!inputStream.good())
  {
    std::cout << "GIFColourTable::readFromStream: Error while reading table data!\n";
    delete[] newTablePointer;
    return false;
  }
  m_TableEntries = 0;
  delete[] m_TablePointer;
  m_TablePointer = newTablePointer;
  m_TableEntries = actualTableEntries;
  return true;
}


/***** GIFImageDescriptor functions *****/

GIFImageDescriptor::GIFImageDescriptor()
{
  m_LeftPosition = m_TopPosition = 0;
  m_Width = m_Height = 0;
  m_PackedFields = 0;
}

GIFImageDescriptor::~GIFImageDescriptor()
{
  //empty
}

uint16_t GIFImageDescriptor::getLeftPosition() const
{
  return m_LeftPosition;
}

uint16_t GIFImageDescriptor::getTopPosition() const
{
  return m_TopPosition;
}

uint16_t GIFImageDescriptor::getWidth() const
{
  return m_Width;
}

uint16_t GIFImageDescriptor::getHeight() const
{
  return m_Height;
}

bool GIFImageDescriptor::getLocalColourTableFlag() const
{
  return ((m_PackedFields >> 7)!=0);
}

bool GIFImageDescriptor::getInterlaceFlag() const
{
  return ((m_PackedFields & (1<<6))!=0);
}

bool GIFImageDescriptor::getSortFlag() const
{
  return ((m_PackedFields & (1<<5))!=0);
}

uint8_t GIFImageDescriptor::getSizeOfLocalColourTable() const
{
  return (m_PackedFields & (1 | (1<<1) | (1<<2)));
}

bool GIFImageDescriptor::readFromStream(std::ifstream& inputStream)
{
  if (!inputStream.good())
  {
    std::cout << "GIFImageDescriptor::readFromStream: Error: bad input stream!\n";
    return false;
  }

  /*GIF image descriptor (required)

    structure:
        Image Separator     - byte
        Image Left Position - unsigned (16 bit)
        Image Top Position  - unsigned (16 bit)
        Image Width         - unsigned (16 bit)
        Image Height        - unsigned (16 bit)
        Packed Fields       - byte
  */

  uint8_t imageSep = 0;
  inputStream.read((char*) &imageSep, 1);
  if (!inputStream.good())
  {
    std::cout << "GIFImageDescriptor::readFromStream: Error: Could not read image separator!\n";
    return false;
  }
  //check for proper value
  if (imageSep!=0x2C)
  {
    std::cout << "GIFImageDescriptor::readFromStream: Error: Image separator has incorrect value!\n";
    return false;
  }
  //read other values
  inputStream.read((char*) &m_LeftPosition, sizeof(uint16_t));
  inputStream.read((char*) &m_TopPosition, sizeof(uint16_t));
  inputStream.read((char*) &m_Width, sizeof(uint16_t));
  inputStream.read((char*) &m_Height, sizeof(uint16_t));
  inputStream.read((char*) &m_PackedFields, sizeof(uint8_t));
  if (!inputStream.good())
  {
    std::cout << "GIFImageDescriptor::readFromStream: Error while reading data!\n";
    return false;
  }
  return true;
}


/***** GIFDataSubBlock functions *****/

GIFDataSubBlock::GIFDataSubBlock()
{
  m_Size = 0;
  m_DataPointer = NULL;
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


/***** GIFTableBasedImageData functions *****/

GIFTableBasedImageData::GIFTableBasedImageData()
{
  m_LZW_minCodeSize = 0;
  m_ImageData.clear();
}

GIFTableBasedImageData::~GIFTableBasedImageData()
{
  m_ImageData.clear();
}

uint8_t GIFTableBasedImageData::getMinCodeSize() const
{
  return m_LZW_minCodeSize;
}

size_t GIFTableBasedImageData::getNumberOfSubBlocks() const
{
  return m_ImageData.size();
}

bool GIFTableBasedImageData::readFromStream(std::ifstream& inputStream)
{
  if (!inputStream.good())
  {
    std::cout << "GIFTableBasedImageData::readFromStream: Error: bad input stream!\n";
    return false;
  }

  /*

      7 6 5 4 3 2 1 0        Field Name                    Type
     +---------------+
     |               |       LZW Minimum Code Size         Byte
     +---------------+

     +===============+
     |               |
     /               /       Image Data                    Data Sub-blocks
     |               |
     +===============+
  */

  inputStream.read((char*) &m_LZW_minCodeSize, 1);
  if (!inputStream.good())
  {
    std::cout << "GIFTableBasedImageData::readFromStream: Error: could not read"
              << " LZW minimum code size!\n";
    return false;
  }

  //read data sub-blocks
  m_ImageData.clear();
  GIFDataSubBlock tempBlock;
  do
  {
    //read next sub-block
    if (!tempBlock.readFromStream(inputStream))
    {
      std::cout << "GIFTableBasedImageData::readFromStream: Error: could not "
                << "read all data sub-blocks!\n";
      return false;
    }
    //only push if it is not the block terminator
    if (tempBlock.getBlockSize()!=0)
    {
      m_ImageData.push_back(tempBlock);
    }
  } while (tempBlock.getBlockSize()!=0);
  return inputStream.good();
}
