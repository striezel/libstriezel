/*
 -----------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2016, 2021  Dirk Stolle

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
#include <iostream>
#include <stdexcept>

namespace libstriezel
{

namespace ar
{

archive::archive(const std::string& fileName)
: archiveLibarchive(fileName)
{
  applyFormats();
  int ret = archive_read_open_filename(m_archive, fileName.c_str(), 4096);
  if (ret != ARCHIVE_OK)
  {
    archive_read_free(m_archive);
    m_archive = nullptr;
    throw std::runtime_error("libstriezel::ar::archive: Failed to open file " + fileName + "!");
  }
  // fill entries
  fillEntries();
}

archive::~archive()
{
  int ret = archive_read_free(m_archive);
  if (ret != ARCHIVE_OK)
    std::cerr << "libstriezel::ar::archive: Could not close/free archive!\n";
  m_archive = nullptr;
}

void archive::applyFormats()
{
  int ret = archive_read_support_format_ar(m_archive);
  if (ret != ARCHIVE_OK)
  {
    archive_read_free(m_archive);
    m_archive = nullptr;
    throw std::runtime_error("libstriezel::ar::archive::applyFormats(): Format not supported!");
  }
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
