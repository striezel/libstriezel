/*
 -------------------------------------------------------------------------------
    This file is part of striezel's common code library.
    Copyright (C) 2011, 2015, 2021, 2023  Dirk Stolle

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

#ifndef LIBSTRIEZEL_COMPRESSIONFUNCTIONS_HPP
#define LIBSTRIEZEL_COMPRESSIONFUNCTIONS_HPP

#include <cstdint>

namespace libstriezel::zlib
{

/** Tries to decompress the data pointed to by compressedData and stores the
 * decompressed bits in decompBuffer.
 *
 * \param compressedData   pointer to the buffer containing the compressed data
 * \param compressedSize   length of the compressed data buffer in bytes
 * \param decompBuffer     pre-allocated buffer that will hold the decompressed data
 * \param decompSize       size of decompBuffer in bytes
 * \return Returns true in case of success, or false if an error occurred.
 */
bool decompress(uint8_t * compressedData, const uint32_t compressedSize, uint8_t * decompBuffer, const uint32_t decompSize);


typedef uint8_t* CompressPointer;

/** Tries to compress the data pointed to by rawData and stores the compressed
 * bits in compBuffer.
 *
 * \param rawData     pointer to the buffer containing the uncompressed data
 * \param rawSize     length of the buffer in bytes
 * \param compBuffer  pre-allocated buffer that will hold the compressed data
 * \param compSize    size of compBuffer in bytes
 * \param usedSize    actual size of the compressed data
 * \param level       compression level, should be in [0;9], where 0 is no
 *                    compression and 9 is best compression
 * \return  Returns true in case of success, or false if an error occurred.
 * \remarks The buffer pointed to by compBuffer will be re-allocated, if it is
 * too small to hold all compressed data. The new size of the buffer will be
 * stored in compSize. usedSize will hold the actual number of bytes that are
 * used in that buffer. This value may be less than compSize.
 */
bool compress(uint8_t * rawData, const uint32_t rawSize, CompressPointer& compBuffer, uint32_t& compSize, uint32_t& usedSize, const int level=6);

} // namespace

#endif // LIBSTRIEZEL_COMPRESSIONFUNCTIONS_HPP
