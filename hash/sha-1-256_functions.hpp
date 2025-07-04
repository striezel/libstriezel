/*
 -----------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2012, 2014  Dirk Stolle

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

#ifndef LIBSTRIEZEL_SHA_1_256_COMMON_HPP
#define LIBSTRIEZEL_SHA_1_256_COMMON_HPP

#include <cstdint>
#include <stdexcept>

/* This file contains functions that are used in SHA-1 and/or SHA-256 hash
   calculations. */

namespace SHA1_256
{

inline uint32_t rotr(const uint8_t n, const uint32_t w)
{
  #ifdef SHA256_DEBUG
  // no values larger than 31 allowed here, because 32 is the word size
  if (n > 31)
    throw std::invalid_argument("rotr(): no values of n which are larger than 31 are allowed here, because 32 is the word size!");
  #endif
  return ((w >> n) | (w << (32 - n)));
}

inline uint32_t rotl(const uint8_t n, const uint32_t w)
{
  #ifdef SHA256_DEBUG
  // no values larger than 31 allowed here, because 32 is the word size
  if (n > 31)
    throw std::invalid_argument("rotl(): no values of n which are larger than 31 are allowed here, because 32 is the word size!");
  #endif
  return ((w << n) | (w >> (32 - n)));
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

} // namespace SHA1_256

#endif // LIBSTRIEZEL_SHA_1_256_COMMON_HPP
