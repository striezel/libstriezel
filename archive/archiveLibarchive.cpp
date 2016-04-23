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

#include "archiveLibarchive.hpp"
#include <iostream>
#include <stdexcept>

namespace libthoro
{

namespace archive
{

archiveLibarchive::archiveLibarchive()
: m_archive(nullptr),
  m_entries(std::vector<libthoro::archive::entryLibarchive>())
{
  //allocate new archive for reading
  m_archive = archive_read_new();
  if (nullptr == m_archive)
    throw std::runtime_error("libthoro::archive::archiveLibarchive: Could not allocate archive structure!");
}

archiveLibarchive::~archiveLibarchive()
{
  if (nullptr != m_archive)
  {
    int ret = archive_read_free(m_archive);
    if (ret != ARCHIVE_OK)
      throw std::runtime_error("libthoro::archive::archiveLibarchive: Could not close/free archive!");
    m_archive = nullptr;
  }
}

std::vector<libthoro::archive::entryLibarchive> archiveLibarchive::entries() const
{
  return m_entries;
}

bool archiveLibarchive::contains(const std::string& fileName) const
{
  auto it = m_entries.begin();
  while (it != m_entries.end())
  {
    if (it->name() == fileName)
      return true;
    ++it;
  } // while
  return false;
}

} //namespace

} //namespace
