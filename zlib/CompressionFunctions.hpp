/*
 -------------------------------------------------------------------------------
    This file is part of Thoronador's common code library.
    Copyright (C) 2011, 2015  Thoronador

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
 -------------------------------------------------------------------------------
*/

#ifndef LIBTHORO_COMPRESSIONFUNCTIONS_H
#define LIBTHORO_COMPRESSIONFUNCTIONS_H

#include <stdint.h>

namespace libthoro
{

namespace zlib
{

/* tries to decompress the data pointed to by compressedData and stores the
   decompressed bits in decompressionBuffer. Returns true in case of success,
   or false if an error occurred.

   parameters:
       compressedData - pointer to the buffer containing the compressed data
       compressedSize - length of the buffer in bytes
       decompBuffer   - pre-allocated buffer that will hold the decompressed data
       decompSize     - size of decompBuffer
*/
bool decompress(uint8_t * compressedData, const uint32_t compressedSize, uint8_t * decompBuffer, const uint32_t decompSize);


typedef uint8_t* CompressPointer;

/* tries to compress the data pointed to by rawData and stores the compressed
   bits in compBuffer. Returns true in case of success, or false if an error
   occurred.

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
bool compress(uint8_t * rawData, const uint32_t rawSize, CompressPointer& compBuffer, uint32_t& compSize, uint32_t& usedSize, const int level=6);

} //namespace zlib

} //namespace

#endif // LIBTHORO_COMPRESSIONFUNCTIONS_H
