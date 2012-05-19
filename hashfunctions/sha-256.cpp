/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
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

#include "sha-256.h"

namespace SHA256
{

void MessageDigest::setInitialValue()
{
  hash[0] = 0x6a09e667;
  hash[1] = 0xbb67ae85;
  hash[2] = 0x3c6ef372;
  hash[3] = 0xa54ff53a;
  hash[4] = 0x510e527f;
  hash[5] = 0x9b05688c;
  hash[6] = 0x1f83d9ab;
  hash[7] = 0x5be0cd19;
}

uint32_t rotr(const uint8_t n, const uint32_t w)
{
  //no values larger than 31 allowed here, because 32 is the word size
  if (n>31) throw 42;
  return ((w >> n) bitor (w << (32-n)));
}

uint32_t Ch(const uint32_t x, const uint32_t y, const uint32_t z)
{
  return ((x bitand y) xor (x bitand z));
}

uint32_t Maj(const uint32_t x, const uint32_t y, const uint32_t z)
{
  return ((x bitand y) xor (x bitand z) xor (y bitand z));
}

uint32_t SigmaZero(const uint32_t x)
{
  return (rotr(7, x) xor rotr(18, x) xor (x>>3));
}

uint32_t SigmaOne(const uint32_t x)
{
  return (rotr(17, x) xor rotr(19, x) xor (x>>10));
}

MessageDigest compute(const uint8_t* data, const uint64_t data_length_in_bits)
{
  //assume length to be a multiple of 8 bits, i.e. whole bytes only
  uint32_t msg_schedule[64];
  uint32_t a, b, c, d, e, f, g, h;

  MessageDigest H;
  H.setInitialValue();
  #warning TODO!
  const uint64_t NumberOfMessageBlocks = 1;
  MessageBlock msgBlock;
  uint64_t i;
  unsigned int t; //Laufvariable
  for (i=0; i<NumberOfMessageBlocks; ++i)
  {
    msgBlock = getMessageBlock(i);

    // 1. prepare message schedule
    for (t=0; t<16; ++t)
    {
      msg_schedule[t] = msgBlock.words[t];
    }//for t
    for (t=16; t<64; ++t)
    {
      msg_schedule[t] = SigmaOne(msg_schedule[t-2]) + msg_schedule[t-7] + SigmaZero(msg_schedule[t-15]) + msg_schedule[t-16];
    }//for run

    // 2. init. working vars
    a = H.hash[0];
    b = H.hash[1];
    c = H.hash[2];
    d = H.hash[3];
    e = H.hash[4];
    f = H.hash[5];
    g = H.hash[6];
    h = H.hash[7];

    // 3. ... TODO!
  }//for


}

} //namespace
