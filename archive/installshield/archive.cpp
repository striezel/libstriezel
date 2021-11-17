/*
 -----------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2017, 2021  Dirk Stolle

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
#include <fstream> //for std::ifstream
#include <iostream>
#include <stdexcept>
#include "../../filesystem/file.hpp"

namespace libstriezel::installshield
{

archive::archive(const std::string& fileName)
: m_archive(nullptr)
{
  unshield_set_log_level(UNSHIELD_LOG_LEVEL_ERROR);
  m_archive = unshield_open(fileName.c_str());
  if (nullptr == m_archive)
  {
    throw std::runtime_error("Could not open InstallShield CAB!");
  }
}

archive::~archive()
{
  unshield_close(m_archive);
}

int64_t archive::numEntries() const
{
  return unshield_file_count(m_archive);
}

std::vector<libstriezel::archive::entry> archive::entries() const
{
  std::vector<libstriezel::archive::entry> result;
  const auto groupCount = unshield_file_group_count(m_archive);
  for(auto groupIdx = 0; groupIdx < groupCount; ++groupIdx)
  {
    UnshieldFileGroup* group = unshield_file_group_get(m_archive, groupIdx);
    for(auto fileIdx = group->first_file; fileIdx <= group->last_file; ++fileIdx)
    {
      if (unshield_file_is_valid(m_archive, fileIdx))
      {
        libstriezel::archive::entry e;
        const std::string directoryName(group->name);
        if (!directoryName.empty())
          e.setName(directoryName + "/" + unshield_file_name(m_archive, fileIdx));
        else
          e.setName(unshield_file_name(m_archive, fileIdx));
        e.setSize(unshield_file_size(m_archive, fileIdx));
        e.setDirectory(false);
        result.push_back(e);
      }
    } // for fileIdx
  } // for groupIdx
  return result;
}

bool archive::extractTo(const std::string& destFileName, int64_t index) const
{
  if (!unshield_file_is_valid(m_archive, index))
  {
    std::cerr << "Error: Index " << index << " does not point to a file!" << std::endl;
    return false;
  }
  // try old format first
  unshield_set_log_level(UNSHIELD_LOG_LEVEL_LOWEST);
  bool oldSave = unshield_file_save_old(m_archive, index, destFileName.c_str());
  unshield_set_log_level(UNSHIELD_LOG_LEVEL_ERROR);
  if (oldSave)
    return true;
  // try new format, if old format failed
  return unshield_file_save(m_archive, index, destFileName.c_str());
}

bool archive::extractTo(const std::string& destFileName, const std::string& archiveFilePath)
{
  /* Check whether destination file already exists, we do not want to overwrite
     existing files. */
  if (libstriezel::filesystem::file::exists(destFileName))
  {
    std::cerr << "archive::installshield::extractTo: error: destination file "
              << destFileName << " already exists!" << std::endl;
    return false;
  }
  // find matching file
  int64_t foundFileIdx = -1;
  const auto groupCount = unshield_file_group_count(m_archive);
  for(auto groupIdx = 0; groupIdx < groupCount; ++groupIdx)
  {
    UnshieldFileGroup* group = unshield_file_group_get(m_archive, groupIdx);
    for(auto fileIdx = group->first_file; fileIdx <= group->last_file; ++fileIdx)
    {
      if (unshield_file_is_valid(m_archive, fileIdx))
      {
        const std::string directoryName(group->name);
        if (!directoryName.empty())
        {
          if (directoryName + "/" + unshield_file_name(m_archive, fileIdx) == archiveFilePath)
          {
            foundFileIdx = fileIdx;
            break;
          }
        }
        else
        {
          if (unshield_file_name(m_archive, fileIdx) == archiveFilePath)
          {
            foundFileIdx = fileIdx;
            break;
          }
        } //else
      } //if valid file
    } //for fileIdx
    if (foundFileIdx >= 0)
      break;
  } //for groupIdx
  // Have we found anything?
  if (foundFileIdx == -1)
  {
    std::cerr << "archive::installshield::extractTo: error: file "
              << archiveFilePath << " does not exist in archive!" << std::endl;
    return false;
  }
  return extractTo(destFileName, foundFileIdx);
}


bool archive::isInstallShield(const std::string& fileName)
{
  std::ifstream stream;
  stream.open(fileName, std::ios_base::binary | std::ios_base::in);
  if (!stream.good() || !stream.is_open())
    return false;

  char start[4] = {'\0', '\0', '\0', '\0'};
  stream.read(start, 4);
  if (!stream.good() || stream.gcount() != 4)
    return false;
  stream.close();

  /* magic literal for InstallShield Cab is "ISc(".*/
  return (std::string(start, 4) == "ISc(");
}

} // namespace
