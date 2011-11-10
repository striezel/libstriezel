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

// constants
const uint8_t cGIFImageSeparator = 0x2C;
const uint8_t cGIFExtensionIntroducer = 0x21;
// ---- graphic control extension
const uint8_t cGIFGraphicControlLabel = 0xF9;
// ---- comment extension
const uint8_t cGIFCommentLabel = 0xFE;
// ---- plain text extension
const uint8_t cGIFPlainTextLabel = 0x01;
// ---- application extension
const uint8_t cGIFApplicationExtensionLabel = 0xFF;
const uint8_t cGIFTrailer = 0x3B;

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
  m_TablePointer = NULL;
}

GIFColourTable::GIFColourTable(const GIFColourTable& op)
{
  m_TableEntries = op.getNumberOfColourEntries();
  if (m_TableEntries!=0)
  {
    m_TablePointer = new unsigned char [m_TableEntries*3];
    memcpy(m_TablePointer, op.getColourTablePointer(), m_TableEntries*3);
  }
  else
  {
    m_TablePointer = NULL;
  }
}

GIFColourTable& GIFColourTable::operator=(const GIFColourTable& op)
{
  //avoid self-assignment
  if (this == &op) return *this;
  delete m_TablePointer;
  m_TableEntries = op.getNumberOfColourEntries();
  if (m_TableEntries!=0)
  {
    m_TablePointer = new unsigned char [m_TableEntries*3];
    memcpy(m_TablePointer, op.getColourTablePointer(), m_TableEntries*3);
  }
  else
  {
    m_TablePointer = NULL;
  }
  return *this;
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
  const unsigned int actualColourTableSize = 3 * actualTableEntries;
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
  if (imageSep!=cGIFImageSeparator)
  {
    std::cout << "GIFImageDescriptor::readFromStream: Error: Image separator has "
              << "incorrect value! Current value is "<<(int) imageSep
              <<", but only "<<(int) cGIFImageSeparator<<" is allowed!\n";
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

const std::vector<GIFDataSubBlock>& GIFTableBasedImageData::getBlocks() const
{
  return m_ImageData;
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

/***** GIFElementBase functions *****/

GIFElementBase::GIFElementBase()
{
  //empty
}

GIFElementBase::~GIFElementBase()
{
  //empty
}

/***** GIFTableBasedImage functions *****/

GIFTableBasedImage::GIFTableBasedImage()
{
  m_LocalColourTable = NULL;
}

GIFTableBasedImage::GIFTableBasedImage(const GIFTableBasedImage& op)
{
  m_ImageDescriptor = op.getImageDescriptor();
  m_ImageData = op.getImageData();
  delete m_LocalColourTable;
  if (op.hasLocalColourTable())
  {
    *m_LocalColourTable = op.getLocalColourTable();
  }
  else
  {
    m_LocalColourTable = NULL;
  }
}

GIFTableBasedImage::~GIFTableBasedImage()
{
  delete m_LocalColourTable;
}

bool GIFTableBasedImage::isExtension() const
{
  return false;
}

bool GIFTableBasedImage::isTableBasedImage() const
{
  return true;
}

const GIFImageDescriptor& GIFTableBasedImage::getImageDescriptor() const
{
  return m_ImageDescriptor;
}

bool GIFTableBasedImage::hasLocalColourTable() const
{
  return (m_LocalColourTable!=NULL);
}

const GIFColourTable& GIFTableBasedImage::getLocalColourTable() const
{
  if (m_LocalColourTable!=NULL) return *m_LocalColourTable;
  //We have no local colour table here, throw up!
  std::cout << "GIFTableBasedImage::getLocalColourTable: Error: there is no local colour table!\n";
  std::cout.flush();
  throw std::string("GIFTableBasedImage::getLocalColourTable: Error: requesting non-existing colour table.");
}

const GIFTableBasedImageData& GIFTableBasedImage::getImageData() const
{
  return m_ImageData;
}

bool GIFTableBasedImage::readFromStream(std::ifstream& inputStream)
{
  if (!inputStream.good())
  {
    std::cout << "GIFTableBasedImage::readFromStream: Error: bad input stream!\n";
    return false;
  }
  //read image descriptor
  if (!m_ImageDescriptor.readFromStream(inputStream))
  {
    std::cout << "GIFTableBasedImage::readFromStream: Error while reading image descriptor!\n";
    return false;
  }
  //check for local colour table
  if (m_ImageDescriptor.getLocalColourTableFlag())
  {
    GIFColourTable * newColourTable = new GIFColourTable;
    //... and read it
    if (!(newColourTable->readFromStream(inputStream, m_ImageDescriptor.getSizeOfLocalColourTable())))
    {
      std::cout << "GIFTableBasedImage::readFromStream: Error while reading local colour table!\n";
      delete newColourTable;
      return false;
    }
  }//if local colour table
  else
  {
    //no local colour table
    delete m_LocalColourTable;
    m_LocalColourTable = NULL;
  }
  //image data follows
  if (!m_ImageData.readFromStream(inputStream))
  {
    std::cout << "GIFTableBasedImage::readFromStream: Error while reading table based image data!\n";
    return false;
  }
  return true;
}

/****************************************
 ***** GIF extensions (version 89a) *****
 ****************************************/

/***** GIFExtensionBase *****/

GIFExtensionBase::GIFExtensionBase()
{
  m_ExtensionLabel = 0;
}

GIFExtensionBase::~GIFExtensionBase()
{
  //empty
}

bool GIFExtensionBase::isExtension() const
{
  return true;
}

bool GIFExtensionBase::isTableBasedImage() const
{
  return false;
}

uint8_t GIFExtensionBase::getExtensionLabel() const
{
  return m_ExtensionLabel;
}

/***** GIFGraphicControlExtension *****/

GIFGraphicControlExtension::GIFGraphicControlExtension()
: GIFExtensionBase()
{
  m_PackedFields = 0;
  m_DelayTime = 0;
  m_TransparentColourIndex = 0;
}

GIFGraphicControlExtension::~GIFGraphicControlExtension()
{
  //empty
}

uint16_t GIFGraphicControlExtension::getDelayTime() const
{
  return m_DelayTime;
}

uint8_t GIFGraphicControlExtension::getDisposalMethod() const
{
  return ((m_PackedFields >> 2) & (1 | (1<<1) | (1<<2)));
}

bool GIFGraphicControlExtension::getUserInputFlag() const
{
  return ((m_PackedFields & (1<<1)) !=0);
}

bool GIFGraphicControlExtension::getTransparentColourFlag() const
{
  return ((m_PackedFields & 1)!=0);
}

uint8_t GIFGraphicControlExtension::getTransparentColourIndex() const
{
  return m_TransparentColourIndex;
}

bool GIFGraphicControlExtension::readFromStream(std::ifstream& inputStream)
{
  if (!inputStream.good())
  {
    std::cout << "GIFGraphicControlExtension::readFromStream: Error: bad input stream!\n";
    return false;
  }
  /*
      7 6 5 4 3 2 1 0        Field Name                    Type
     +---------------+
  0  |               |       Extension Introducer          Byte
     +---------------+
  1  |               |       Graphic Control Label         Byte
     +---------------+

     +---------------+
  0  |               |       Block Size                    Byte
     +---------------+
  1  |     |     | | |       <Packed Fields>               See somewhere else
     +---------------+
  2  |               |       Delay Time                    Unsigned
     +-             -+
  3  |               |
     +---------------+
  4  |               |       Transparent Color Index       Byte
     +---------------+

     +---------------+
  0  |               |       Block Terminator              Byte
     +---------------+
  */

  uint8_t byte = 0;
  inputStream.read((char*) &byte, 1);
  if (!inputStream.good())
  {
    std::cout << "GIFGraphicControlExtension::readFromStream: Error: could not "
              << "read extension introducer!\n";
    return false;
  }
  //check for proper value
  if (byte!=cGIFExtensionIntroducer)
  {
    std::cout << "GIFGraphicControlExtension::readFromStream: Error: extension "
              << "introducer has value "<<(int) byte <<", but only "
              << (int)cGIFExtensionIntroducer<<" is allowed here!\n";
    return false;
  }
  //read label
  inputStream.read((char*) &byte, 1);
  if (!inputStream.good())
  {
    std::cout << "GIFGraphicControlExtension::readFromStream: Error: could not "
              << "read graphic control label!\n";
    return false;
  }
  //check value
  if (byte!=cGIFGraphicControlLabel)
  {
    std::cout << "GIFGraphicControlExtension::readFromStream: Error: extension "
              << "label has value "<<(int) byte <<", but only "
              << (int)cGIFGraphicControlLabel<<" is allowed here!\n";
    return false;
  }
  m_ExtensionLabel = cGIFGraphicControlLabel;

  //read block size
  inputStream.read((char*) &byte, 1);
  if (!inputStream.good())
  {
    std::cout << "GIFGraphicControlExtension::readFromStream: Error: could not "
              << "read block size!\n";
    return false;
  }
  if (byte!=4)
  {
    std::cout << "GIFGraphicControlExtension::readFromStream: Error: block size"
              << " has value "<<(int) byte <<", but only 4 is allowed here!\n";
    return false;
  }
  //read packed fields
  inputStream.read((char*) &byte, sizeof(uint8_t));
  //read delay time
  inputStream.read((char*) &m_DelayTime, sizeof(uint16_t));
  //read transparency index
  inputStream.read((char*) &m_TransparentColourIndex, sizeof(uint8_t));
  if (!inputStream.good())
  {
    std::cout << "GIFGraphicControlExtension::readFromStream: Error: could not "
              << "read extension data block!\n";
    return false;
  }
  //read block terminator
  inputStream.read((char*) &byte, 1);
  if (!inputStream.good())
  {
    std::cout << "GIFGraphicControlExtension::readFromStream: Error: could not "
              << "read block terminator!\n";
    return false;
  }
  if (byte!=0)
  {
    std::cout << "GIFGraphicControlExtension::readFromStream: Error: block "
              << "terminator has value "<<(int) byte <<", but only zero is allowed here!\n";
    return false;
  }
  return true;
}

/***** GIFCommentExtension *****/

GIFCommentExtension::GIFCommentExtension()
: GIFExtensionBase()
{
  m_CommentData.clear();
}

GIFCommentExtension::~GIFCommentExtension()
{
  m_CommentData.clear();
}

const std::vector<GIFDataSubBlock>& GIFCommentExtension::getCommentData() const
{
  return m_CommentData;
}

bool GIFCommentExtension::readFromStream(std::ifstream& inputStream)
{
  if (!inputStream.good())
  {
    std::cout << "GIFCommentExtension::readFromStream: Error: bad input stream!\n";
    return false;
  }
  /*
      7 6 5 4 3 2 1 0        Field Name                    Type
     +---------------+
  0  |               |       Extension Introducer          Byte
     +---------------+
  1  |               |       Comment Label                 Byte
     +---------------+

     +===============+
     |               |
  N  |               |       Comment Data                  Data Sub-blocks
     |               |
     +===============+

     +---------------+
  0  |               |       Block Terminator              Byte
     +---------------+
  */

  uint8_t byte = 0;
  inputStream.read((char*) &byte, 1);
  if (!inputStream.good())
  {
    std::cout << "GIFCommentExtension::readFromStream: Error: could not "
              << "read extension introducer!\n";
    return false;
  }
  //check for proper value
  if (byte!=cGIFExtensionIntroducer)
  {
    std::cout << "GIFCommentExtension::readFromStream: Error: extension "
              << "introducer has value "<<(int) byte <<", but only "
              << (int)cGIFExtensionIntroducer<<" is allowed here!\n";
    return false;
  }
  //read label
  inputStream.read((char*) &byte, 1);
  if (!inputStream.good())
  {
    std::cout << "GIFCommentExtension::readFromStream: Error: could not "
              << "read comment label!\n";
    return false;
  }
  //check value
  if (byte!=cGIFCommentLabel)
  {
    std::cout << "GIFCommentExtension::readFromStream: Error: extension "
              << "label has value "<<(int) byte <<", but only "
              << (int)cGIFCommentLabel<<" is allowed here!\n";
    return false;
  }
  m_ExtensionLabel = cGIFCommentLabel;

  //read data sub-blocks
  GIFDataSubBlock tempBlock;
  m_CommentData.clear();
  do
  {
    if (!tempBlock.readFromStream(inputStream))
    {
      std::cout << "GIFCommentExtension::readFromStream: Error: could not "
                << "read data sub-block for comment data!\n";
      return false;
    }
    if (tempBlock.getBlockSize()!=0)
    {
      m_CommentData.push_back(tempBlock);
    }
  } while (tempBlock.getBlockSize()!=0);
  return inputStream.good();
}

/** GIFPlainTextExtension functions **/

GIFPlainTextExtension::GIFPlainTextExtension()
{
  m_TextGridLeftPosition = m_TextGridTopPosition = 0;
  m_TextGridWidth = m_TextGridHeight = 0;
  m_CharacterCellWidth = m_CharacterCellHeight = 0;
  m_TextBackgroundColourIndex = m_TextForegroundColourIndex = 0;
  m_PlainTextData.clear();
}

GIFPlainTextExtension::~GIFPlainTextExtension()
{
  m_PlainTextData.clear();
}

uint16_t GIFPlainTextExtension::getTextGridLeftPosition() const
{
  return m_TextGridLeftPosition;
}

uint16_t GIFPlainTextExtension::getTextGridTopPosition() const
{
  return m_TextGridTopPosition;
}

uint16_t GIFPlainTextExtension::getTextGridWidth() const
{
  return m_TextGridWidth;
}

uint16_t GIFPlainTextExtension::getTextGridHeight() const
{
  return m_TextGridHeight;
}

uint8_t GIFPlainTextExtension::getCharacterCellWidth() const
{
  return m_CharacterCellWidth;
}

uint8_t GIFPlainTextExtension::getCharacterCellHeight() const
{
  return m_CharacterCellHeight;
}

uint8_t GIFPlainTextExtension::getTextForegroundColourIndex() const
{
  return m_TextForegroundColourIndex;
}

uint8_t GIFPlainTextExtension::getTextBackgroundColourIndex() const
{
  return m_TextBackgroundColourIndex;
}

const std::vector<GIFDataSubBlock>& GIFPlainTextExtension::getPlainTextData() const
{
  return m_PlainTextData;
}

bool GIFPlainTextExtension::readFromStream(std::ifstream& inputStream)
{
  if (!inputStream.good())
  {
    std::cout << "GIFPlainTextExtension::readFromStream: Error: bad input stream!\n";
    return false;
  }

  /*
      7 6 5 4 3 2 1 0        Field Name                    Type
     +---------------+
  0  |               |       Extension Introducer          Byte
     +---------------+
  1  |               |       Plain Text Label              Byte
     +---------------+

     +---------------+
  0  |               |       Block Size                    Byte
     +---------------+
  1  |               |       Text Grid Left Position       Unsigned
     +-             -+
  2  |               |
     +---------------+
  3  |               |       Text Grid Top Position        Unsigned
     +-             -+
  4  |               |
     +---------------+
  5  |               |       Text Grid Width               Unsigned
     +-             -+
  6  |               |
     +---------------+
  7  |               |       Text Grid Height              Unsigned
     +-             -+
  8  |               |
     +---------------+
  9  |               |       Character Cell Width          Byte
     +---------------+
 10  |               |       Character Cell Height         Byte
     +---------------+
 11  |               |       Text Foreground Color Index   Byte
     +---------------+
 12  |               |       Text Background Color Index   Byte
     +---------------+

     +===============+
     |               |
  N  |               |       Plain Text Data               Data Sub-blocks
     |               |
     +===============+

     +---------------+
  0  |               |       Block Terminator              Byte
     +---------------+
  */

  uint8_t byte = 0;
  inputStream.read((char*) &byte, 1);
  if (!inputStream.good())
  {
    std::cout << "GIFPlainTextExtension::readFromStream: Error: could not "
              << "read extension introducer!\n";
    return false;
  }
  //check for proper value
  if (byte!=cGIFExtensionIntroducer)
  {
    std::cout << "GIFPlainTextExtension::readFromStream: Error: extension "
              << "introducer has value "<<(int) byte <<", but only "
              << (int)cGIFExtensionIntroducer<<" is allowed here!\n";
    return false;
  }
  //read label
  inputStream.read((char*) &byte, 1);
  if (!inputStream.good())
  {
    std::cout << "GIFPlainTextExtension::readFromStream: Error: could not "
              << "read plain text label!\n";
    return false;
  }
  //check value
  if (byte!=cGIFPlainTextLabel)
  {
    std::cout << "GIFPlainTextExtension::readFromStream: Error: extension "
              << "label has value "<<(int) byte <<", but only "
              << (int)cGIFPlainTextLabel<<" is allowed here!\n";
    return false;
  }
  m_ExtensionLabel = cGIFPlainTextLabel;
  //read block size
  inputStream.read((char*) &byte, 1);
  if (!inputStream.good())
  {
    std::cout << "GIFPlainTextExtension::readFromStream: Error: could not "
              << "read block size!\n";
    return false;
  }
  if (byte!=12)
  {
    std::cout << "GIFPlainTextExtension::readFromStream: Error: block size"
              << " has value "<<(int) byte <<", but only 12 is allowed here!\n";
    return false;
  }
  //read block data
  inputStream.read((char*) &m_TextGridLeftPosition, sizeof(uint16_t));
  inputStream.read((char*) &m_TextGridTopPosition, sizeof(uint16_t));
  inputStream.read((char*) &m_TextGridWidth, sizeof(uint16_t));
  inputStream.read((char*) &m_TextGridHeight, sizeof(uint16_t));
  inputStream.read((char*) &m_CharacterCellWidth, sizeof(uint8_t));
  inputStream.read((char*) &m_CharacterCellHeight, sizeof(uint8_t));
  inputStream.read((char*) &m_TextForegroundColourIndex, sizeof(uint8_t));
  inputStream.read((char*) &m_TextBackgroundColourIndex, sizeof(uint8_t));
  if (!inputStream.good())
  {
    std::cout << "GIFPlainTextExtension::readFromStream: Error: could not "
              << "read extension data!\n";
    return false;
  }
  //read data sub-blocks
  m_PlainTextData.clear();
  GIFDataSubBlock tempBlock;
  do
  {
    if (!tempBlock.readFromStream(inputStream))
    {
      std::cout << "GIFPlainTextExtension::readFromStream: Error: could not "
                << "read data sub-blocks!\n";
      return false;
    }
    if (tempBlock.getBlockSize()!=0)
    {
      m_PlainTextData.push_back(tempBlock);
    }
  } while (tempBlock.getBlockSize()!=0);
  return inputStream.good();
}

/** GIFApplicationExtension functions **/

GIFApplicationExtension::GIFApplicationExtension()
{
  memset(m_ApplicationIdentifier, 0, 9);
  memset(m_ApplicationAuthenticationCode, 0, 4);
  m_ApplicationData.clear();
}

GIFApplicationExtension::~GIFApplicationExtension()
{
  m_ApplicationData.clear();
}

const char* GIFApplicationExtension::getApplicationIdentifier() const
{
  return m_ApplicationIdentifier;
}

const char* GIFApplicationExtension::getApplicationAuthenticationCode() const
{
  return m_ApplicationAuthenticationCode;
}

const std::vector<GIFDataSubBlock>& GIFApplicationExtension::getApplicationData() const
{
  return m_ApplicationData;
}

bool GIFApplicationExtension::readFromStream(std::ifstream& inputStream)
{
  if (!inputStream.good())
  {
    std::cout << "GIFApplicationExtension::readFromStream: Error: bad input stream!\n";
    return false;
  }

  /*
      7 6 5 4 3 2 1 0        Field Name                    Type
     +---------------+
  0  |               |       Extension Introducer          Byte
     +---------------+
  1  |               |       Extension Label               Byte
     +---------------+

     +---------------+
  0  |               |       Block Size                    Byte
     +---------------+
  1  |               |
     +-             -+
  2  |               |
     +-             -+
  3  |               |       Application Identifier        8 Bytes
     +-             -+
  4  |               |
     +-             -+
  5  |               |
     +-             -+
  6  |               |
     +-             -+
  7  |               |
     +-             -+
  8  |               |
     +---------------+
  9  |               |
     +-             -+
 10  |               |       Appl. Authentication Code     3 Bytes
     +-             -+
 11  |               |
     +---------------+

     +===============+
     |               |
     |               |       Application Data              Data Sub-blocks
     |               |
     |               |
     +===============+

     +---------------+
  0  |               |       Block Terminator              Byte
     +---------------+
  */

  uint8_t byte = 0;
  inputStream.read((char*) &byte, 1);
  if (!inputStream.good())
  {
    std::cout << "GIFApllicationExtension::readFromStream: Error: could not "
              << "read extension introducer!\n";
    return false;
  }
  //check for proper value
  if (byte!=cGIFExtensionIntroducer)
  {
    std::cout << "GIFApplicationExtension::readFromStream: Error: extension "
              << "introducer has value "<<(int) byte <<", but only "
              << (int)cGIFExtensionIntroducer<<" is allowed here!\n";
    return false;
  }
  //read label
  inputStream.read((char*) &byte, 1);
  if (!inputStream.good())
  {
    std::cout << "GIFApplicationExtension::readFromStream: Error: could not "
              << "read application extension label!\n";
    return false;
  }
  //check value
  if (byte!=cGIFApplicationExtensionLabel)
  {
    std::cout << "GIFApplicationExtension::readFromStream: Error: extension "
              << "label has value "<<(int) byte <<", but only "
              << (int)cGIFApplicationExtensionLabel<<" is allowed here!\n";
    return false;
  }
  m_ExtensionLabel = cGIFApplicationExtensionLabel;
  //read block size
  inputStream.read((char*) &byte, 1);
  if (!inputStream.good())
  {
    std::cout << "GIFApplicationExtension::readFromStream: Error: could not "
              << "read block size!\n";
    return false;
  }
  if (byte!=11)
  {
    std::cout << "GIFPlainTextExtension::readFromStream: Error: block size"
              << " has value "<<(int) byte <<", but only 12 is allowed here!\n";
    return false;
  }
  //read application identifier
  memset(m_ApplicationIdentifier, 0, 9);
  inputStream.read(m_ApplicationIdentifier, 8);
  if (!inputStream.good())
  {
    std::cout << "GIFApplicationExtension::readFromStream: Error: could not "
              << "read application identifier!\n";
    return false;
  }
  //read application authentication code
  memset(m_ApplicationAuthenticationCode, 0, 4);
  inputStream.read(m_ApplicationAuthenticationCode, 3);
  if (!inputStream.good())
  {
    std::cout << "GIFApplicationExtension::readFromStream: Error: could not "
              << "read application authentication code!\n";
    return false;
  }
  //read data sub-blocks
  m_ApplicationData.clear();
  GIFDataSubBlock tempBlock;
  do
  {
    if (!tempBlock.readFromStream(inputStream))
    {
      std::cout << "GIFApplicationExtension::readFromStream: Error: could not "
                << "read data sub-blocks!\n";
      return false;
    }
    if (tempBlock.getBlockSize()!=0)
    {
      m_ApplicationData.push_back(tempBlock);
    }
  } while (tempBlock.getBlockSize()!=0);
  return inputStream.good();
}

/***** GIF functions *****/
GIF::GIF()
{
  m_GlobalColourTable = NULL;
  m_Elements.clear();
}

GIF::~GIF()
{
  delete m_GlobalColourTable;
  m_GlobalColourTable = NULL;
  while (!m_Elements.empty())
  {
    GIFElementBase* elemPtr = m_Elements.back();
    m_Elements.pop_back();
    delete elemPtr;
  }//while
  m_Elements.clear();
}

const GIFHeader& GIF::getHeader() const
{
  return m_Header;
}

const GIFLogicalScreenDescriptor& GIF::getLogicalScreenDescriptor() const
{
  return m_LSD;
}

bool GIF::hasGlobalColourTable() const
{
  //return m_LSD.getColourTableFlag();
  return (m_GlobalColourTable!=NULL);
}

const GIFColourTable& GIF::getGlobalColourTable() const
{
  if (m_GlobalColourTable!=NULL) return *m_GlobalColourTable;
  //We have no global colour table here, throw up!
  std::cout << "GIF::getGlobalColourTable: Error: there is no global colour table!\n";
  std::cout.flush();
  throw std::string("GIF::getGlobalColourTable: Error: requesting non-existing colour table.");
}

const std::vector<GIFElementBase*>& GIF::getElements() const
{
  return m_Elements;
}

bool GIF::readFromFile(const std::string& FileName, const bool onlyReadFirstImage)
{
  #warning Not completely implemented yet!
  std::ifstream input;
  input.open(FileName.c_str(), std::ifstream::in | std::ifstream::binary);
  if (!input)
  {
    std::cout << "GIF::readFromFile: Error: Could not open file \""<<FileName
              << "\" for reading.\n";
    return false;
  }
  /* basic GIF structure

     1 x header

     1 x logical screen descriptor

         -> 1 x (global) colour table, if flag in LSD indicates its presence

     n x table based image data (can occur 0+ times)

     1 x trailer
  */

  //header comes first, always
  if (!m_Header.readFromStream(input))
  {
    std::cout << "GIF::readFromFile: Error while reading GIF header of file \""
              << FileName << "\" for reading.\n";
    input.close();
    return false;
  }
  //logical screen descriptor is next
  if (!m_LSD.readFromStream(input))
  {
    std::cout << "GIF::readFromFile: Error while reading logical screen "
              << "descriptor of file \"" << FileName << "\".\n";
    input.close();
    return false;
  }
  //check for presence of global colour table, and if there's one, load it
  if (m_LSD.getColourTableFlag())
  {
    //load global colour table
    GIFColourTable * newGlobalColourTable = new GIFColourTable;
    if (!(newGlobalColourTable->readFromStream(input, m_LSD.getSizeOfGlobalColourTable())))
    {
      std::cout << "GIF::readFromFile: Error while reading global colour table"
                << " of file \"" << FileName << "\".\n";
      delete newGlobalColourTable;
      input.close();
      return false;
    }
    //table was read successfully, replace it with the current one
    delete m_GlobalColourTable;
    m_GlobalColourTable = newGlobalColourTable;
  }//if global colour table present
  else
  {
    //there is no global colour table
    delete m_GlobalColourTable;
    m_GlobalColourTable = NULL;
  }

  //now the image data of the first image
  m_Elements.clear();
  GIFTableBasedImage * tempImg = NULL;
  GIFExtensionBase * tempExt = NULL;
  //potential endless loop
  while (true)
  {
    uint8_t peekByte = 0, secondPeek = 0;
    input.read((char*) &peekByte, 1);
    if (!input)
    {
      std::cout <<"GIF::readFromFile: Error: Couldn't read next byte of file \""
                << FileName<< "\".\n";
      input.close();
      return false;
    }

    switch (peekByte)
    {
      case cGIFImageSeparator:
           input.seekg(-1, std::ios::cur);
           tempImg = new GIFTableBasedImage;
           if (tempImg->readFromStream(input))
           {
             m_Elements.push_back(tempImg);
           }
           else
           {
             std::cout << "GIF::readFromFile: Error while reading first table "
                       << "based image of file \"" << FileName << "\".\n";
             delete tempImg;
             input.close();
             return false;
           }
           if (onlyReadFirstImage)
           {
             std::cout << "GIF::readFromFile: Warning: only one image read from file \""
                       << FileName << "\", but there may be more here.\n";
             input.close();
             return true;
           }//if
           break;
      case cGIFExtensionIntroducer:
           //we've got an extension, but which one?
           input.read((char*) &secondPeek, 1);
           if (!input)
           {
             std::cout <<"GIF::readFromFile: Error: Couldn't read next byte of file \""
                       << FileName<< "\".\n";
             input.close();
             return false;
           }
           //switch again
           switch (secondPeek)
           {
             case cGIFGraphicControlLabel:
                  tempExt = new GIFGraphicControlExtension;
                  break;
             case cGIFCommentLabel:
                  tempExt = new GIFCommentExtension;
                  break;
             case cGIFPlainTextLabel:
                  tempExt = new GIFPlainTextExtension;
                  break;
             case cGIFApplicationExtensionLabel:
                  tempExt = new GIFApplicationExtension;
                  break;
             default:
                  std::cout << "GIF::readFromFile: Error: unknown extension "
                            << "found in file \"" << FileName << "\". Label is "
                            << (int)secondPeek<<".\n";
                  input.close();
                  return false;
                  break;
           }//swi (inner; extensions)
           input.seekg(-2, std::ios::cur);
           if (!tempExt->readFromStream(input))
           {
             std::cout << "GIF::readFromFile: Error while reading extension "
                       << "data from in file \"" << FileName << "\".\n";
             delete tempExt;
             input.close();
             return false;
           }
           std::cout << "Debug: Successfully read one extension (label: "
                     << (int) (tempExt->getExtensionLabel()) << ").\n";
           if (tempExt->getExtensionLabel()==cGIFApplicationExtensionLabel)
           {
             GIFApplicationExtension * appExtPtr = static_cast<GIFApplicationExtension*> (tempExt);
             std::cout << "  app. id.: \""<< appExtPtr->getApplicationIdentifier()
                       <<"\", app. code: \""<< appExtPtr->getApplicationAuthenticationCode() <<"\".\n";
           }
           m_Elements.push_back(tempExt);
           break;
      case cGIFTrailer:
           //trailer indicates end of file, exit gracefully
           input.close();
           return true;
           break;
      default:
           std::cout << "GIF::readFromFile: Error: unknown introducer found "
                     << "found in file \"" << FileName << "\". Value is "
                     << (int)secondPeek<<".\n";
           input.close();
           return false;
           break;
    }//swi
  }//while (endless)
  std::cout << "GIF::readFromFile: Warning: you should not be here.\n";
  input.close();
  return false;
}
