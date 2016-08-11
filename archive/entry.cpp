/*
 -----------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2016  Dirk Stolle

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

namespace libstriezel
{

namespace archive
{

entry::entry()
: m_name(""),
  m_size(-1),
  m_directory(false),
  m_link(false),
  m_m_time(static_cast<std::time_t>(-1))
{
}

entry::~entry()
{
  //empty
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

bool entry::isSymLink() const
{
  return m_link;
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

void entry::setName(const std::string& name)
{
  m_name = name;
}

void entry::setSize(const int64_t sizeInBytes)
{
  if (sizeInBytes >= 0)
    m_size = sizeInBytes;
  else
    m_size = -1; // -1 indicates unknown size
}

void entry::setTime(const std::time_t modTime)
{
  m_m_time = modTime;
}

void entry::setDirectory(const bool isDir)
{
  m_directory = isDir;
}

void entry::setSymLink(const bool isSymbolic)
{
  m_link = isSymbolic;
}

} //namespace

} //namespace
