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
#include <archive_entry.h>
#include "../../filesystem/file.hpp"

namespace libstriezel
{

namespace rar
{

archive::archive(const std::string& fileName)
: libstriezel::archive::archiveLibarchive(fileName)
{
  applyFormats();
  int ret = archive_read_open_filename(m_archive, fileName.c_str(), 4096);
  if (ret != ARCHIVE_OK)
  {
    archive_read_free(m_archive);
    m_archive = nullptr;
    throw std::runtime_error("libstriezel::rar::archive: Failed to open file " + fileName + "!");
  }
  //fill entries
  fillEntries();
}

archive::~archive()
{
  int ret = archive_read_free(m_archive);
  if (ret != ARCHIVE_OK)
    std::cerr << "libstriezel::rar::archive: Could not close/free archive!\n";
  m_archive = nullptr;
}

void archive::applyFormats()
{
  int r2 = archive_read_support_format_rar(m_archive);
  if (r2 != ARCHIVE_OK)
  {
    archive_read_free(m_archive);
    m_archive = nullptr;
    throw std::runtime_error("libstriezel::tar::archive::applyFormats(): Format not supported!");
  }
}

bool archive::isRar(const std::string& fileName)
{
  /* The magic literal for rar files is "Rar!\x1a\x07\x00" (newer versions) or
     "RE\x7e\x5e" (older versions). */
  std::ifstream stream;
  stream.open(fileName, std::ios_base::binary | std::ios_base::in);
  if (!stream.good() || !stream.is_open())
    return false;

  std::string sequence = "";
  try
  {
    char buffer[7];
    std::memset(buffer, '\0', 7);
    stream.read(buffer, 7);
    if (!stream.good() || (stream.gcount() < 7))
    {
      stream.close();
      return false;
    }
    sequence = std::string(buffer, 7);
  }
  catch (...)
  {
    stream.close();
    return false;
  }
  stream.close();

  return ((sequence == std::string("Rar!\x1a\x07\0", 7))
          || (sequence.substr(0, 4) == std::string("RE\x7e\x5e", 4)));
}

} //namespace

} //namespace
