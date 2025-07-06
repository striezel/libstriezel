/*
 -----------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2016, 2021, 2025  Dirk Stolle

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

#include "entryLibarchive.hpp"
#include <sys/stat.h>

namespace libstriezel::archive
{

entryLibarchive::entryLibarchive(struct archive_entry * ent)
: archive::entry()
{
  // name
  setName(archive_entry_pathname(ent));
  // status buffer
  const struct stat * statbuf = archive_entry_stat(ent);
  // size
  setSize(archive_entry_size(ent));
  // check some file types
  const auto type = archive_entry_filetype(ent);
  // directory: AE_IFDIR
  setDirectory(type == AE_IFDIR);
  // symlink: AE_IFLINK
  setSymLink(type == AE_IFLNK);
  // m_time: time_t usually is just seconds since epoch
  #if defined(_WIN32)
  setTime(statbuf->st_mtime);
  #else
  setTime(statbuf->st_mtim.tv_sec);
  #endif
}

} // namespace
