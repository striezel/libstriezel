/*
 -----------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2012 Dirk Stolle

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

#ifndef LIBSTRIEZEL_SHA256_FUNCTIONS_HPP
#define LIBSTRIEZEL_SHA256_FUNCTIONS_HPP

#include <cstdint>
#include "../sha-1-256_functions.hpp"

/* This file contains the constants and (shorter) functions for SHA-256
   calculation. */

namespace SHA256
{

// SHA-256 constants
const uint32_t sha256_k[64] = {
  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

inline uint32_t CapitalSigmaZero(const uint32_t x)
{
  return (SHA1_256::rotr(2, x) ^ SHA1_256::rotr(13, x) ^ SHA1_256::rotr(22, x));
}

inline uint32_t CapitalSigmaOne(const uint32_t x)
{
  return (SHA1_256::rotr(6, x) ^ SHA1_256::rotr(11, x) ^ SHA1_256::rotr(25, x));
}

inline uint32_t sigmaZero(const uint32_t x)
{
  return (SHA1_256::rotr(7, x) ^ SHA1_256::rotr(18, x) ^ (x>>3));
}

inline uint32_t sigmaOne(const uint32_t x)
{
  return (SHA1_256::rotr(17, x) ^ SHA1_256::rotr(19, x) ^ (x>>10));
}

} // namespace SHA256

#endif // LIBSTRIEZEL_SHA256_FUNCTIONS_HPP
