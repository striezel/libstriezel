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

namespace ar
{

entry::entry(struct archive_entry * ent)
: m_name(""),
  m_size(-1),
  m_directory(false),
  m_m_time(static_cast<std::time_t>(-1))
{
  //name
  m_name = archive_entry_pathname(ent);
  //status buffer
  const struct stat * statbuf = archive_entry_stat(ent);
  //size
  m_size = statbuf->st_size;
  // directory: AE_IFDIR
  m_directory = (archive_entry_filetype(ent) == AE_IFDIR);
  //m_time: time_t usually is just seconds since epoch
  m_m_time = statbuf->st_mtim.tv_sec;
}

const std::string& entry::name() const
{
  return m_name;
}

int64_t entry::size() const
{
  return m_size;
}

std::time_t entry::m_time() const
{
  return m_m_time;
}

bool entry::isDirectory() const
{
  return m_directory;
}

std::string entry::basename() const
{
  if (m_name.empty())
    return "";

  std::string result(m_name);
  if (result[result.size()-1] == '/')
    result.erase(result.size()-1);
  const std::string::size_type pos = result.rfind('/');
  if (pos == std::string::npos)
    return result;
  return result.substr(pos +1);
}

} //namespace

} //namespace
