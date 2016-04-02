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

#ifndef LIBTHORO_ISO9660_ENTRY_HPP
#define LIBTHORO_ISO9660_ENTRY_HPP

#include <cstdint>
#include <ctime>
#include <string>
#include <cdio/iso9660.h>

namespace libthoro
{

namespace iso9660
{

class entry
{
  public:
    /** \brief constructor to create entry from ISO9660 stat buffer
     *
     * \param statbuf   pointer to ISO9660 stat buffer, may not be null
     * \param currentDirectory the current directory
     */
    entry(const iso9660_stat_t * statbuf, const std::string& currentDirectory);


    /** \brief gets the name of the file
     *
     * \return Returns name of the file.
     * May return an empty string, if file name is not known.
     */
    const std::string& name() const;


    /** \brief gets the size of the entry in bytes
     *
     * \return Returns the size of the entry in bytes.
     * Returns -1, if the size is not known.
     */
    int64_t size() const;


    /** \brief gets the modification time of the file
     *
     * \return Returns modification time of the file.
     * Returns -1 (cast to time_t), if the time is not known.
     */
    std::time_t m_time() const;


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
    int64_t m_size; /**< size of file in bytes */
    bool m_directory; /**< whether or not this entry is a directory */
    std::time_t m_m_time; /**< modification time */
}; //class

} //namespace

} //namespace

#endif // LIBTHORO_ISO9660_ENTRY_HPP
