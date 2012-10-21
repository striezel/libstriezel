/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2012 Thoronador

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

#include "CompressionFunctions.h"
#include <iostream>
#include <zlib.h>

namespace Thoro
{

bool zlibDecompress(uint8_t * compressedData, const uint32_t compressedSize, uint8_t * decompBuffer, const uint32_t decompSize)
{
  if ((compressedData==NULL) or (compressedSize==0) or (decompBuffer==NULL) or (decompSize==0))
  {
    std::cout << "zlibDecompress: Error: invalid buffer values given!\n";
    return false;
  }

  z_stream streamZlib;

  /* allocate inflate state */
  streamZlib.zalloc = Z_NULL;
  streamZlib.zfree = Z_NULL;
  streamZlib.opaque = Z_NULL;
  streamZlib.avail_in = 0;
  streamZlib.next_in = Z_NULL;
  int z_return = inflateInit(&streamZlib);
  if (z_return != Z_OK)
  {
    switch (z_return)
    {
      case Z_MEM_ERROR:
           std::cout << "zlibDecompress: Error: not enough memory to initialize z_stream!\n";
           break;
      case Z_VERSION_ERROR:
           std::cout << "zlibDecompress: Error: incompatible library version!\n";
           break;
      case Z_STREAM_ERROR:
           std::cout << "zlibDecompress: Error: invalid parameters in z_stream!\n";
           break;
      default:
           std::cout << "zlibDecompress: Error: could not initialize z_stream!\n";
           break;
    }//swi
    return false;
  }//if error occured


  streamZlib.avail_in = compressedSize;
  streamZlib.next_in = compressedData;

  streamZlib.avail_out = decompSize;
  streamZlib.next_out = decompBuffer;

  /* decompress */
  z_return = inflate(&streamZlib, Z_NO_FLUSH);
  switch (z_return)
  {
    case Z_NEED_DICT:
         z_return = Z_DATA_ERROR;     /* and fall through */
    case Z_DATA_ERROR:
    case Z_STREAM_ERROR: //stream state
    case Z_MEM_ERROR:
         (void)inflateEnd(&streamZlib);
         std::cout << "zlibDecompress: Error while calling inflate()!\n";
         return false;
  }//swi
  int have = decompSize - streamZlib.avail_out;
  /* clean up zlib */
  (void)inflateEnd(&streamZlib);
  //check, if size matches expected number of bytes
  if (have!=decompSize)
  {
    std::cout << "zlibDecompress: Error: Having only "<<have<<" bytes in output"
              << "buffer, but expected size is "<<decompSize<<" bytes.\n";
    return false;
  }
  //return value Z_STREAM_END should be the right one, if all was successful
  if (z_return==Z_STREAM_END) return true;
  return false;
}

bool zlibCompress(uint8_t * rawData, const uint32_t rawSize, CompressPointer& compBuffer, uint32_t& compSize, uint32_t& usedSize, const int level)
{
  if ((rawData==NULL) or (rawSize==0) or (compBuffer==NULL) or (compSize==0))
  {
    usedSize = 0;
    std::cout << "zlibCompress: Error: invalid buffer values given!\n";
    return false;
  }

  z_stream streamZlib;

  /* allocate deflate state */
  streamZlib.zalloc = Z_NULL;
  streamZlib.zfree = Z_NULL;
  streamZlib.opaque = Z_NULL;
  streamZlib.avail_in = 0;
  streamZlib.next_in = Z_NULL;
  int z_return = deflateInit(&streamZlib, level);
  if (z_return != Z_OK)
  {
    switch (z_return)
    {
      case Z_MEM_ERROR:
           std::cout << "zlibCompress: Error: not enough memory to initialize z_stream!\n";
           break;
      case Z_VERSION_ERROR:
           std::cout << "zlibCompress: Error: incompatible library version!\n";
           break;
      case Z_STREAM_ERROR:
           std::cout << "zlibCompress: Error: "<<level<<" is not a valid compression level!\n";
           break;
      default:
           std::cout << "zlibCompress: Error: could not initialize z_stream!\n";
           break;
    }//swi
    return false;
  }//if error occured

  const uLong bound = deflateBound(&streamZlib, rawSize);
  if (compSize<bound)
  {
    //re-allocate buffer
    delete[] compBuffer;
    compBuffer = new uint8_t[bound];
    compSize = bound;
  }


  streamZlib.avail_in = rawSize;
  streamZlib.next_in = rawData;

  streamZlib.avail_out = compSize;
  streamZlib.next_out = compBuffer;

  /* compress */
  z_return = deflate(&streamZlib, Z_FINISH);
  switch (z_return)
  {
    case Z_OK: //not enough output buffer
         usedSize = 0;
         (void)deflateEnd(&streamZlib);
         std::cout << "zlibCompress: output buffer is too small for deflate(), available output buffer size is "
                   << streamZlib.avail_out<<" bytes!\n";
         return false;
    case Z_STREAM_END: //finished
         usedSize = compSize - streamZlib.avail_out;
         (void)deflateEnd(&streamZlib);
         return true;
    default:
         std::cout << "zlibCompress: unknown error (code="<<z_return<<")!\n";
         (void)deflateEnd(&streamZlib);
         return false;
  }//swi
}

} //namespace
