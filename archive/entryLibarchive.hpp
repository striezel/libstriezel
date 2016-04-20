/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's common code library.
    Copyright (C) 2016  Thoronador

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

#ifndef LIBTHORO_ARCHIVE_ENTRY_LIBARCHIVE_HPP
#define LIBTHORO_ARCHIVE_ENTRY_LIBARCHIVE_HPP

#include <cstdint>
#include <ctime>
#include <string>
#include <archive_entry.h>
#include "entry.hpp"

namespace libthoro
{

namespace archive
{

/** \brief class to represent an entry within an archive for libarchive
 */
class entryLibarchive: public entry
{
  public:
    /** \brief constructor to create entry from libarchive entry
     *
     * \param statbuf   pointer to libarchive entry, may not be null
     */
    entryLibarchive(struct archive_entry * ent);
}; //class

} //namespace

} //namespace

#endif // LIBTHORO_ARCHIVE_ENTRY_LIBARCHIVE_HPP
