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

#ifndef LIBTHORO_ZIP_ARCHIVE_HPP
#define LIBTHORO_ZIP_ARCHIVE_HPP

#include <string>
#include <zip.h>

namespace libthoro
{

namespace zip
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


    /** \brief returns the number of entries in the ZIP archive
     *
     * \return Returns number of entries, if archive is open.
     * Returns -1, if an error occurred.
     */
    int64_t numEntries() const;
  private:
    struct zip * m_archive;
}; //class

} //namespace

} //namespace

#endif // LIBTHORO_ZIP_ARCHIVE_HPP
