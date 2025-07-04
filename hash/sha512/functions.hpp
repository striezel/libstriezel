/*
 -----------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2015  Dirk Stolle

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

#ifndef LIBSTRIEZEL_SHA512_FUNCTIONS_HPP
#define LIBSTRIEZEL_SHA512_FUNCTIONS_HPP

#include <cstdint>
#include <stdexcept>

/* This file contains the constants and (shorter) functions for SHA-512
   calculation. */

namespace SHA512
{

//SHA-512 constants
const uint64_t sha512_k[80] = {
  0x428a2f98d728ae22, 0x7137449123ef65cd, 0xb5c0fbcfec4d3b2f, 0xe9b5dba58189dbbc,
  0x3956c25bf348b538, 0x59f111f1b605d019, 0x923f82a4af194f9b, 0xab1c5ed5da6d8118,
  0xd807aa98a3030242, 0x12835b0145706fbe, 0x243185be4ee4b28c, 0x550c7dc3d5ffb4e2,
  0x72be5d74f27b896f, 0x80deb1fe3b1696b1, 0x9bdc06a725c71235, 0xc19bf174cf692694,
  0xe49b69c19ef14ad2, 0xefbe4786384f25e3, 0x0fc19dc68b8cd5b5, 0x240ca1cc77ac9c65,
  0x2de92c6f592b0275, 0x4a7484aa6ea6e483, 0x5cb0a9dcbd41fbd4, 0x76f988da831153b5,
  0x983e5152ee66dfab, 0xa831c66d2db43210, 0xb00327c898fb213f, 0xbf597fc7beef0ee4,
  0xc6e00bf33da88fc2, 0xd5a79147930aa725, 0x06ca6351e003826f, 0x142929670a0e6e70,
  0x27b70a8546d22ffc, 0x2e1b21385c26c926, 0x4d2c6dfc5ac42aed, 0x53380d139d95b3df,
  0x650a73548baf63de, 0x766a0abb3c77b2a8, 0x81c2c92e47edaee6, 0x92722c851482353b,
  0xa2bfe8a14cf10364, 0xa81a664bbc423001, 0xc24b8b70d0f89791, 0xc76c51a30654be30,
  0xd192e819d6ef5218, 0xd69906245565a910, 0xf40e35855771202a, 0x106aa07032bbd1b8,
  0x19a4c116b8d2d0c8, 0x1e376c085141ab53, 0x2748774cdf8eeb99, 0x34b0bcb5e19b48a8,
  0x391c0cb3c5c95a63, 0x4ed8aa4ae3418acb, 0x5b9cca4f7763e373, 0x682e6ff3d6b2b8a3,
  0x748f82ee5defb2fc, 0x78a5636f43172f60, 0x84c87814a1f0ab72, 0x8cc702081a6439ec,
  0x90befffa23631e28, 0xa4506cebde82bde9, 0xbef9a3f7b2c67915, 0xc67178f2e372532b,
  0xca273eceea26619c, 0xd186b8c721c0c207, 0xeada7dd6cde0eb1e, 0xf57d4f7fee6ed178,
  0x06f067aa72176fba, 0x0a637dc5a2c898a6, 0x113f9804bef90dae, 0x1b710b35131c471b,
  0x28db77f523047d84, 0x32caab7b40c72493, 0x3c9ebe0a15c9bebc, 0x431d67c49c100d4c,
  0x4cc5d4becb3e42b6, 0x597f299cfc657e2a, 0x5fcb6fab3ad6faec, 0x6c44198c4a475817
};

/* functions that are used in SHA-384 and/or SHA-512 hash calculations */

inline uint64_t rotr(const uint8_t n, const uint64_t w)
{
  #ifdef SHA512_DEBUG
  //no values larger than 63 allowed here, because 64 is the word size
  if (n>63)
    throw std::invalid_argument("rotr(): no values of n which are larger than 63 are allowed here, because 64 is the word size!");
  #endif
  return ((w >> n) | (w << (64-n)));
}

inline uint64_t rotl(const uint8_t n, const uint64_t w)
{
  #ifdef SHA512_DEBUG
  //no values larger than 63 allowed here, because 64 is the word size
  if (n>63)
    throw std::invalid_argument("rotl(): no values of n which are larger than 63 are allowed here, because 64 is the word size!");
  #endif
  return ((w << n) | (w >> (64-n)));
}

inline uint64_t Ch(const uint64_t x, const uint64_t y, const uint64_t z)
{
  return ((x & y) ^ ((~x) & z));
}

inline uint64_t Maj(const uint64_t x, const uint64_t y, const uint64_t z)
{
  return ((x & y) ^ (x & z) ^ (y & z));
}

/*
inline uint64_t Parity(const uint64_t x, const uint64_t y, const uint64_t z)
{
  return (x ^ y ^ z);
}
*/

inline uint64_t CapitalSigmaZero(const uint64_t x)
{
  return (rotr(28, x) ^ rotr(34, x) ^ rotr(39, x));
}

inline uint64_t CapitalSigmaOne(const uint64_t x)
{
  return (rotr(14, x) ^ rotr(18, x) ^ rotr(41, x));
}

inline uint64_t sigmaZero(const uint64_t x)
{
  return (rotr(1, x) ^ rotr(8, x) ^ (x>>7));
}

inline uint64_t sigmaOne(const uint64_t x)
{
  return (rotr(19, x) ^ rotr(61, x) ^ (x>>6));
}

}//namespace SHA512

#endif // LIBSTRIEZEL_SHA512_FUNCTIONS_HPP
