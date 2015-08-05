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

#ifndef LIBTHORO_SHA384_FILESOURCEUTILITY_HPP
#define LIBTHORO_SHA384_FILESOURCEUTILITY_HPP

#include "sha384.hpp"

namespace SHA384
{

  /** \brief computes and returns the message digest of the given file's contents
   *
   * \param fileName    name of the file
   * \return Returns the SHA384 message digest on success.
   *         Returns the "null" message digest (i.e. all bits set to zero),
   *         if an error occurred.
   */
  MessageDigest computeFromFile(const std::string& fileName);

} //namespace

#endif // LIBTHORO_SHA384_FILESOURCEUTILITY_HPP
