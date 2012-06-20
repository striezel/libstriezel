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

#ifndef GIFEXTENSIONS_H
#define GIFEXTENSIONS_H

#include <fstream>
#include <vector>
#include <stdint.h>
#include "DataSubBlock.h"
#include "ElementBase.h"

/* GIF extensions (can occur in version 89a) */

struct GIFExtensionBase: public GIFElementBase
{
  public:
    /* constructor */
    GIFExtensionBase();

    /* destructor */
    virtual ~GIFExtensionBase();

    virtual bool isExtension() const;
    virtual bool isTableBasedImage() const;

    /* returns the extension's label */
    uint8_t getExtensionLabel() const;

    /* tries to read a GIF extension from the given input file stream and
       returns true in case of success, false on failure.

       parameters:
           inputStream - the input file stream that is used to read the data.
                         The stream should already be opened.

       remarks:
           This function is purely virtual.
    */
    virtual bool readFromStream(std::ifstream& inputStream) =0;
  protected:
    uint8_t m_ExtensionLabel;
};//struct


struct GIFGraphicControlExtension: public GIFExtensionBase
{
  public:
    /* constructor */
    GIFGraphicControlExtension();

    /* destructor */
    virtual ~GIFGraphicControlExtension();

    /* returns the delay time that specifies the number of hundredths (1/100)
       of a second to wait before continuing with the processing
    */
    uint16_t getDelayTime() const;

    // --- packed fields
    /* returns the disposal method */
    uint8_t getDisposalMethod() const;

    /* returns true, if the user input flag is set */
    bool getUserInputFlag() const;

    /* returns true, if the transparency flag is set */
    bool getTransparentColourFlag() const;

    // --- end of packed fields

    /* returns the index of the transparent colour */
    uint8_t getTransparentColourIndex() const;

    /* tries to read a GIF extension from the given input file stream and
       returns true in case of success, false on failure.

       parameters:
           inputStream - the input file stream that is used to read the data.
                         The stream should already be opened.
    */
    virtual bool readFromStream(std::ifstream& inputStream);
  private:
    uint8_t m_PackedFields;
    uint16_t m_DelayTime;
    uint8_t m_TransparentColourIndex;
};//struct


struct GIFCommentExtension: public GIFExtensionBase
{
  public:
    /* constructor */
    GIFCommentExtension();

    /* destructor */
    virtual ~GIFCommentExtension();

    const std::vector<GIFDataSubBlock>& getCommentData() const;

    /* tries to read the GIF comment extension from the given input file stream
       and returns true in case of success, false on failure.

       parameters:
           inputStream - the input file stream that is used to read the data.
                         The stream should already be opened.
    */
    virtual bool readFromStream(std::ifstream& inputStream);
  private:
    std::vector<GIFDataSubBlock> m_CommentData;
};//struct


struct GIFPlainTextExtension: public GIFExtensionBase
{
  public:
    /* constructor */
    GIFPlainTextExtension();

    /* destructor */
    virtual ~GIFPlainTextExtension();

    /* access functions */
    uint16_t getTextGridLeftPosition() const;
    uint16_t getTextGridTopPosition() const;
    uint16_t getTextGridWidth() const;
    uint16_t getTextGridHeight() const;
    uint8_t getCharacterCellWidth() const;
    uint8_t getCharacterCellHeight() const;
    uint8_t getTextForegroundColourIndex() const;
    uint8_t getTextBackgroundColourIndex() const;
    const std::vector<GIFDataSubBlock>& getPlainTextData() const;

    /* tries to read the GIF plain text extension from the given input file
       stream and returns true in case of success, false on failure.

       parameters:
           inputStream - the input file stream that is used to read the data.
                         The stream should already be opened.
    */
    virtual bool readFromStream(std::ifstream& inputStream);
  private:
    uint16_t m_TextGridLeftPosition, m_TextGridTopPosition;
    uint16_t m_TextGridWidth, m_TextGridHeight;
    uint8_t m_CharacterCellWidth, m_CharacterCellHeight;
    uint8_t m_TextForegroundColourIndex, m_TextBackgroundColourIndex;
    std::vector<GIFDataSubBlock> m_PlainTextData;
};//struct


struct GIFApplicationExtension: public GIFExtensionBase
{
  public:
    /* constructor */
    GIFApplicationExtension();

    /* destructor */
    virtual ~GIFApplicationExtension();

    /* access functions */
    const char* getApplicationIdentifier() const;
    const char* getApplicationAuthenticationCode() const;
    const std::vector<GIFDataSubBlock>& getApplicationData() const;

    /* tries to read the GIF application extension from the given input file
       stream and returns true in case of success, false on failure.

       parameters:
           inputStream - the input file stream that is used to read the data.
                         The stream should already be opened.
    */
    virtual bool readFromStream(std::ifstream& inputStream);
  private:
    char m_ApplicationIdentifier[9];
    char m_ApplicationAuthenticationCode[4];
    std::vector<GIFDataSubBlock> m_ApplicationData;
};//struct

#endif // GIFEXTENSIONS_H
