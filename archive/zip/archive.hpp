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

#ifndef LIBSTRIEZEL_ZIP_ARCHIVE_HPP
#define LIBSTRIEZEL_ZIP_ARCHIVE_HPP

#include <string>
#include <vector>
#include <zip.h>
#include "entry.hpp"

namespace libstriezel::zip
{

class archive
{
  public:
     /** \brief constructor - opens a ZIP archive in read-only mode
      *
      * \param fileName  -  file name of the .zip archive
      * \remarks This function throws an exception, if the file does not
      *          exist or a similar error occurs.
      */
    archive(const std::string& fileName);


    /** \brief destructor
     */
    ~archive();


    /* Delete unwanted default copy constructor, assignment operator and
       move constructor. */
    archive(const archive& op) = delete;
    archive & operator=(const archive& op) = delete;
    archive(const archive&& op) = delete;


    /** \brief Returns the number of entries in the ZIP archive.
     *
     * \return Returns number of entries, if archive is open.
     * Returns -1, if an error occurred.
     */
    int64_t numEntries() const;


    /** \brief Gets a vector of all files within the ZIP archive.
     *
     * \return Returns a vector of all entries within the ZIP archive.
     * Returns an empty vector, if an error occurred.
     */
    std::vector<entry> entries() const;


    /** \brief Extracts the file at a given index to the specified destination.
     *
     * \param destFileName  the destination file name - file must not exist yet
     * \param index         index of the entry that shall be extracted
     * \return Returns true, if the file could be extracted successfully.
     *         Returns false, if the extraction failed.
     */
    bool extractTo(const std::string& destFileName, int64_t index) const;


    /** \brief Checks whether a file may be a ZIP archive.
     *
     * \param fileName  file name of the potential ZIP archive
     * \return Returns true, if the file seems to be a ZIP archive.
     *         Returns false, if not.
     */
    static bool isZip(const std::string& fileName);
  private:
    /** \brief Gets the error message for the archive.
     *
     * \return Returns a string containing the error message.
     *         Might return an empty string, if there is no error.
     */
    std::string getError() const;


    struct zip * m_archive; /**< zip archive handle */
};

} // namespace

#endif // LIBSTRIEZEL_ZIP_ARCHIVE_HPP
