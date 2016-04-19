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
#include <sys/stat.h>

namespace libthoro
{

namespace tar
{

entry::entry(struct archive_entry * ent)
: archive::entry()
{
  //name
  setName(archive_entry_pathname(ent));
  //status buffer
  const struct stat * statbuf = archive_entry_stat(ent);
  //size
  setSize(statbuf->st_size);
  // directory: AE_IFDIR
  setDirectory(archive_entry_filetype(ent) == AE_IFDIR);
  //m_time: time_t usually is just seconds since epoch
  setTime(statbuf->st_mtim.tv_sec);
}

} //namespace

} //namespace
