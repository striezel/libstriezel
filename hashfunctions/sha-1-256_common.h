/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's common code library.
    Copyright (C) 2012 thoronador

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

#ifndef LIBTHORO_SHA_1_256_COMMON_H
#define LIBTHORO_SHA_1_256_COMMON_H

namespace SHA1_256
{

inline uint32_t rotr(const uint8_t n, const uint32_t w)
{
  //no values larger than 31 allowed here, because 32 is the word size
  if (n>31) throw 42;
  return ((w >> n) | (w << (32-n)));
}

inline uint32_t Ch(const uint32_t x, const uint32_t y, const uint32_t z)
{
  return ((x & y) ^ ((~x) & z));
}

inline uint32_t Maj(const uint32_t x, const uint32_t y, const uint32_t z)
{
  return ((x & y) ^ (x & z) ^ (y & z));
}

inline uint32_t Parity(const uint32_t x, const uint32_t y, const uint32_t z)
{
  return (x ^ y ^ z);
}

} //namespace SHA1_256

#endif // LIBTHORO_SHA_1_256_COMMON_H
