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
#include <archive_entry.h>
#include "../../filesystem/file.hpp"

namespace libthoro
{

namespace tar
{

archive::archive(const std::string& fileName)
: libthoro::archive::archiveLibarchive(fileName)
{
  applyFormats();
  int ret = archive_read_open_filename(m_archive, fileName.c_str(), 4096);
  if (ret != ARCHIVE_OK)
  {
    archive_read_free(m_archive);
    m_archive = nullptr;
    throw std::runtime_error("libthoro::tar::archive: Failed to open file " + fileName + "!");
  }
  //fill entries
  fillEntries();
}

archive::~archive()
{
  int ret = archive_read_free(m_archive);
  if (ret != ARCHIVE_OK)
    throw std::runtime_error("libthoro::tar::archive: Could not close/free archive!");
  m_archive = nullptr;
}

void archive::applyFormats()
{
  int r2 = archive_read_support_format_tar(m_archive);
  if (r2 != ARCHIVE_OK)
  {
    archive_read_free(m_archive);
    m_archive = nullptr;
    throw std::runtime_error("libthoro::tar::archive::applyFormats(): Format not supported!");
  }
  r2 = archive_read_support_format_gnutar(m_archive);
  if (r2 != ARCHIVE_OK)
  {
    archive_read_free(m_archive);
    m_archive = nullptr;
    throw std::runtime_error("libthoro::tar::archive::applyFormats(): Format not supported!");
  }
}

bool archive::extractTo(const std::string& destFileName, const std::string& tarFilePath)
{
  //If file does not exist in archive, it cannot be extracted.
  if (!contains(tarFilePath))
    return false;

  /* Check whether destination file already exists, we do not want to overwrite
     existing files. */
  if (libthoro::filesystem::file::exists(destFileName))
  {
    std::cerr << "tar::archive::extractTo: error: destination file "
              << destFileName << " already exists!" << std::endl;
    return false;
  }

  struct archive_entry * ent;
  bool beenToEOF = false;
  unsigned int retryCount = 0;
  while (true)
  {
    int ret = archive_read_next_header(m_archive, &ent);
    if ((ret == ARCHIVE_OK) || (ret == ARCHIVE_WARN))
    {
      libthoro::archive::entryLibarchive e(ent);
      if (e.name() == tarFilePath)
      {
        //found the file, extract data
        //open/create destination file
        std::ofstream destination;
        destination.open(destFileName, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
        if (!destination.good() || !destination.is_open())
        {
          std::cerr << "tar::archive::extractTo: error: destination file "
                    << destFileName << " could not be created/opened for writing!"
                    << std::endl;
          return false;
        }

        int64_t totalBytesRead = 0;
        const unsigned int bufferSize = 4096;
        char buffer[bufferSize];
        while (totalBytesRead < e.size())
        {
          int bytesRead = 0;
          if (totalBytesRead + bufferSize <= e.size())
            bytesRead = archive_read_data(m_archive, buffer, bufferSize);
          else
            bytesRead = archive_read_data(m_archive, buffer, e.size() % bufferSize);
          if (bytesRead >= 0)
          {
            //write bytes to file
            destination.write(buffer, bytesRead);
            if (!destination.good())
            {
              std::cerr << "tar::archive::extractTo: error: Could not write data to file "
                        << destFileName << "." << std::endl;
              destination.close();
              filesystem::file::remove(destFileName);
              return false;
            } //if write failed
            totalBytesRead += bytesRead;
          }
          else
          {
            std::cerr << "tar::archive::extractTo: error while reading data from archive!"
                      << std::endl;
            destination.close();
            filesystem::file::remove(destFileName);
            return false;
          } //else (error)
        } //while
        //close destination file
        destination.close();
        return true;
      } //if
    } //if ARCHIVE_OK or ARCHIVE_WARN
    else if (ret == ARCHIVE_EOF)
    {
      if (beenToEOF)
      {
        //Already been here, quit.
        std::cerr << "tar::archive::extractTo: Could not find file " << tarFilePath
                  << " in archive!" << std::endl;
        return false;
      }
      //set EOF flag for later detection
      beenToEOF = true;
      //close and re-open archive to get to the first entry again
      reopen();
    } //if ARCHIVE_EOF
    else if (ret == ARCHIVE_RETRY)
    {
      //retry
      ++retryCount;
      if (retryCount >= 100)
      {
        std::cerr << "libthoro::tar::archive::extractTo(): Too many re-tries!" << std::endl;
        return false;
      }
    } //if retry
    else
    {
      //May be ARCHIVE_FATAL or similar
      std::cerr << "libthoro::tar::archive::extractTo(): Fatal or unknown error!" << std::endl;
      return false;
    } //else
  } //while
}

bool archive::isTar(const std::string& fileName)
{
  /* The magic literal for tar files it at offset 257.
     The literal can be "ustar\0" or "ustar\040\040\0", depending on the
     version / variant of the tar file.
  */
  std::ifstream stream;
  stream.open(fileName, std::ios_base::binary | std::ios_base::in);
  if (!stream.good() || !stream.is_open())
    return false;

  std::string sequence = "";
  try
  {
    stream.seekg(257);
    char buffer[9];
    std::memset(buffer, '\0', 9);
    stream.read(buffer, 8);
    if (!stream.good() || (stream.gcount() < 8))
    {
      stream.close();
      return false;
    }
    sequence = std::string(buffer, 8);
  }
  catch (...)
  {
    stream.close();
    return false;
  }
  stream.close();

  return ((sequence == std::string("ustar\x20\x20\0", 8))
      || (sequence.substr(0, 6) == std::string("ustar\0", 6)));
}

} //namespace

} //namespace
