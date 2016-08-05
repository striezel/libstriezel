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

#include "archive.hpp"
#include <cerrno> //for errno
#include <cstring> //for memset()
#include <fstream> //for std::ofstream
#include <iostream> //for std::cerr
#include <memory> //for std::unique_ptr
#include <sstream> //for ostringstream to convert int to string
#include <stdexcept> //for standard exception classes
#include "../../filesystem/file.hpp"

namespace libstriezel
{

namespace zip
{

struct DeleterZipFile {
  void operator()(zip_file* fp) const
  {
    if (fp != nullptr)
      zip_fclose(fp);
  }
};


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

bool archive::extractTo(const std::string& destFileName, int64_t index) const
{
  const auto num = numEntries();
  if (((num >= 0) && (index >= num)) || (index < 0))
  {
    std::cerr << "zip::archive::extractTo: error: invalid index!" << std::endl;
    return false;
  }

  /* Check whether destination file already exists, we do not want to overwrite
     existing files. */
  if (libstriezel::filesystem::file::exists(destFileName))
  {
    std::cerr << "zip::archive::extractTo: error: destination file "
              << destFileName << " already exists!" << std::endl;
    return false;
  }

  //open/create destination file
  std::ofstream destination;
  destination.open(destFileName, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
  if (!destination.good() || !destination.is_open())
  {
    std::cerr << "zip::archive::extractTo: error: destination file "
              << destFileName << " could not be created/opened for writing!"
              << std::endl;
    return false;
  }

  //open file inside archive and wrap it in unique_ptr to make sure it gets closed
  std::unique_ptr<zip_file, DeleterZipFile> file(zip_fopen_index(m_archive, index, 0));
  if (nullptr == file)
  {
    std::cerr << "zip::archive::extractTo: error: " << getError() << std::endl;
    destination.close();
    filesystem::file::remove(destFileName);
    return false;
  }

  //one megabyte should be enough for incremental buffer
  const unsigned int bufferSize = 1024 * 1024;
  char buffer[bufferSize];
  zip_int64_t bytesRead = 1;

  while (bytesRead > 0)
  {
    bytesRead = zip_fread(file.get(), buffer, bufferSize);
    if (bytesRead < 0)
    {
      std::cerr << "zip::archive::extractTo: error while reading data from archive: "
                << getError() << std::endl;
      destination.close();
      filesystem::file::remove(destFileName);
      //close zip file - unique_ptr deleter handles zip_fclose()
      file = nullptr;
      return false;
    } //if
    destination.write(buffer, bytesRead);
    if (!destination.good())
    {
      std::cerr << "zip::archive::extractTo: error: Could not write data to file "
                << destFileName << "." << std::endl;
      destination.close();
      filesystem::file::remove(destFileName);
      //close zip file - unique_ptr deleter handles zip_fclose()
      file = nullptr;
      return false;
    } //if
  } //while

  //close zip file - unique_ptr deleter handles zip_fclose()
  file = nullptr;
  //close destination file
  destination.close();
  return true;
}

std::string archive::getError() const
{
  int zipErr = 0;
  int sysErr = 0;
  zip_error_get(m_archive, &zipErr, &sysErr);

  //2048 bytes should be more than enough
  const unsigned int bufferSize = 2048;
  char buf[bufferSize];
  std::memset(buf, '\0', bufferSize);
  int written = zip_error_to_str(buf, bufferSize, zipErr, sysErr);
  if (written < 0)
    return std::string("Error while getting the error string!")
          +std::string(" (A most peculiar thing, isn't it?)");
  return std::string(buf);
}

bool archive::isZip(const std::string& fileName)
{
  std::ifstream stream;
  stream.open(fileName, std::ios_base::binary | std::ios_base::in);
  if (!stream.good() || !stream.is_open())
    return false;

  uint32_t start = 0;
  stream.read(reinterpret_cast<char*>(&start), 4);
  if (!stream.good() || stream.gcount() != 4)
    return false;
  stream.close();

  /* magic literal for ZIP files is 50 4B 03 04.*/
  return (start == 0x04034B50);
}

} //namespace

} //namespace
