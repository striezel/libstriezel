/*
 -----------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2012, 2014, 2015  Dirk Stolle

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

#include "MessageSource.hpp"

namespace SHA512
{

#if BYTE_ORDER == LITTLE_ENDIAN
// byte order adjustment functions (Shouldn't there be a std header for that?)
/* reverses little endian to big endian

   parameters:
       w - little endian value
       x - var. to store the converted value
*/
void reverse64(const uint64_t w, uint64_t& x)
{
  uint64_t tmp = w;
  tmp = (tmp >> 32) | (tmp << 32);
  tmp = ((tmp & 0xff00ff00ff00ff00ULL) >> 8) |
        ((tmp & 0x00ff00ff00ff00ffULL) << 8);
  x = ((tmp & 0xffff0000ffff0000ULL) >> 16) |
      ((tmp & 0x0000ffff0000ffffULL) << 16);
}
#endif

void MessageBlock::reverseBlock()
{
  #if BYTE_ORDER == LITTLE_ENDIAN
  unsigned int i;
  for (i=0; i<16; ++i)
  {
    reverse64(words[i], words[i]);
  }
  #endif
}

/* MessageSource functions */
MessageSource::MessageSource()
: m_BitsRead(0),
  m_PaddingBuffer((uint8_t*) 0),
  m_Status(psUnpadded)
{
}

MessageSource::~MessageSource()
{
  if (m_PaddingBuffer!=(uint8_t*) 0)
  {
    delete[] m_PaddingBuffer;
    m_PaddingBuffer = 0;
  }
}

} //namespace
