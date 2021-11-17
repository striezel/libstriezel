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

namespace libstriezel::archive::iso9660
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
    throw std::runtime_error("libstriezel::archive::iso9660::archive: Failed to open file " + fileName + "!");
  }
  // fill entries
  fillEntries();
}

archive::~archive()
{
  const int ret = archive_read_free(m_archive);
  if (ret != ARCHIVE_OK)
    std::cerr << "libstriezel::archive::iso9660::archive: Could not close/free archive!\n";
  m_archive = nullptr;
}

void archive::applyFormats()
{
  const int ret = archive_read_support_format_iso9660(m_archive);
  if (ret != ARCHIVE_OK)
  {
    archive_read_free(m_archive);
    m_archive = nullptr;
    throw std::runtime_error("libstriezel::archive::iso9660::archive::applyFormats(): Format not supported!");
  }
}

bool archive::isISO9660(const std::string& fileName)
{
  /* ISO9660 files don't use the first 32768 bytes (16 * 2048 bytes).
     The "Volume Descriptor Set" starts at offset 0x8000 (32768), and the first
     byte designates the type. After that, the sequence "CD001" (or in hexa-
     decimal code "43 44 30 30 31") is encountered. We check for the existence
     of this sequence.
  */

  std::ifstream stream;
  stream.open(fileName, std::ios_base::binary | std::ios_base::in);
  if (!stream.good() || !stream.is_open())
    return false;
  std::string sequence = "";
  try
  {
    stream.seekg(0x8001);
    char buffer[6];
    std::memset(buffer, '\0', 6);
    stream.read(buffer, 5);
    if (!stream.good() || (stream.gcount() != 5))
    {
      stream.close();
      return false;
    }
    sequence = std::string(buffer, 5);
  }
  catch (...)
  {
    stream.close();
    return false;
  }
  stream.close();

  return (std::string("CD001") == sequence);
}

} // namespace
