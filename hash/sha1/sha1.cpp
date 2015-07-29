/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's common code library.
    Copyright (C) 2012, 2014  Thoronador

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

#include "sha1.hpp"
#include "../sha-1-256_functions.hpp"
#include <iostream>
#include <stdexcept>
#include <cstring>
#include <sys/types.h>

namespace SHA1
{

// MessageDigest functions

MessageDigest::MessageDigest()
{
  setToNull();
}

std::string MessageDigest::toHexString() const
{
  const char cHexDigits[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  std::string result;
  unsigned int i;
  for (i=0; i<5; ++i)
  {
    result.push_back(cHexDigits[hash[i]>>28]);
    result.push_back(cHexDigits[(hash[i]>>24)%16]);
    result.push_back(cHexDigits[(hash[i]>>20)%16]);
    result.push_back(cHexDigits[(hash[i]>>16)%16]);
    result.push_back(cHexDigits[(hash[i]>>12)%16]);
    result.push_back(cHexDigits[(hash[i]>>8)%16]);
    result.push_back(cHexDigits[(hash[i]>>4)%16]);
    result.push_back(cHexDigits[hash[i]%16]);
  }//for
  return result;
}

bool MessageDigest::fromHexString(const std::string& digestHexString)
{
  if (digestHexString.length()!=40) return false;
  unsigned int i, j;
  for (i=0; i<5; ++i)
  {
    hash[i] = 0;
    for (j=0; j<8; ++j)
    {
      switch (digestHexString.at(i*8+j))
      {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
             hash[i] = hash[i] | ((digestHexString.at(i*8+j)-'0')<<(28-j*4));
             break;
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
             hash[i] = hash[i] | ((digestHexString.at(i*8+j)-'a'+10)<<(28-j*4));
             break;
        default:
             //invalid character encountered
             return false;
      }//swi
    }//for j
  }//for i
  return true;
}

bool MessageDigest::isNull() const
{
  return ((hash[0]==0) and (hash[1]==0) and (hash[2]==0) and (hash[3]==0)
      and (hash[4]==0));
}

void MessageDigest::setToNull()
{
  hash[0]= hash[1]= hash[2]= hash[3]= hash[4]= 0;
}

bool MessageDigest::operator==(const MessageDigest& other) const
{
  return ((hash[0]==other.hash[0]) and (hash[1]==other.hash[1])
    and (hash[2]==other.hash[2]) and (hash[3]==other.hash[3])
    and (hash[4]==other.hash[4]));
}

bool MessageDigest::operator!=(const MessageDigest& other) const
{
  return ((hash[0]!=other.hash[0]) or (hash[1]!=other.hash[1])
    or (hash[2]!=other.hash[2]) or (hash[3]!=other.hash[3])
    or (hash[4]!=other.hash[4]));
}

bool MessageDigest::operator<(const MessageDigest& other) const
{
  unsigned int i;
  for (i=0; i<4; ++i)
  {
    if (hash[i]<other.hash[i]) return true;
    if (hash[i]>other.hash[i]) return false;
  }//for
  return (hash[4]<other.hash[4]);
}

//SHA-1 constants
const uint32_t sha1_k[80] = {
  0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999,
  0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999,
  0x5a827999, 0x5a827999, 0x5a827999, 0x5a827999, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1,
  0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1,
  0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1, 0x6ed9eba1,
  0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc,
  0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc,
  0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0x8f1bbcdc, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6,
  0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6,
  0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6, 0xca62c1d6
};

uint32_t f_t(const uint8_t t, const uint32_t x, const uint32_t y, const uint32_t z)
{
  if (t<20) return SHA1_256::Ch(x,y,z);
  if (t<40) return SHA1_256::Parity(x,y,z);
  if (t<60) return SHA1_256::Maj(x,y,z);
  if (t<80) return SHA1_256::Parity(x,y,z);
  throw std::invalid_argument("f_t(): values of t have to be less than 80!");
}

MessageDigest computeFromSource(MessageSource& source)
{
  MessageBlock msgBlock;

  uint32_t msg_schedule[80];
  uint32_t a, b, c, d, e;
  uint32_t temp1;
  MessageDigest H;

  H.setToNull();

  //setup stuff should have been done before this, so go on with hash initialization

  //set initial value
  H.hash[0] = 0x67452301;
  H.hash[1] = 0xefcdab89;
  H.hash[2] = 0x98badcfe;
  H.hash[3] = 0x10325476;
  H.hash[4] = 0xc3d2e1f0;

  unsigned int t; //Laufvariable

  while (source.getNextMessageBlock(msgBlock))
  {
    // 1. prepare message schedule
    for (t=0; t<16; ++t)
    {
      msg_schedule[t] = msgBlock.words[t];
    }//for t
    for (t=16; t<80; ++t)
    {
      msg_schedule[t] = SHA1_256::rotl(1, msg_schedule[t-3] ^ msg_schedule[t-8] ^ msg_schedule[t-14] ^ msg_schedule[t-16]);
    }//for run

    // 2. init. working vars
    a = H.hash[0];
    b = H.hash[1];
    c = H.hash[2];
    d = H.hash[3];
    e = H.hash[4];

    // 3. for loop
    for (t=0; t<80; ++t)
    {
      temp1 = SHA1_256::rotl(5, a) + f_t(t, b, c, d) + e + sha1_k[t] + msg_schedule[t];
      e = d;
      d = c;
      c = SHA1_256::rotl(30, b);
      b = a;
      a = temp1;
    }//for t

    // 4. compute next intermediate hash value
    H.hash[0] = a + H.hash[0];
    H.hash[1] = b + H.hash[1];
    H.hash[2] = c + H.hash[2];
    H.hash[3] = d + H.hash[3];
    H.hash[4] = e + H.hash[4];
  }//while message blocks are there

  return H;
}

MessageDigest computeFromFile(const std::string& fileName)
{
  //setup file stuff
  FileSource source;
  if (!source.open(fileName))
  {
    std::cout << "Could not open file \""<<fileName<<"\" via file source!\n";
    MessageDigest H;
    return H;
  }
  return computeFromSource(source);
}

MessageDigest computeFromBuffer(uint8_t* data, const uint64_t data_length_in_bits)
{
  BufferSource source(data, data_length_in_bits);
  return computeFromSource(source);
}

} //namespace SHA1
