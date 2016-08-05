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
#include <cstring>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <zlib.h>
#include "../../filesystem/file.hpp"

namespace libstriezel
{

namespace gzip
{

archive::archive(const std::string& fileName)
: m_gzip(nullptr),
  m_entries(std::vector<entry>())
{
  std::ifstream infile;
  infile.open(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
  if (!infile.good())
    throw std::runtime_error("libstriezel::gzip::archive: Could not open file!");
  infile.seekg(0, std::ios_base::end);
  const std::ifstream::pos_type pos = infile.tellg();
  //file should be at least 10 bytes for header + 4 bytes for size
  if (pos <= 14)
    throw std::runtime_error("libstriezel::gzip::archive: File is too small to be a proper gzip file!");
  //seek size
  infile.seekg(-4, std::ios_base::end);
  uint32_t uncompressedSize = 0;
  infile.read(reinterpret_cast<char*>(&uncompressedSize), 4);
  if (!infile.good() || infile.gcount() != 4)
    throw std::runtime_error("libstriezel::gzip::archive: Could not read uncompressed file size!");
  //clean up
  infile.close();

  entry oneEntry;
  oneEntry.setSize(uncompressedSize);
  oneEntry.setName(fileName);
  std::string n = oneEntry.basename();
  const std::string::size_type len = n.size();
  //*.gz, e.g. "file.tar.gz", becomes "file.tar"
  if ((len > 3) && (n.substr(len-3, 3) == ".gz"))
  {
    oneEntry.setName(n.substr(0, len-3));
  }
  //*.tgz, e.g. "file.tgz", becomes "file.tar"
  else if ((len > 4) && (n.substr(len-4, 4) == ".tgz"))
    oneEntry.setName(n.substr(0, len-4) + ".tar");
  //*.svgz, e.g. "file.svgz", becomes "file.svg" (just cut the z off)
  else if ((len > 5) && (n.substr(len-5, 5) == ".svgz"))
    oneEntry.setName(n.substr(0, len-1));
  else
    //fall back to "data" as generic name
    oneEntry.setName("data");
  m_entries.push_back(oneEntry);

  //open file with zlib's gzip functions
  m_gzip = gzopen(fileName.c_str(), "rb");
  if (nullptr == m_gzip)
    throw std::runtime_error("libstriezel::gzip::archive: Could not open file with gzopen()!");
}

archive::~archive()
{
  if (gzclose(m_gzip) != Z_OK)
    throw std::runtime_error("libstriezel::gzip::archive: Could not close/free archive!");
  m_gzip = nullptr;
}

std::vector<entry> archive::entries() const
{
  return m_entries;
}

bool archive::extractTo(const std::string& destFileName)
{
  /* Check whether destination file already exists, we do not want to overwrite
     existing files. */
  if (libstriezel::filesystem::file::exists(destFileName))
  {
    std::cerr << "gzip::archive::extractTo: error: destination file "
              << destFileName << " already exists!" << std::endl;
    return false;
  }

  //rewind
  if (gzrewind(m_gzip) == -1)
  {
    //error while rewinding / seeking
    std::cerr << "gzip::archive::extractTo: error: Unable to rewind!" << std::endl;
    return false;
  }

  //open/create destination file
  std::ofstream destination;
  destination.open(destFileName, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
  if (!destination.good() || !destination.is_open())
  {
    std::cerr << "gzip::archive::extractTo: error: destination file "
              << destFileName << " could not be created/opened for writing!"
              << std::endl;
    return false;
  }

  const unsigned int bufferSize = 4096;
  char buffer[bufferSize];
  int bytesRead = 0;
  do
  {
    std::memset(buffer, '\0', bufferSize);
    bytesRead = gzread(m_gzip, buffer, bufferSize);
    if (bytesRead > 0)
    {
      //write bytes to file
      destination.write(buffer, bytesRead);
      if (!destination.good())
      {
        std::cerr << "gzip::archive::extractTo: error: Could not write data to file "
                  << destFileName << "." << std::endl;
        destination.close();
        filesystem::file::remove(destFileName);
        return false;
      } //if write failed
    } //if more than zero bytes have been read
  } while (bytesRead > 0);

  destination.close();
  //check last state
  if (bytesRead < 0)
  {
    std::cerr << "gzip::archive::extractTo: Error while reading compressed data!"
              << std::endl;
    filesystem::file::remove(destFileName);
    return false;
  }
  return true;
}

bool archive::isGzip(const std::string& fileName)
{
  std::ifstream stream;
  stream.open(fileName, std::ios_base::binary | std::ios_base::in);
  if (!stream.good() || !stream.is_open())
    return false;

  uint16_t start = 0;
  stream.read(reinterpret_cast<char*>(&start), 2);
  if (!stream.good() || stream.gcount() != 2)
    return false;
  stream.close();

  /* magic literal for gzip files is 1F 8B.*/
  return (start == 0x8B1F);
}

} //namespace

} //namespace
