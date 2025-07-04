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

#ifndef LIBSTRIEZEL_GIFSTRUCTURES_HPP
#define LIBSTRIEZEL_GIFSTRUCTURES_HPP

#include <cstdint>
#include <fstream>
#include <string>
#include <vector>
#include "ElementBase.hpp"
#include "DataSubBlock.hpp"

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

    /* returns a reference to a C++ string indicating the version found
       in the GIF header. Possible values are "87a" for 87a and "89a" for 89a.
       An empty string will indicate an invalid or unknown version or
       indicate that no header has been read yet.
    */
    const std::string& getVersion() const;

    /* tries to read a GIF header from the given input file stream and returns
       true in case of success, false on failure.

       parameters:
           inputStream - the input file stream that is used to read the data.
                         The stream should already be opened.
    */
    bool readFromStream(std::ifstream& inputStream);
  private:
    std::string m_Version;
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
    /* structure to hold a table entry */
    struct ColourTableEntry
    {
      uint8_t red;
      uint8_t green;
      uint8_t blue;

      /* default constructor - initializes all members with zero */
      ColourTableEntry();

      /* alternative constructor - presets members with given values */
      ColourTableEntry(const uint8_t r, const uint8_t g, const uint8_t b);
    }; //struct

    /* constructor */
    GIFColourTable();

    /* destructor */
    ~GIFColourTable();

    /* checks whether the colour table is empty */
    bool empty() const;

    /* clears the colour table, i.e. removes all entries */
    void clear();

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

    /* returns the red, green and blue values of the index-th entry in the
       colour table in the second parameter. Index is zero-based.
       If a entry with that index exists, the function returns true. Otherwise,
       false will be returned. In the later case, the parameter entry will
       remain unchanged.

       parameters:
           index - zero-based index of the entry in the colour table
           entry - variable used to store the RGB components of the colour
    */
    bool getEntryByIndex(const uint8_t index, ColourTableEntry& entry) const;

    /* returns pointer to the colour table for faster access */
    const std::vector<ColourTableEntry>& getRawColourTable() const;

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
    std::vector<ColourTableEntry> m_Table;
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


struct GIFTableBasedImageData
{
  public:
    /* constructor */
    GIFTableBasedImageData();

    /* destructor */
    ~GIFTableBasedImageData();

    /* returns the LZW minimum code size read from the stream */
    uint8_t getMinCodeSize() const;

    /* returns the number of data sub-blocks */
    size_t getNumberOfSubBlocks() const;

    /* access to the data sub-blocks */
    const std::vector<GIFDataSubBlock>& getBlocks() const;

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


struct GIFTableBasedImage: public GIFElementBase
{
  public:
    /* constructor */
    GIFTableBasedImage();

    /* copy constructor */
    GIFTableBasedImage(const GIFTableBasedImage& op);

    /* destructor */
    ~GIFTableBasedImage();

    virtual bool isExtension() const;
    virtual bool isTableBasedImage() const;

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
    GIFColourTable m_LocalColourTable;
    GIFTableBasedImageData m_ImageData;
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

    /* returns a vector of elements (extensions and table based images) found
       in the file
    */
    const std::vector<GIFElementBase*>& getElements() const;

    /* tries to read a GIF image from the given file and returns true in case
       of success, false on failure.

       parameters:
           FileName           - the name of the GIF file
           onlyReadFirstImage - if set to true, the read function will exit
                                after having read the first table based image
                                from the GIF stream
    */
    bool readFromFile(const std::string& FileName, const bool onlyReadFirstImage=false);
  private:
    GIFHeader m_Header;
    GIFLogicalScreenDescriptor m_LSD;
    GIFColourTable* m_GlobalColourTable;
    std::vector<GIFElementBase*> m_Elements;
};//struct

#endif // LIBSTRIEZEL_GIFSTRUCTURES_HPP
