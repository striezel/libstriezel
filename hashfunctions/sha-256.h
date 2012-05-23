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

#ifndef RANDOM_THORO_SHA_256_H
#define RANDOM_THORO_SHA_256_H

#include <stdint.h>
#include <string>

namespace SHA256
{
  struct MessageDigest
  {
    uint32_t hash[8];

    /* default constructor */
    MessageDigest();

    /* returns the message digest's representation as hexadecimal string */
    std::string toHexString() const;

    /* set the message digest according to the given hexadecimal string and
       returns true in case of success, or false if the string does not re-
       present a valid hexadecimal digest

       parameters:
           digestHexString - the string containing the message digest as hex
                             digits (must be all lower case)
    */
    bool fromHexString(const std::string& digestHexString);

    /* returns true, if all hash bits are set to zero */
    bool isNull() const;

    /* sets all bits of the hash to zero */
    void setToNull();

    /* equality operator */
    bool operator==(const MessageDigest& other) const;
  };

  /* computes and returns the message digest of data in the given buffer of the
     given length

     parameters:
         data                - pointer to the message data buffer
         data_length_in_bits - length of data in bits. Value is rounded up to
                               the next integral multiple of eight, i.e. only
                               full bytes are allowed.
  */
  MessageDigest computeFromBuffer(const uint8_t* data, const uint64_t data_length_in_bits);

  /* computes and returns the message digest of the given file's contents

     parameters:
         fileName - name of the file
  */
  MessageDigest computeFromFile(const std::string& fileName);
}//SHA256 namespace

#endif // RANDOM_THORO_SHA_256_H
