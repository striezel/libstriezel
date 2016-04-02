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
#include "../filesystem/directory.hpp"
#include "../filesystem/file.hpp"

namespace libthoro
{

namespace iso9660
{

archive::archive(const std::string& fileName)
: m_iso(nullptr)
{
  m_iso = iso9660_open_ext(fileName.c_str(),
          ISO_EXTENSION_JOLIET_LEVEL1 | ISO_EXTENSION_JOLIET_LEVEL2 |
          ISO_EXTENSION_JOLIET_LEVEL3 | ISO_EXTENSION_ROCK_RIDGE |
          ISO_EXTENSION_HIGH_SIERRA);
  if (nullptr == m_iso)
  {
    throw std::runtime_error(std::string("Error: Could not open ISO9660 image " + fileName + "!"));
  } //if open operation failed
}

archive::~archive()
{
  if (m_iso != nullptr)
  {
    iso9660_close(m_iso);
    m_iso = nullptr;
  }
}

std::vector<entry> archive::entries() const
{
  std::vector<entry> result;
  listFiles(result, "/");
  return result;
}

uint8_t archive::jolietLevel() const
{
  return iso9660_ifs_get_joliet_level(m_iso);
}

bool archive::extractTo(const std::string& destFileName, const std::string& isoPath) const
{
  if (isoPath.empty() || (isoPath == ".") || (isoPath == "..") || (isoPath == "/"))
  {
    std::cerr << "iso9660::archive::extractTo: error: invalid file path!" << std::endl;
    return false;
  }

  /* Check whether destination file already exists, we do not want to overwrite
     existing files. */
  if (libthoro::filesystem::file::exists(destFileName))
  {
    std::cerr << "iso9660::archive::extractTo: error: destination file "
              << destFileName << " already exists!" << std::endl;
    return false;
  }

  iso9660_stat_t * statbuf = iso9660_ifs_stat_translate(m_iso, isoPath.c_str());
  if (statbuf == nullptr)
  {
    std::cerr << "iso9660::archive::extractTo: Error while searching for file "
              << isoPath << " in archive!" << std::endl;
    return false;
  }
  //Directory entries cannot be extracted!
  if (statbuf->type == iso9660_stat_t::_STAT_DIR)
  {
    std::cerr << "iso9660::archive::extractTo: Error: " << isoPath
              << " is a directory!" << std::endl;
    std::free(statbuf);
    return false;
  }

  //open/create destination file
  std::ofstream destination;
  destination.open(destFileName, std::ios_base::out | std::ios_base::binary | std::ios_base::trunc);
  if (!destination.good() || !destination.is_open())
  {
    std::cerr << "iso9660::archive::extractTo: Error: Destination file "
              << destFileName << " could not be created/opened for writing!"
              << std::endl;
    std::free(statbuf);
    return false;
  }

  uint32_t i = 0;
  char buffer[ISO_BLOCKSIZE];
  for(i = 0; i < statbuf->size; i = i + ISO_BLOCKSIZE)
  {
    std::memset(buffer, '\0', ISO_BLOCKSIZE);
    if (ISO_BLOCKSIZE != iso9660_iso_seek_read(m_iso, buffer, statbuf->lsn + (i/ISO_BLOCKSIZE), 1))
    {
      std::cerr << "iso9660::archive::extractTo: Error: Could not read from "
                << isoPath << "!" << std::endl;
      destination.close();
      std::free(statbuf);
      return false;
    } //if
    /* Write from buffer to file stream:
       All ISO entries are divided into blocks, but the actual file size might
       not be an integral multiple of the block size. That's why we have to
       check here.
    */
    if (i+ISO_BLOCKSIZE <= statbuf->size)
      destination.write(buffer, ISO_BLOCKSIZE);
    else
    {
      destination.write(buffer, statbuf->size % ISO_BLOCKSIZE);
      //Sync output with device, because that was the last write operation.
      destination.flush();
    }

    if (!destination.good())
    {
      std::cerr << "iso9660::archive::extractTo: Error: Could not write to "
                << destFileName << "!" << std::endl;
      destination.close();
      std::free(statbuf);
      return false;
    }
  } //for
  //free stat buffer
  std::free(statbuf);
  statbuf = nullptr;
  //close stream
  destination.close();
  return true;
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

void archive::listFiles(std::vector<entry>& result, const std::string& directory) const
{
  CdioList_t* directoryList = iso9660_ifs_readdir(m_iso, directory.c_str());
  if (directoryList == nullptr)
    return;

  const unsigned int listLength = _cdio_list_length(directoryList);
  CdioListNode_t * node = _cdio_list_begin(directoryList);
  unsigned int processedNodes = 0;
  while ((node != nullptr) && (processedNodes < listLength))
  {
    iso9660_stat_t * p_statbuf = static_cast<iso9660_stat_t*> (_cdio_list_node_data(node));
    if (nullptr != p_statbuf)
    {
      entry e(p_statbuf, filesystem::slashify(directory));
      const std::string bname = e.basename();
      if ((bname != ".") &&   (bname != ".."))
      {
        result.push_back(e);
        if (e.isDirectory())
          listFiles(result, filesystem::slashify(directory) + bname);
      } //if not current or parent directory
    } //if statbuf is not null
    node = _cdio_list_node_next(node);
    ++processedNodes;
  } //while

  _cdio_list_free(directoryList, 1 /* true */);
  directoryList = nullptr;
}

} //namespace

} //namespace
