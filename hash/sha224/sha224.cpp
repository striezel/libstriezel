/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's common code library.
    Copyright (C) 2015  Thoronador

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

#include "sha224.hpp"
#include "../sha256/functions.hpp"

namespace SHA224
{

MessageDigest::MessageDigest()
{
  setToNull();
}

MessageDigest::MessageDigest(const uint32_t h[8])
{
  hash[0] = h[0];
  hash[1] = h[1];
  hash[2] = h[2];
  hash[3] = h[3];
  hash[4] = h[4];
  hash[5] = h[5];
  hash[6] = h[6];
}

std::string MessageDigest::toHexString() const
{
  const char cHexDigits[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  std::string result;
  unsigned int i;
  for (i=0; i<7; ++i)
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
  if (digestHexString.length()!=56) return false;
  unsigned int i, j;
  for (i=0; i<7; ++i)
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
      and (hash[4]==0) and (hash[5]==0) and (hash[6]==0));
}

void MessageDigest::setToNull()
{
  hash[0] = hash[1] = hash[2] = hash[3] = hash[4] = hash[5] = hash[6] = 0;
}

bool MessageDigest::operator==(const MessageDigest& other) const
{
  return ((hash[0]==other.hash[0]) and (hash[1]==other.hash[1])
    and (hash[2]==other.hash[2]) and (hash[3]==other.hash[3])
    and (hash[4]==other.hash[4]) and (hash[5]==other.hash[5])
    and (hash[6]==other.hash[6]));
}

bool MessageDigest::operator!=(const MessageDigest& other) const
{
  return ((hash[0]!=other.hash[0]) or (hash[1]!=other.hash[1])
    or (hash[2]!=other.hash[2]) or (hash[3]!=other.hash[3])
    or (hash[4]!=other.hash[4]) or (hash[5]!=other.hash[5])
    or (hash[6]!=other.hash[6]));
}

bool MessageDigest::operator<(const MessageDigest& other) const
{
  unsigned int i;
  for (i=0; i<6; ++i)
  {
    if (hash[i]<other.hash[i]) return true;
    if (hash[i]>other.hash[i]) return false;
  }//for
  return (hash[6]<other.hash[6]);
}


MessageDigest computeFromSource(MessageSource& source)
{
  MessageBlock msgBlock;

  uint32_t msg_schedule[64];
  uint32_t a, b, c, d, e, f, g, h;
  uint32_t temp1, temp2;

  uint32_t hash[8];
  //setup stuff should have been done before this, so go on with hash initialization

  //set initial value
  hash[0] = 0xc1059ed8;
  hash[1] = 0x367cd507;
  hash[2] = 0x3070dd17;
  hash[3] = 0xf70e5939;
  hash[4] = 0xffc00b31;
  hash[5] = 0x68581511;
  hash[6] = 0x64f98fa7;
  hash[7] = 0xbefa4fa4;

  unsigned int t; //loop variable

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
      msg_schedule[t] = SHA256::sigmaOne(msg_schedule[t-2]) + msg_schedule[t-7] + SHA256::sigmaZero(msg_schedule[t-15]) + msg_schedule[t-16];
    }//for run

    // 2. init. working vars
    a = hash[0];
    b = hash[1];
    c = hash[2];
    d = hash[3];
    e = hash[4];
    f = hash[5];
    g = hash[6];
    h = hash[7];

    // 3. for loop
    for (t=0; t<64; ++t)
    {
      temp1 = h + SHA256::CapitalSigmaOne(e) + SHA1_256::Ch(e, f, g) + SHA256::sha256_k[t] + msg_schedule[t];
      temp2 = SHA256::CapitalSigmaZero(a) + SHA1_256::Maj(a, b, c);
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
    hash[0] = a + hash[0];
    hash[1] = b + hash[1];
    hash[2] = c + hash[2];
    hash[3] = d + hash[3];
    hash[4] = e + hash[4];
    hash[5] = f + hash[5];
    hash[6] = g + hash[6];
    hash[7] = h + hash[7];
  }//while message blocks are there

  return MessageDigest(hash);
}

} //namespace
