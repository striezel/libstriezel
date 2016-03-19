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

#ifndef LIBTHORO_ZIP_ENTRY_HPP
#define LIBTHORO_ZIP_ENTRY_HPP

#include <cstdint>
#include <ctime>
#include <string>
#include <zip.h>

namespace libthoro
{

namespace zip
{

class entry
{
  public:
    /** \brief constructor to create entry from zip_stat
     *
     * \param zs   zip_stat structure
     */
    entry(const struct zip_stat& zs);


    /** \brief gets the name of the file
     *
     * \return Returns name of the file.
     * May return an empty string, if file name is not known.
     */
    const std::string& name() const;


    /** \brief gets the index of the file in the zip archive
     *
     * \return Returns zero-based index of the entry in the ZIP file.
     * Returns -1, if index is not known.
     */
    int index() const;


    /** \brief gets the uncompressed size of the entry in bytes
     *
     * \return Returns the uncompressed size of the entry in bytes.
     * Returns -1, if the size is not known.
     */
    int64_t sizeUncompressed() const;


    /** \brief gets the compressed size of the entry in bytes
     *
     * \return Returns the compressed size of the entry in bytes.
     * Returns -1, if the size is not known.
     */
    int64_t sizeCompressed() const;


     /** \brief gets the modification time of the file
      *
      * \return Returns modification time of the file.
      * Returns -1 (cast to time_t), if the time is not known.
      */
    std::time_t m_time() const;


    /** \brief gets the CRC checksum of the file
     *
     * \return Returns the CRC checksum of the file.
     * Returns 0, if the checksum is not known.
     */
    uint32_t crc() const;


    /** \brief checks whether the entry is a directory
     *
     * \return Returns true, if the entry is a directory.
     *         Returns false, if not.
     *         Also returns false, if name or size is not known.
     */
    bool isDirectory() const;


    /** \brief strips directory name from the entry's name
     *
     * \return Returns the name with any leading directory components removed.
     */
    std::string basename() const;
  private:
    std::string m_name; /**< file name */
    int m_index;        /**< index of the file in the ZIP archive */
    int64_t m_sizeUncompressed; /**< size of file (uncompressed) in bytes */
    int64_t m_sizeCompressed;   /**< size of file (compressed) in bytes */
    std::time_t m_m_time;       /**< modification time */
    uint32_t m_crc;             /**< CRC checksum */
}; //class

} //namespace

} //namespace

#endif // LIBTHORO_ZIP_ENTRY_HPP
