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

#ifndef GIFSTRUCTURES_H
#define GIFSTRUCTURES_H

#include <fstream>
#include <vector>
#include <stdint.h>

struct GIFHeader
{
  public:
    /* constructor */
    GIFHeader();

    /* destructor */
    ~GIFHeader();

    /* returns the version found in the GIF header as integer. Possible values
       are 87 for 87a and 89 for 89a. A return value of zero will indicate an
       invalid or unknown version or indicate that no header has been read yet.
    */
    int getVersionInt() const;

    /* returns a constant pointer to the C string indicating the version found
       in the GIF header. Possible values are "87a" for 87a and "89a" for 89a.
       A return value of NULL will indicate an invalid or unknown version or
       indicate that no header has been read yet.
    */
    const char* getVersion() const;

    /* tries to read a GIF header from the given input file stream and returns
       true in case of success, false on failure.

       parameters:
           inputStream - the input file stream that is used to read the data.
                         The stream should already be opened.
    */
    bool readFromStream(std::ifstream& inputStream);
  private:
    char* m_Version;
}; //struct


struct GIFLogicalScreenDescriptor
{
  public:
    /* constructor */
    GIFLogicalScreenDescriptor();

    /* destructor */
    ~GIFLogicalScreenDescriptor();

    /* access functions for various data members */
    uint16_t getLogicalScreenWidth() const;
    uint16_t getLogicalScreenHeight() const;
    uint8_t getBackgroundColourIndex() const;
    uint8_t getPixelAspectRatio() const;
    // ---- packed fields
    bool getColourTableFlag() const;
    uint8_t getColourResolution() const;
    bool getSortFlag() const;
    uint8_t getSizeOfGlobalColourTable() const;

    /* tries to read a GIF's logical screen descriptor from the given input
       file stream and returns true in case of success, false on failure.

       parameters:
           inputStream - the input file stream that is used to read the data.
                         The stream should already be opened.
    */
    bool readFromStream(std::ifstream& inputStream);
  private:
    uint16_t m_ScreenWidth, m_ScreenHeight;
    uint8_t m_PackedFields, m_BackgroundColourIndex, m_PixelAspectRatio;
}; //struct


struct GIFColourTable
{
  public:
    /* constructor */
    GIFColourTable();

    /* copy constructor */
    GIFColourTable(const GIFColourTable& op);

    /* assignment operator */
    GIFColourTable& operator=(const GIFColourTable& op);

    /* destructor */
    ~GIFColourTable();

    /* returns the number of entries within the table */
    uint16_t getNumberOfColourEntries() const;

    /* returns the red, green and blue values of the index-th entry in the
       colour table in the second to fourth parameter. Index is zero-based.
       If a entry with that index exists, the function returns true. Otherwise,
       false will be returned. In the later case, red and green and blue will
       remain unchanged.

       parameters:
           index - zero-based index of the entry in the colour table
           red   - variable used to store the red component of the colour
           green - variable used to store the green component of the colour
           blue  - variable used to store the blue component of the colour
    */
    bool getEntryByIndex(const uint8_t index, uint8_t& red, uint8_t& green, uint8_t& blue) const;

    /* returns pointer to the colour table for faster access */
    const unsigned char* getColourTablePointer() const;

    /* tries to read a GIF colour table from the given input file stream and
       returns true in case of success, false on failure.

       parameters:
           inputStream       - the input file stream that is used to read the
                               data. The stream should already be opened.
           sizeOfColourTable - the size value of the colour table as read from
                               the GIF stream. The actual number of entries and
                               size will be calculated based on that value.
    */
    bool readFromStream(std::ifstream& inputStream, const uint8_t sizeOfColourTable);
  private:
    unsigned char * m_TablePointer;
    uint16_t m_TableEntries;
};//struct


struct GIFImageDescriptor
{
  public:
    /* constructor */
    GIFImageDescriptor();

    /* destructor */
    ~GIFImageDescriptor();

    /* access function */
    uint16_t getLeftPosition() const;
    uint16_t getTopPosition() const;
    uint16_t getWidth() const;
    uint16_t getHeight() const;
    // ---- packed fields
    bool getLocalColourTableFlag() const;
    bool getInterlaceFlag() const;
    bool getSortFlag() const;
    uint8_t getSizeOfLocalColourTable() const;

    /* tries to read a GIF image descriptor from the given input file stream and
       returns true in case of success, false on failure.

       parameters:
           inputStream - the input file stream that is used to read the data.
                         The stream should already be opened.
    */
    bool readFromStream(std::ifstream& inputStream);
  private:
    uint16_t m_LeftPosition;
    uint16_t m_TopPosition;
    uint16_t m_Width;
    uint16_t m_Height;
    uint8_t m_PackedFields;
}; //struct


