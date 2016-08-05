/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2011, 2012, 2014  Dirk Stolle

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

#include "GIFStructures.hpp"
#include <iostream>
#include <cstring>
#include <cmath>
#include "Constants.hpp"
#include "Extensions.hpp"

/***** GIFHeader functions *****/

GIFHeader::GIFHeader()
: m_Version("")
{
}

GIFHeader::~GIFHeader()
{
  m_Version.clear();
}

int GIFHeader::getVersionInt() const
{
  if (m_Version.empty()) return 0;
  if (m_Version=="87a") return 87;
  if (m_Version=="89a") return 89;
  return 0;
}

const std::string& GIFHeader::getVersion() const
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
  m_Version = std::string(buffer);
  delete[] buffer;
  return true;
}

/***** GIFLogicalScreenDescriptor functions *****/

GIFLogicalScreenDescriptor::GIFLogicalScreenDescriptor()
: m_ScreenWidth(0),
  m_ScreenHeight(0),
  m_PackedFields(0),
  m_BackgroundColourIndex(0),
  m_PixelAspectRatio(0)
{
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

/***** GIFColourTable functions *****/


GIFColourTable::ColourTableEntry::ColourTableEntry()
: red(0), green(0), blue(0)
{
}

GIFColourTable::ColourTableEntry::ColourTableEntry(const uint8_t r, const uint8_t g, const uint8_t b)
: red(r), green(g), blue(b)
{
}

GIFColourTable::GIFColourTable()
: m_Table(std::vector<ColourTableEntry>())
{
}


GIFColourTable::~GIFColourTable()
{
  m_Table.clear();
}

bool GIFColourTable::empty() const
{
  return m_Table.empty();
}

void GIFColourTable::clear()
{
  m_Table.clear();
}

uint16_t GIFColourTable::getNumberOfColourEntries() const
{
  return m_Table.size();
}

bool GIFColourTable::getEntryByIndex(const uint8_t index, uint8_t& red, uint8_t& green, uint8_t& blue) const
{
  if (m_Table.empty())
  {
    std::cout << "GIFColourTable::getEntryByIndex: Error: Colour table has no entries!\n";
    return false;
  }
  if (index>=m_Table.size())
  {
    std::cout << "GIFColourTable::getEntryByIndex: Error: Colour table has not enough entries!\n";
    return false;
  }
  //table is present and has enough entries, put data into referenced vars
  red = m_Table[index].red;
  green = m_Table[index].green;
  blue = m_Table[index].blue;
  return true;
}

bool GIFColourTable::getEntryByIndex(const uint8_t index, ColourTableEntry& entry) const
{
  if (m_Table.empty())
  {
    std::cout << "GIFColourTable::getEntryByIndex: Error: Colour table has no entries!\n";
    return false;
  }
  if (index>=m_Table.size())
  {
    std::cout << "GIFColourTable::getEntryByIndex: Error: Colour table has not enough entries!\n";
    return false;
  }
  //table is present and has enough entries, put data into referenced vars
  entry = m_Table[index];
  return true;
}

const std::vector<GIFColourTable::ColourTableEntry>& GIFColourTable::getRawColourTable() const
{
  return m_Table;
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

  m_Table.clear();
  unsigned int i;
  for (i=0; i<actualTableEntries; ++i)
  {
    m_Table.push_back(ColourTableEntry(newTablePointer[3*i], newTablePointer[3*i+1], newTablePointer[3*i+2]));
  } //for
  delete[] newTablePointer;
  return true;
}


/***** GIFImageDescriptor functions *****/

GIFImageDescriptor::GIFImageDescriptor()
: m_LeftPosition(0),
  m_TopPosition(0),
  m_Width(0),
  m_Height(0),
  m_PackedFields(0)
{
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

/***** GIFTableBasedImageData functions *****/

GIFTableBasedImageData::GIFTableBasedImageData()
: m_LZW_minCodeSize(0),
  m_ImageData(std::vector<GIFDataSubBlock>())
{
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

/***** GIFTableBasedImage functions *****/

GIFTableBasedImage::GIFTableBasedImage()
: m_ImageDescriptor(GIFImageDescriptor()),
  m_LocalColourTable(GIFColourTable()),
  m_ImageData(GIFTableBasedImageData())
{
}

GIFTableBasedImage::~GIFTableBasedImage()
{
  m_LocalColourTable.clear();
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
  //there are no "valid" empty tables, i.e. empty table means no table
  return (!m_LocalColourTable.empty());
}

const GIFColourTable& GIFTableBasedImage::getLocalColourTable() const
{
  if (!m_LocalColourTable.empty()) return m_LocalColourTable;
  //We have no local colour table here, throw an exception!
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
    //... and read it
    if (!m_LocalColourTable.readFromStream(inputStream, m_ImageDescriptor.getSizeOfLocalColourTable()))
    {
      std::cout << "GIFTableBasedImage::readFromStream: Error while reading local colour table!\n";
      /* Any read data may be inconsistent, after readFromStream() failed,
         so let's clear it. */
      m_LocalColourTable.clear();
      return false;
    }
  }//if local colour table
  else
  {
    //no local colour table
    m_LocalColourTable.clear();
  }
  //image data follows
  if (!m_ImageData.readFromStream(inputStream))
  {
    std::cout << "GIFTableBasedImage::readFromStream: Error while reading table based image data!\n";
    return false;
  }
  return true;
}

/***** GIF functions *****/
GIF::GIF()
: m_Elements(std::vector<GIFElementBase*>())
{
  m_GlobalColourTable = NULL;
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
              << FileName << "\".\n";
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
