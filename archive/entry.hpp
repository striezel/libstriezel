/*
 -----------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2016, 2021  Dirk Stolle

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

#ifndef LIBSTRIEZEL_ARCHIVE_ENTRY_HPP
#define LIBSTRIEZEL_ARCHIVE_ENTRY_HPP

#include <cstdint>
#include <ctime>
#include <string>

namespace libstriezel::archive
{

/** \brief base class to represent an entry within an arbitrary archive
 */
class entry
{
  public:
    /** \brief default constructor to create empty entry
     */
    entry();


    /** \brief empty virtual destructor
     */
    virtual ~entry();

    /** \brief Gets the name of the file.
     *
     * \return Returns name of the file.
     * May return an empty string, if file name is not known.
     */
    const std::string& name() const;


    /** \brief Gets the size of the entry in bytes.
     *
     * \return Returns the size of the entry in bytes.
     * Returns -1, if the size is not known.
     */
    int64_t size() const;


    /** \brief Gets the modification time of the file.
     *
     * \return Returns modification time of the file.
     * Returns -1 (cast to time_t), if the time is not known.
     */
    std::time_t m_time() const;


    /** \brief Checks whether the entry is a directory.
     *
     * \return Returns true, if the entry is a directory.
     *         Returns false, if not.
     *         Also returns false, if name or size is not known.
     */
    virtual bool isDirectory() const;


    /** \brief Checks whether the entry is a symbolic link.
     *
     * \return Returns true, if the entry is a symbolic link.
     *         Returns false, if not.
     */
    virtual bool isSymLink() const;


    /** \brief Strips directory name from the entry's name.
     *
     * \return Returns the name with any leading directory components removed.
     */
    std::string basename() const;


    /** \brief Sets the name of the file.
     *
     * \param name  the new name of the file
     */
    void setName(const std::string& name);


    /** \brief Sets the size of the file (in bytes).
     *
     * \param sizeInBytes  the new size of the file in bytes
     */
    void setSize(const int64_t sizeInBytes);


    /** \brief Sets the modification time of the file.
     *
     * \param modTime  the new modification time of the file
     */
    void setTime(const std::time_t modTime);


    /** \brief Sets whether or not the entry represents a directory.
     *
     * \param isDir   true, if entry is a directory; set to false, if not
     */
    void setDirectory(const bool isDir);


    /** \brief Sets whether or not the entry represents a symbolic link.
     *
     * \param isSymbolic  true, if entry is a symbolic link; set to false, if not
     */
    void setSymLink(const bool isSymbolic);
  private:
    std::string m_name; /**< file name */
    int64_t m_size; /**< size of file in bytes */
    bool m_directory; /**< whether or not this entry is a directory */
    bool m_link; /**< whether or not this entry is a symbolic link */
    std::time_t m_m_time; /**< modification time */
};

} // namespace

#endif // LIBSTRIEZEL_ARCHIVE_ENTRY_HPP
