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

#ifndef LIBSTRIEZEL_SHA512_BUFFERSOURCEUTILITY_HPP
#define LIBSTRIEZEL_SHA512_BUFFERSOURCEUTILITY_HPP

#include "sha512.hpp"

namespace SHA512
{
  /** \brief computes and returns the message digest of data in the given buffer of the given length
   *
   * \param data   pointer to the message data buffer
   * \param data_length_in_bits   length of data in bits. Value is rounded up to
   *                              the next integral multiple of eight, i.e. only
   *                              full bytes are allowed.
   * \return Returns the SHA512 message digest on success.
   *         Returns the "null" message digest (i.e. all bits set to zero),
   *         if an error occurred.
   */
  MessageDigest computeFromBuffer(uint8_t* data, const uint64_t data_length_in_bits);
} //namespace

#endif // LIBSTRIEZEL_SHA512_BUFFERSOURCEUTILITY_HPP
