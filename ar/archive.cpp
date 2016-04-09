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

#include "archive.hpp"
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <archive_entry.h>

namespace libthoro
{

namespace ar
{

archive::archive(const std::string& fileName)
: m_archive(nullptr)
{
  m_archive = archive_read_new();
  if (nullptr == m_archive)
    throw std::runtime_error("libthoro::ar::archive: Could not allocate archive structure!");
  int ret = archive_read_support_format_ar(m_archive);
  if (ret != ARCHIVE_OK)
    throw std::runtime_error("libthoro::ar::archive: Format not supported!");
  ret = archive_read_open_filename(m_archive, fileName.c_str(), 512);
  if (ret != ARCHIVE_OK)
    throw std::runtime_error("libthoro::ar::archive: Failed to open file " + fileName + "!");
}

archive::~archive()
{
  int ret = archive_read_free(m_archive);
  if (ret != ARCHIVE_OK)
    throw std::runtime_error("libthoro::ar::archive: Could not close/free archive!");
  m_archive = nullptr;
}

std::vector<entry> archive::entries() const
{
  struct archive_entry * ent;

  std::vector<entry> result;
  while (archive_read_next_header(m_archive, &ent) == ARCHIVE_OK)
  {
    result.push_back(ent);
  } //while
  return result;
}

bool archive::extractTo(const std::string& destFileName, int64_t index) const
{
  throw std::runtime_error("ar::archive::extractTo(): Not implemented yet!");
}

bool archive::isAr(const std::string& fileName)
{
  std::ifstream stream;
  stream.open(fileName, std::ios_base::binary | std::ios_base::in);
  if (!stream.good() || !stream.is_open())
    return false;

  char start[7];
  std::memset(start, '\0', 7);
  stream.read(start, 7);
  if (!stream.good() || stream.gcount() != 7)
    return false;
  stream.close();

  /* magic literal for Ar files is "!<arch>".*/
  return (std::string(start, 7) == "!<arch>");
}

} //namespace

} //namespace
