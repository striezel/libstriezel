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

#ifndef LIBTHORO_ISO9660_ENTRY_HPP
#define LIBTHORO_ISO9660_ENTRY_HPP

#include <cstdint>
#include <ctime>
#include <string>
#include <cdio/iso9660.h>
#include "../archive/entry.hpp"

namespace libthoro
{

namespace iso9660
{

class entry: public archive::entry
{
  public:
    /** \brief constructor to create entry from ISO9660 stat buffer
     *
     * \param statbuf   pointer to ISO9660 stat buffer, may not be null
     * \param currentDirectory the current directory
     */
    entry(const iso9660_stat_t * statbuf, const std::string& currentDirectory);
}; //class

} //namespace

} //namespace

#endif // LIBTHORO_ISO9660_ENTRY_HPP
