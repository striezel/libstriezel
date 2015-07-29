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

//#define SHA256_DEBUG

#include "sha256.hpp"
#include "functions.hpp"
#include <cstring>
#include <sys/types.h>
#include <fstream>
//#ifdef SHA256_DEBUG
  #include <iostream>
//#endif

namespace SHA256
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
  for (i=0; i<8; ++i)
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
  if (digestHexString.length()!=64) return false;
  unsigned int i, j;
  for (i=0; i<8; ++i)
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
      and (hash[4]==0) and (hash[5]==0) and (hash[6]==0) and (hash[7]==0));
}

void MessageDigest::setToNull()
{
  hash[0]= hash[1]= hash[2]= hash[3]= hash[4]= hash[5]= hash[6]= hash[7]= 0;
}

bool MessageDigest::operator==(const MessageDigest& other) const
{
  return ((hash[0]==other.hash[0]) and (hash[1]==other.hash[1])
    and (hash[2]==other.hash[2]) and (hash[3]==other.hash[3])
    and (hash[4]==other.hash[4]) and (hash[5]==other.hash[5])
    and (hash[6]==other.hash[6]) and (hash[7]==other.hash[7]));
}

bool MessageDigest::operator!=(const MessageDigest& other) const
{
  return ((hash[0]!=other.hash[0]) or (hash[1]!=other.hash[1])
    or (hash[2]!=other.hash[2]) or (hash[3]!=other.hash[3])
    or (hash[4]!=other.hash[4]) or (hash[5]!=other.hash[5])
    or (hash[6]!=other.hash[6]) or (hash[7]!=other.hash[7]));
}

bool MessageDigest::operator<(const MessageDigest& other) const
{
  unsigned int i;
  for (i=0; i<7; ++i)
  {
    if (hash[i]<other.hash[i]) return true;
    if (hash[i]>other.hash[i]) return false;
  }//for
  return (hash[7]<other.hash[7]);
}

MessageDigest computeFromBuffer(uint8_t* data, const uint64_t data_length_in_bits)
{
  BufferSource source(data, data_length_in_bits);
  return computeFromSource(source);
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

MessageDigest computeFromSource(MessageSource& source)
{
  MessageBlock msgBlock;

  uint32_t msg_schedule[64];
  uint32_t a, b, c, d, e, f, g, h;
  uint32_t temp1, temp2;
  MessageDigest H;

  H.setToNull();

  //setup stuff should have been done before this, so go on with hash initialization

  //set initial value
  H.hash[0] = 0x6a09e667;
  H.hash[1] = 0xbb67ae85;
  H.hash[2] = 0x3c6ef372;
  H.hash[3] = 0xa54ff53a;
  H.hash[4] = 0x510e527f;
  H.hash[5] = 0x9b05688c;
  H.hash[6] = 0x1f83d9ab;
  H.hash[7] = 0x5be0cd19;

  unsigned int t; //Laufvariable

  while (source.getNextMessageBlock(msgBlock))
  {
    // 1. prepare message schedule
    for (t=0; t<16; ++t)
    {
      msg_schedule[t] = msgBlock.words[t];
    }//for t
    #ifdef SHA256_DEBUG
    for (t=0; t<16; ++t)
    {
      std::dec(std::cout);
      std::cout << "W["<<t<<"] = ";
      std::hex(std::cout);
      std::cout <<msg_schedule[t]<<"\n";
    }//for
    #endif
    for (t=16; t<64; ++t)
    {
      msg_schedule[t] = sigmaOne(msg_schedule[t-2]) + msg_schedule[t-7] + sigmaZero(msg_schedule[t-15]) + msg_schedule[t-16];
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

    // 3. for loop
    for (t=0; t<64; ++t)
    {
      temp1 = h +CapitalSigmaOne(e) + SHA1_256::Ch(e, f, g) + sha256_k[t] + msg_schedule[t];
      temp2 = CapitalSigmaZero(a) + SHA1_256::Maj(a, b, c);
      h = g;
      g = f;
      f = e;
      e = d + temp1;
      d = c;
      c = b;
      b = a;
      a = temp1 + temp2;
      #ifdef SHA256_DEBUG
      std::dec(std::cout);
      std::cout << "t="<<t<<": a to h: ";
      std::hex(std::cout);
      std::cout <<a<<" "<<b<<" "<<c<<" "<<d<<" "<<e<<" "<<f<<" "<<g<<" "<<h<<"\n";
      #endif
    }//for t

    // 4. compute next intermediate hash value
    H.hash[0] = a + H.hash[0];
    H.hash[1] = b + H.hash[1];
    H.hash[2] = c + H.hash[2];
    H.hash[3] = d + H.hash[3];
    H.hash[4] = e + H.hash[4];
    H.hash[5] = f + H.hash[5];
    H.hash[6] = g + H.hash[6];
    H.hash[7] = h + H.hash[7];
  }//while message blocks are there

  return H;
}

} //namespace
