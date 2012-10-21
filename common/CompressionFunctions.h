/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011 Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#ifndef LIBTHORO_COMPRESSIONFUNCTIONS_H
#define LIBTHORO_COMPRESSIONFUNCTIONS_H

#include <stdint.h>

namespace Thoro
{

/* tries to decompress the data pointed to by compressedData and stores the
   decompressed bits in decompressionBuffer. Returns true in case of success,
   or false if an error occured.

   parameters:
       compressedData - pointer to the buffer containing the compressed data
       compressedSize - length of the buffer in bytes
       decompBuffer   - pre-allocated buffer that will hold the decompressed data
       decompSize     - size of decompBuffer
*/
bool zlibDecompress(uint8_t * compressedData, const uint32_t compressedSize, uint8_t * decompBuffer, const uint32_t decompSize);


typedef uint8_t* CompressPointer;

/* tries to compress the data pointed to by rawData and stores the compressed
   bits in compBuffer. Returns true in case of success, or false if an error
   occured.

   parameters:
       rawData    - pointer to the buffer containing the uncompressed data
       rawSize    - length of the buffer in bytes
       compBuffer - pre-allocated buffer that will hold the compressed data
       compSize   - size of compBuffer in bytes
       usedSize   - actual size of the compressed data
       level      - compression level, should be in [0;9], where 0 is no
                    compression and 9 is best compression

   remarks:
       The buffer pointed to by compBuffer will be re-allocated, if it is too
       small to hold all compressed data. The new size of the buffer will be
       stored in compSize.
       usedSize will hold the actual number of bytes that are used in that
       buffer. This value may be less than compSize.
*/
bool zlibCompress(uint8_t * rawData, const uint32_t rawSize, CompressPointer& compBuffer, uint32_t& compSize, uint32_t& usedSize, const int level=6);

} //namespace

#endif // LIBTHORO_COMPRESSIONFUNCTIONS_H
