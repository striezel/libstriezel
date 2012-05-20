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

    /* returns the message digest's representation as hexadecimal string */
    std::string toHexString() const;

    /* equality operator */
    bool operator==(const MessageDigest& other) const;
  };

  MessageDigest compute(const uint8_t* data, const uint64_t data_length_in_bits);
}//SHA256 namespace

#endif // RANDOM_THORO_SHA_256_H
