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

#ifndef LIBSTRIEZEL_SHA224_HPP
#define LIBSTRIEZEL_SHA224_HPP

#include <stdint.h>
#include <string>
#include "../sha256/MessageSource.hpp"

namespace SHA224
{
  //alias for types that are shared with SHA-256
  typedef SHA256::MessageSource MessageSource;
  typedef SHA256::MessageBlock  MessageBlock;

  /// the MessageDigest structure for SHA224
  struct MessageDigest
  {
    uint32_t hash[7];

    /** \brief default constructor */
    MessageDigest();


    /** \brief alternative constructor
     *
     * \param h  array of eight (yes, eight; not seven) 32bit words.
     *           The first seven words will be used to initialize the hash.
     */
    MessageDigest(const uint32_t h[8]);


    /** \brief turns the current hash value into a string
     *
     * \return returns the message digest's representation as hexadecimal string
     */
    std::string toHexString() const;


    /** \brief set the message digest according to the given hexadecimal string
     *
     * \param digestHexString  the string containing the message digest as hex
     *                         digits (must be all lower case)
     * \return Returns true in case of success.
     *         Returns false if the string does not represent a valid
     *         hexadecimal digest.
     */
    bool fromHexString(const std::string& digestHexString);


    /** \brief checks whether the hash is the "null" hash
     *
     * \return Returns true, if all hash bits are set to zero.
     */
    bool isNull() const;


    /** \brief sets all bits of the hash to zero */
    void setToNull();


    /** \brief equality operator */
    bool operator==(const MessageDigest& other) const;


    /** \brief inequality operator */
    bool operator!=(const MessageDigest& other) const;


    /** \brief comparison operator */
    bool operator<(const MessageDigest& other) const;
  }; //struct MessageDigest


  /** \brief computes and returns the message digest of the given source
   *
   * \param source   the message source
   * \return Returns the SHA224 message digest on success.
   *         Returns the "null" message digest (i.e. all bits set to zero),
   *         if an error occurred.
   */
  MessageDigest computeFromSource(MessageSource& source);
} //namespace

#endif // LIBSTRIEZEL_SHA224_HPP
