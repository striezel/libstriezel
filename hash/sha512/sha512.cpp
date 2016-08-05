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

#include "sha512.hpp"
#include "functions.hpp"
#ifdef SHA512_DEBUG
#include <iostream>
#endif // SHA512_DEBUG

namespace SHA512
{

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
    result.push_back(cHexDigits[(hash[i]>>60)%16]);
    result.push_back(cHexDigits[(hash[i]>>56)%16]);
    result.push_back(cHexDigits[(hash[i]>>52)%16]);
    result.push_back(cHexDigits[(hash[i]>>48)%16]);
    result.push_back(cHexDigits[(hash[i]>>44)%16]);
    result.push_back(cHexDigits[(hash[i]>>40)%16]);
    result.push_back(cHexDigits[(hash[i]>>36)%16]);
    result.push_back(cHexDigits[(hash[i]>>32)%16]);
    result.push_back(cHexDigits[(hash[i]>>28)%16]);
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
  if (digestHexString.length()!=128) return false;
  unsigned int i, j;
  for (i=0; i<8; ++i)
  {
    hash[i] = 0;
    for (j=0; j<16; ++j)
    {
      switch (digestHexString.at(i*16+j))
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
             hash[i] = hash[i] | (static_cast<uint64_t>(digestHexString.at(i*16+j)-'0')<<(60-j*4));
             break;
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'e':
        case 'f':
             hash[i] = hash[i] | (static_cast<uint64_t>(digestHexString.at(i*16+j)-'a'+10)<<(60-j*4));
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
  hash[0] = hash[1] = hash[2] = hash[3] = hash[4] = hash[5] = hash[6] = hash[7] = 0;
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


MessageDigest computeFromSource(MessageSource& source)
{
  MessageBlock msgBlock;

  uint64_t msg_schedule[80];
  uint64_t a, b, c, d, e, f, g, h;
  uint64_t temp1, temp2;
  MessageDigest H;

  //setup stuff should have been done before this, so go on with hash initialization

  //set initial value
  H.hash[0] = 0x6a09e667f3bcc908;
  H.hash[1] = 0xbb67ae8584caa73b;
  H.hash[2] = 0x3c6ef372fe94f82b;
  H.hash[3] = 0xa54ff53a5f1d36f1;
  H.hash[4] = 0x510e527fade682d1;
  H.hash[5] = 0x9b05688c2b3e6c1f;
  H.hash[6] = 0x1f83d9abfb41bd6b;
  H.hash[7] = 0x5be0cd19137e2179;

  unsigned int t; //loop variable

  while (source.getNextMessageBlock(msgBlock))
  {
    // 1. prepare message schedule
    for (t=0; t<16; ++t)
    {
      msg_schedule[t] = msgBlock.words[t];
    }//for t
    #ifdef SHA512_DEBUG
    for (t=0; t<16; ++t)
    {
      std::dec(std::cout);
      std::cout << "W["<<t<<"] = ";
      std::hex(std::cout);
      std::cout <<msg_schedule[t]<<"\n";
    }//for
    #endif
    for (t=16; t<80; ++t)
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
    for (t=0; t<80; ++t)
    {
      temp1 = h + CapitalSigmaOne(e) + SHA512::Ch(e, f, g) + sha512_k[t] + msg_schedule[t];
      temp2 = CapitalSigmaZero(a) + SHA512::Maj(a, b, c);
      h = g;
      g = f;
      f = e;
      e = d + temp1;
      d = c;
      c = b;
      b = a;
      a = temp1 + temp2;
      #ifdef SHA512_DEBUG
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
