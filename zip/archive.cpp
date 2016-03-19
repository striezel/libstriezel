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
#include <sstream> //for ostringstream to convert int to string
#include <stdexcept> //for standard exception classes
#include <cerrno> //for errno

namespace libthoro
{

namespace zip
{

archive::archive(const std::string& fileName)
: m_archive(nullptr)
{
  int errorCode = 0;
  m_archive = zip_open(fileName.c_str(), 0 /*ZIP_RDONLY*/, &errorCode);
  if (m_archive == nullptr)
  {
    //error handling: throw
    std::string msg;
    switch(errorCode)
    {
      case ZIP_ER_INCONS:
           msg = "Inconsistencies were found in the file " + fileName + ".";
           break;
      case ZIP_ER_INVAL:
           //Should not occur, because c_str() won't return NULL.
           msg = "Invalid path argument!";
           break;
      case ZIP_ER_MEMORY:
           msg = "Required memory could not be allocated.";
           break;
      case ZIP_ER_NOENT:
           msg = "The file " + fileName + " does not exist (and ZIP_CREATE is not set).";
           break;
      case ZIP_ER_NOZIP:
           msg = fileName + " is not a ZIP archive.";
           break;
      case ZIP_ER_OPEN:
           msg = "The file " + fileName +" could not be opened.";
           break;
      case ZIP_ER_READ:
           msg = "A read error occurred.";
           {
             std::ostringstream stream;
             stream << errno;
             msg += " errno is " + stream.str() + ".";
           }
           break;
      case ZIP_ER_SEEK:
           msg = "The file " + fileName + " is not seekable.";
           break;
      case ZIP_ER_EXISTS:
           msg = "The file " + fileName + " exists and ZIP_EXCL is set.";
           break;
      default:
           msg = "Unknown error occurred.";
           break;
    } //swi
    msg = "ZIP error: " + msg;
    throw std::runtime_error(msg);
  } //if
}

archive::~archive()
{
  if (m_archive != nullptr)
  {
    /* Close archive via zip_discard():
       We could also use zip_close() here, but zip_close() tries to write any
       changes that were made to the archive to the file. However, we are in
       read-only mode and should not make any changes to the ZIP file. To be on
       the safe side, we use zip_discard() instead of zip_close().

       Currently, the version packaged with Debian wheezy does not contain
       zip_discard() yet, so we use zip_unchange_*() to discard any accidental
       changes to the file.
    */
    //zip_discard(m_archive);
    zip_unchange_all(m_archive);
    zip_unchange_archive(m_archive);
    zip_close(m_archive);
    m_archive = nullptr;
  }
}

int64_t archive::numEntries() const
{
   return zip_get_num_entries(m_archive, 0);
}

std::vector<entry> archive::entries() const
{
    const auto num = numEntries();
    std::vector<entry> result;
    struct zip_stat sb;
    zip_stat_init(&sb);
    int64_t i = 0;
    for (i=0; i<num; ++i)
    {
      const int ret = zip_stat_index(m_archive, i, 0, &sb);
      if (0 == ret)
      {
        result.push_back(sb);
      }
      else
        return std::vector<entry>();
    } //for
    return result;
}

} //namespace

} //namespace
