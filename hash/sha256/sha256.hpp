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

#ifndef LIBTHORO_SHA_256_HPP
#define LIBTHORO_SHA_256_HPP

#include <stdint.h>
#include <string>
#include "MessageSource.hpp"

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

    /* inequality operator */
    bool operator!=(const MessageDigest& other) const;

    /* comparison operator */
    bool operator<(const MessageDigest& other) const;
  };


  /** \brief computes and returns the message digest of the given source
   *
   * \param source   the message source
   * \return Returns the SHA256 message digest on success.
   *         Returns the "null" message digest (i.e. all bits set to zero),
   *         if an error occurred.
   */
  MessageDigest computeFromSource(MessageSource& source);
}//SHA256 namespace

#endif // LIBTHORO_SHA_256_HPP
