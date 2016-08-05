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

#ifndef LIBSTRIEZEL_SHA1_FILESOURCEUTILITY_HPP
#define LIBSTRIEZEL_SHA1_FILESOURCEUTILITY_HPP

#include "sha1.hpp"

namespace SHA1
{

  /** \brief computes and returns the message digest of the given file's contents
   *
   * \param fileName    name of the file
   * \return Returns the SHA1 message digest on success.
   *         Returns the "null" message digest (i.e. all bits set to zero),
   *         if an error occurred.
   */
  MessageDigest computeFromFile(const std::string& fileName);

} //namespace

#endif // LIBSTRIEZEL_SHA1_FILESOURCEUTILITY_HPP
