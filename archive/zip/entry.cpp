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

#include "entry.hpp"
#include <limits>

namespace libthoro
{

namespace zip
{

entry::entry(const struct zip_stat& zs)
: m_name(""),
  m_index(-1),
  m_sizeUncompressed(-1),
  m_sizeCompressed(-1),
  m_m_time(static_cast<std::time_t>(-1)),
  m_crc(0)
{
  if ((zs.valid & ZIP_STAT_NAME) != 0)
    m_name = std::string(zs.name);
  if ((zs.valid & ZIP_STAT_INDEX) != 0)
    m_index = zs.index;
  if ((zs.valid & ZIP_STAT_SIZE) != 0)
  {
    if (zs.size <= static_cast<zip_uint64_t>(std::numeric_limits<int64_t>::max()))
      m_sizeUncompressed = zs.size;
  }
  if ((zs.valid & ZIP_STAT_COMP_SIZE) != 0)
  {
    if (zs.size <= static_cast<zip_uint64_t>(std::numeric_limits<int64_t>::max()))
      m_sizeCompressed = zs.comp_size;
  }
  if ((zs.valid & ZIP_STAT_MTIME) != 0)
    m_m_time = zs.mtime;
  if ((zs.valid & ZIP_STAT_CRC) != 0)
    m_crc = zs.crc;
}

entry::entry(const std::string& theName, const int index, const int64_t uncompressedSize,
             const int64_t compressedSize, const std::time_t modTime, const uint32_t CRC)
: m_name(theName),
  m_index(index),
  m_sizeUncompressed(uncompressedSize),
  m_sizeCompressed(compressedSize),
  m_m_time(modTime),
  m_crc(CRC)
{
  //"normalize" invalid values to "not set" values
  if (m_index < -1)
    m_index = -1;
  if (m_sizeCompressed < -1)
    m_sizeCompressed = -1;
  if (m_sizeUncompressed < -1)
    m_sizeUncompressed = -1;
}

const std::string& entry::name() const
{
  return m_name;
}

int entry::index() const
{
  return m_index;
}

int64_t entry::sizeUncompressed() const
{
  return m_sizeUncompressed;
}

int64_t entry::sizeCompressed() const
{
  return m_sizeCompressed;
}

std::time_t entry::m_time() const
{
  return m_m_time;
}

uint32_t entry::crc() const
{
  return m_crc;
}

bool entry::isDirectory() const
{
  /* Directory entries typically have a uncompressed size of zero bytes and
     their names end with a slash.
  */
  if (m_sizeUncompressed != 0)
    return false;
  const std::string::size_type len = m_name.size();
  if (len>0)
  {
    return (m_name[len-1] == '/');
  }
  else
    return false;
}

std::string entry::basename() const
{
  if (m_name.empty())
    return "";

  std::string result(m_name);
  if (result[result.size()-1] == '/')
    result.erase(result.size()-1);
  const std::string::size_type pos = result.rfind('/');
  if (pos == std::string::npos)
    return result;
  return result.substr(pos +1);
}

bool entry::operator==(const entry& other) const
{
  return ((m_name == other.m_name) && (m_index == other.m_index)
       && (m_sizeUncompressed == other.m_sizeUncompressed)
       && (m_sizeCompressed == other.m_sizeCompressed)
       && (m_m_time == other.m_m_time) && (m_crc == other.m_crc));
}

} //namespace

} //namespace