struct GIFDataSubBlock
{
  public:
    /* constructor */
    GIFDataSubBlock();

    /* copy constructor */
    GIFDataSubBlock(const GIFDataSubBlock& op);

    /* assignment operator */
    GIFDataSubBlock& operator=(const GIFDataSubBlock& op);

    /* destructor */
    ~GIFDataSubBlock();

    /* returns the size of the data block */
    uint8_t getBlockSize() const;

    /* returns a pointer to the block data */
    const unsigned char* getBlockData() const;

    /* tries to read a GIF data sub-block from the given input file stream and
       returns true in case of success, false on failure.

       parameters:
           inputStream - the input file stream that is used to read the data.
                         The stream should already be opened.
    */
    bool readFromStream(std::ifstream& inputStream);
  private:
    uint8_t m_Size;
    unsigned char* m_DataPointer;
}; //struct


struct GIFTableBasedImageData
{
  public:
    /* constructor */
    GIFTableBasedImageData();

    /* destructor */
    ~GIFTableBasedImageData();

    /* returns the LZW minimum code size read from the stream */
    uint8_t getMinCodeSize() const;

    size_t getNumberOfSubBlocks() const;

    /* tries to read the GIF table based image data from the given input file
       stream and returns true in case of success, false on failure.

       parameters:
           inputStream - the input file stream that is used to read the data.
                         The stream should already be opened.
    */
    bool readFromStream(std::ifstream& inputStream);
  private:
    uint8_t m_LZW_minCodeSize;
    std::vector<GIFDataSubBlock> m_ImageData;
};//struct


struct GIFTableBasedImage
{
  public:
    /* constructor */
    GIFTableBasedImage();

    /* copy constructor */
    GIFTableBasedImage(const GIFTableBasedImage& op);

    /* destructor */
    ~GIFTableBasedImage();

    /* ---- access functions ---- */
    /* returns the image descriptor */
    const GIFImageDescriptor& getImageDescriptor() const;

    /* returns true, if there is a local colour table

       remarks:
           Although the image descriptor has its own flag for this, the actual
           presence of a local colour table can be different, if there were any
           errors during the loading process.
    */
    bool hasLocalColourTable() const;

    /* returns the local colour table. If no local colour table is present, the
       function will throw an exception. To avoid that, use the function
       hasLocalColourTable() first to check for the table's presence.
    */
    const GIFColourTable& getLocalColourTable() const;

    /* returns the table based image data */
    const GIFTableBasedImageData& getImageData() const;

    /* tries to read a GIF table based image from the given input file stream
       and returns true in case of success, false on failure.

       parameters:
           inputStream - the input file stream that is used to read the data.
                         The stream should already be opened.
    */
    bool readFromStream(std::ifstream& inputStream);
  private:
    GIFImageDescriptor m_ImageDescriptor;
    GIFColourTable* m_LocalColourTable;
    GIFTableBasedImageData m_ImageData;
};//struct

/* GIF extensions (can occur in version 89a) */

struct GIFExtensionBase
{
  public:
    /* constructor */
    GIFExtensionBase();

    /* destructor */
    virtual ~GIFExtensionBase();

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


/** the "actual" GIF, i.e. the class that manages all the stuff for one image **/
struct GIF
{
  public:
    /* constructor */
    GIF();

    /* destructor */
    ~GIF();

    /* returns the GIF's header */
    const GIFHeader& getHeader() const;

    /* returns the logical screen descriptor */
    const GIFLogicalScreenDescriptor& getLogicalScreenDescriptor() const;

    /* returns true, if there is a global colour table

       remarks:
           Although the local screen descriptor has its own flag for this, the
           actual presence of a global colour table can be different, if there
           were any errors during the loading process.
    */
    bool hasGlobalColourTable() const;

    /* returns the global colour table. If no global colour table is present,
       the function will throw an exception. To avoid that, use the function
       hasGlobalColourTable() first to check for the table's presence.
    */
    const GIFColourTable& getGlobalColourTable() const;

    /* returns a vector of table based images found in the file */
    const std::vector<GIFTableBasedImage*>& getImages() const;

    /* tries to read a GIF image from the given file and returns true in case
       of success, false on failure.

       parameters:
           FileName - the name of the GIF file
    */
    bool readFromFile(const std::string& FileName);
  private:
    GIFHeader m_Header;
    GIFLogicalScreenDescriptor m_LSD;
    GIFColourTable* m_GlobalColourTable;
    std::vector<GIFTableBasedImage*> m_Images;
};//struct

#endif // GIFSTRUCTURES_H
