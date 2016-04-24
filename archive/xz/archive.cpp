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
#include <iostream>
#include <stdexcept>
#include <lzma.h>
#include "../../filesystem/file.hpp"

namespace libthoro
{

namespace xz
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
    throw std::runtime_error("libthoro::xz::archive: Failed to open file " + fileName + "!");
  }
  //fill entries
  fillEntries();
  postprocessEntry(fileName);
}

archive::~archive()
{
  int ret = archive_read_free(m_archive);
  if (ret != ARCHIVE_OK)
    throw std::runtime_error("libthoro::xz::archive: Could not close/free archive!");
  m_archive = nullptr;
}

void archive::postprocessEntry(const std::string& fileName)
{
  if (m_entries.size() == 1)
  {
    if (m_entries[0].size() == 0)
      m_entries[0].setSize(-1);
    if (m_entries[0].name() == "data")
    {
      libthoro::archive::entryLibarchive oneEntry(m_entries[0]);
      oneEntry.setName(fileName);
      const std::string n = oneEntry.basename();
      const std::string::size_type len = n.size();
      if ((len > 3)
          && ((n.substr(len-3, 3) == ".xz") || (n.substr(len-3, 3) == ".XZ")))
      {
        m_entries[0].setName(n.substr(0, len-3));
      }
    } //if name equals "data"
  } //if
}

void archive::applyFormats()
{
  int ret = archive_read_support_format_raw(m_archive);
  if (ret != ARCHIVE_OK)
  {
    archive_read_free(m_archive);
    m_archive = nullptr;
    throw std::runtime_error("libthoro::xz::archive::applyFormats(): Format not supported!");
  }
  ret = archive_read_support_compression_xz(m_archive);
  if (ret != ARCHIVE_OK)
  {
    archive_read_free(m_archive);
    m_archive = nullptr;
    throw std::runtime_error("libthoro::xz::archive::applyFormats(): Format xz not supported!");
  }
}

bool archive::isXz(const std::string& fileName)
{
  std::ifstream stream;
  stream.open(fileName, std::ios_base::binary | std::ios_base::in);
  if (!stream.good() || !stream.is_open())
    return false;

  char start[7];
  std::memset(start, '\0', 7);
  stream.read(start, 6);
  if (!stream.good() || stream.gcount() != 6)
    return false;
  stream.close();

  /* See http://tukaani.org/xz/xz-file-format-1.0.4.txt, section 2.1.1.1 for
     an explanation of the magic bytes in the file header.

     The magic literal for xz is FD 37 7A 58 5A 00 in hexadecimal,
     or 0xFD, '7', 'z', 'X', 'Z', 0x00 in ASCII.
  */
  return (std::string(start, 6) == std::string("\xFD\x37\x7A\x58\x5A\x00", 6));
}

bool archive::extractTo(const std::string& destFileName, const std::string& archiveFilePath)
{
  //If file does not exist in archive, it cannot be extracted.
  if (!contains(archiveFilePath))
    return false;
  //delegate the extraction work to extractDataTo()
  return extractDataTo(destFileName);
}

} //namespace

} //namespace
