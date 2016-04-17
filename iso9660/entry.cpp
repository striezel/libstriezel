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

#include "entry.hpp"
#include <cstring>
#include <limits>
#include <cdio/iso9660.h>

namespace libthoro
{

namespace iso9660
{

entry::entry(const iso9660_stat_t * statbuf, const std::string& currentDirectory)
: archive::entry()
{
  //make sure buffer is big enough
  const std::size_t bufferSize = std::max(static_cast<std::size_t>(1024), std::strlen(statbuf->filename)+1);
  char nameBuffer[bufferSize];
  //fill buffer with NUL bytes
  std::memset(nameBuffer, '\0', bufferSize);
  iso9660_name_translate(statbuf->filename, nameBuffer);
  setName(currentDirectory + std::string(nameBuffer));
  //size
  setSize(statbuf->size);
  // _STAT_DIR == 2
  setDirectory(statbuf->type == iso9660_stat_s::_STAT_DIR);
  //m_time
  struct tm  temp = statbuf->tm;
  setTime(std::mktime(&temp));
}

} //namespace

} //namespace
