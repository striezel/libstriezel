/*
 -----------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2016, 2017, 2021  Dirk Stolle

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

#ifndef LIBSTRIEZEL_GZIP_ARCHIVE_HPP
#define LIBSTRIEZEL_GZIP_ARCHIVE_HPP

#include <string>
#include <vector>
#include <zlib.h>
#include "../entry.hpp"

namespace libstriezel::gzip
{

class archive
{
  public:
    /** \brief constructor - opens gzip-compressed file in read-only mode
     *
     * \param fileName  -  file name of the gzip-compressed file
     * \remarks This function throws an exception, if the file does not
     *          exist or a similar error occurs.
     */
    archive(const std::string& fileName);


    /** \brief destructor
     */
    virtual ~archive();


    /* Delete unwanted default copy constructor, assignment operator and
       move constructor. */
    archive(const archive& op) = delete;
    archive & operator=(const archive& op) = delete;
    archive(const archive&& op) = delete;


    /** \brief Gets a vector of all files within the gzip file.
     *
     * \return Returns a vector of all entries within the gzip file.
     * Returns an empty vector, if an error occurred.
     * \remarks For gzip files, there will be only one single entry.
     * However, this function is supplied to have an interface similar to the
     * other archive classes.
     */
    std::vector<libstriezel::archive::entry> entries() const;


    /** \brief Extracts the uncompressed file to the specified destination.
     *
     * \param destFileName  the destination file name - file must not exist yet
     * \return Returns true, if the file could be extracted successfully.
     *         Returns false, if the extraction failed.
     */
    bool extractTo(const std::string& destFileName);


    /** \brief Checks whether a file may be a gzip-compressed file.
     *
     * \param fileName  file name of the potential gzip-compressed file
     * \return Returns true, if the file seems to be a gzip-compressed file.
     *         Returns false, if not.
     */
    static bool isGzip(const std::string& fileName);
  private:
    gzFile m_gzip; /**< gzip file handle */
    std::vector<libstriezel::archive::entry> m_entries; /**< entries in the archive */
};

} // namespace

#endif // LIBSTRIEZEL_GZIP_ARCHIVE_HPP
