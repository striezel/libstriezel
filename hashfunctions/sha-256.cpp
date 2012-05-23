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
#include <cstring>
#include <sys/types.h>
#include <fstream>
#ifdef SHA256_DEBUG
  #include <iostream>
#endif

namespace SHA256
{

#if BYTE_ORDER == LITTLE_ENDIAN
// byte order adjustment functions (Shouldn't there be a std header for that?)

/* reverses little endian to big endian

   parameters:
       w - little endian value
       x - var. to store the converted value
*/
void reverse32(const uint32_t w, uint32_t& x)
{
  uint32_t tmp = w;
  tmp = (tmp >> 16) | (tmp << 16);
  x = ((tmp & 0xff00ff00UL) >> 8) | ((tmp & 0x00ff00ffUL) << 8);
}

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


struct MessageBlock
{
  uint32_t words[16];
};//struct

//SHA-256 constants
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

uint32_t rotr(const uint8_t n, const uint32_t w)
{
  //no values larger than 31 allowed here, because 32 is the word size
  if (n>31) throw 42;
  return ((w >> n) | (w << (32-n)));
}

uint32_t Ch(const uint32_t x, const uint32_t y, const uint32_t z)
{
  return ((x & y) ^ ((~x) & z));
}

uint32_t Maj(const uint32_t x, const uint32_t y, const uint32_t z)
{
  return ((x & y) ^ (x & z) ^ (y & z));
}

uint32_t CapitalSigmaZero(const uint32_t x)
{
  return (rotr(2, x) ^ rotr(13, x) ^ rotr(22, x));
}

uint32_t CapitalSigmaOne(const uint32_t x)
{
  return (rotr(6, x) ^ rotr(11, x) ^ rotr(25, x));
}

uint32_t sigmaZero(const uint32_t x)
{
  return (rotr(7, x) ^ rotr(18, x) ^ (x>>3));
}

uint32_t sigmaOne(const uint32_t x)
{
  return (rotr(17, x) ^ rotr(19, x) ^ (x>>10));
}

void getMessageBlock(MessageBlock& mBlock, const uint64_t n, const uint8_t* data, const uint64_t data_length_in_bits)
{
  if ((n+1)*512<=data_length_in_bits)
  {
    memcpy(&(mBlock.words[0]), &data[n*64], 64);
    #if BYTE_ORDER == LITTLE_ENDIAN
    unsigned int i;
    for (i=0; i<16; ++i)
    {
      reverse32(mBlock.words[i], mBlock.words[i]);
    }
    #endif
  }
  else
  {
    throw 42;
  }
}

/* pads a SHA-256 message as necessary and returns a buffer containing the
   padded message.

   parameters:
       data                - pointer to the message data buffer
       data_length_in_bits - length of message in bits (will be rounded up to
                             the next integral multiple of 8, if it's not an
                             integral multiple of 8 yet)
       padded_length       - variable that will be used to store the length of
                             the padded message in bits

   remarks:
       The application has to free the buffer the return value is pointing to,
       if it no longer needs that data. Otherwise the application will have a
       memory leak.
*/
uint8_t * padMessage(const uint8_t* data, uint64_t data_length_in_bits, uint64_t& padded_length)
{
  data_length_in_bits += ((data_length_in_bits%8)>0); //we want full bytes only
  padded_length = data_length_in_bits +8 /*usually just one bit, but since we have full bytes here only...*/
                 +64;
  uint64_t padded_blocks = padded_length / 512 + ((padded_length%512)>0);
  padded_length = padded_blocks*512;
  uint8_t * padded_data = new uint8_t[padded_length/8];
  //zero out last blocks of padded message (so we have all zero bits for padding (and a bit more, but we'll overwrite that later))
  if (padded_blocks>=2)
  {
    //zero out last but one block
    memset(&padded_data[(padded_length-1024)/8], 0, 1024/8);
  }
  else
  {
    memset(&padded_data[(padded_length-512)/8], 0, 512/8);
  }
  //copy stuff from message data buffer to padded buffer
  memcpy(padded_data, data, data_length_in_bits/8);
  //add 1-bit
  padded_data[data_length_in_bits/8] = 0x80;
  //add data length in bits
  #if BYTE_ORDER == LITTLE_ENDIAN
  reverse64(data_length_in_bits, data_length_in_bits);
  #endif
  memcpy(&padded_data[padded_length/8-8], &data_length_in_bits, 8);
  //get going
  return padded_data;
}

MessageDigest computeFromBuffer(const uint8_t* data, const uint64_t data_length_in_bits)
{
  uint64_t padded_len = 0;
  uint8_t * padded_data = padMessage(data, data_length_in_bits, padded_len);

  const uint64_t NumberOfMessageBlocks = padded_len/512;
  MessageBlock msgBlock;

  uint32_t msg_schedule[64];
  uint32_t a, b, c, d, e, f, g, h;
  uint32_t temp1, temp2;
  MessageDigest H;
  //set initial value
  H.hash[0] = 0x6a09e667;
  H.hash[1] = 0xbb67ae85;
  H.hash[2] = 0x3c6ef372;
  H.hash[3] = 0xa54ff53a;
  H.hash[4] = 0x510e527f;
  H.hash[5] = 0x9b05688c;
  H.hash[6] = 0x1f83d9ab;
  H.hash[7] = 0x5be0cd19;

  uint64_t i;
  unsigned int t; //Laufvariable

  for (i=0; i<NumberOfMessageBlocks; ++i)
  {
    getMessageBlock(msgBlock, i, padded_data, padded_len);
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
      temp1 = h +CapitalSigmaOne(e) + Ch(e, f, g) + sha256_k[t] + msg_schedule[t];
      temp2 = CapitalSigmaZero(a) + Maj(a, b, c);
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
  }//for i

  delete [] padded_data;
  padded_data = NULL;
  return H;
}

MessageDigest computeFromFile(const std::string& fileName)
{
  //This is still quite inefficent and memory consuming.
  MessageDigest temp;
  temp.setToNull();
  std::ifstream input;
  input.open(fileName.c_str(), std::ios::in | std::ios::binary);
  if (!input)
  {
    return temp;
  }
  input.seekg(0, std::ios_base::end);
  const std::ifstream::pos_type cFileSize = input.tellg();
  input.seekg(0, std::ios_base::beg);
  uint8_t * buffer = new uint8_t[cFileSize];
  input.read((char*) buffer, cFileSize);
  const bool well_done = input.good();
  input.close();
  if (!well_done)
  {
    delete[] buffer;
    buffer = NULL;
    return temp;
  }
  temp = computeFromBuffer(buffer, cFileSize*8);
  delete[] buffer;
  buffer = NULL;
  return temp;
}

} //namespace
