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

#include "Extensions.h"
#include <cstring>
#include <iostream>
#include "Constants.h"

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
