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

#ifndef LIBSTRIEZEL_GIFDATASUBBLOCK_HPP
#define LIBSTRIEZEL_GIFDATASUBBLOCK_HPP

#include <fstream>
#include <stdint.h>

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

#endif // LIBSTRIEZEL_GIFDATASUBBLOCK_HPP
