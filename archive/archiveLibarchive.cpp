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

archiveLibarchive::archiveLibarchive(const std::string& fileName)
: m_archive(nullptr),
  m_entries(std::vector<libthoro::archive::entryLibarchive>()),
  m_fileName(fileName)
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

void archiveLibarchive::fillEntries()
{
  m_entries.clear();
  struct archive_entry * ent;
  unsigned int retryCount = 0;
  bool finished = false;
  while (!finished)
  {
    const int ret = archive_read_next_header(m_archive, &ent);
    switch (ret)
    {
      case ARCHIVE_OK: //all OK
      case ARCHIVE_WARN: //success, but non-critical error occurred
           m_entries.push_back(ent);
           break;
      case ARCHIVE_EOF:
           //reached end of archive
           finished = true;
           break;
      case ARCHIVE_RETRY:
           //operation failed but can be retried, so let's do that
           ++retryCount;
           if (retryCount >= 100)
           {
             archive_read_free(m_archive);
             m_archive = nullptr;
             throw std::runtime_error("libthoro::archive::archiveLibarchive::fillEntries(): "
                     + std::string("Too many retries!"));
           }
           break;
      case ARCHIVE_FATAL:
           //fatal error
           archive_read_free(m_archive);
           m_archive = nullptr;
           throw std::runtime_error("libthoro::archive::archiveLibarchive::fillEntries(): "
                   + std::string("Fatal error while getting archive entries!"));
           break;
      default:
           //unknown error
           archive_read_free(m_archive);
           m_archive = nullptr;
           throw std::runtime_error("libthoro::archive::archiveLibarchive::fillEntries(): "
                   + std::string("Unknown error while getting archive entries!"));
           break;
    } //swi
  } //while
  //reopen file to start at beginning when getting next header
  reopen();
}

void archiveLibarchive::reopen()
{
  //close and re-open archive to get to the first entry again
  archive_read_free(m_archive);
  m_archive = nullptr;
  m_archive = archive_read_new();
  if (nullptr == m_archive)
    throw std::runtime_error("libthoro::archive::archiveLibarchive::reopen(): Could not allocate archive structure!");
  applyFormats();
  int r2 = archive_read_open_filename(m_archive, m_fileName.c_str(), 4096);
  if (r2 != ARCHIVE_OK)
  {
    archive_read_free(m_archive);
    m_archive = nullptr;
    throw std::runtime_error("libthoro::archive::archiveLibarchive::reopen(): Failed to re-open file " + m_fileName + "!");
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
