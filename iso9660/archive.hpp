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

#ifndef LIBTHORO_ISO9660_ARCHIVE_HPP
#define LIBTHORO_ISO9660_ARCHIVE_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <cdio/iso9660.h>
#include "entry.hpp"

namespace libthoro
{

namespace iso9660
{

///class to read ISO 9660 CD images (usually with extension .iso)
class archive
{
  public:
    /** \brief constructor - opens an ISO9660 image in read-only mode
      *
      * \param fileName  -  file name of the ISO9660 image
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


    /** \brief gets a vector of all files within the ISO image
     *
     * \return Returns a vector of all entries within the ISO image.
     * Returns an empty vector, if an error occurred.
     */
    std::vector<entry> entries() const;


    /** \brief returns the Joliet level of the ISO file
     *
     * \return Returns the Joliet level of the .iso file.
     */
    uint8_t jolietLevel() const;


    /** \brief extracts the file at a given path in the ISO image to the specified destination
     *
     * \param destFileName  the destination file name - file must not exist yet
     * \param isoPath       path of the file that shall be extracted
     * \return Returns true, if the file could be extracted successfully.
     *         Returns false, if the extraction failed.
     */
    bool extractTo(const std::string& destFileName, const std::string& isoPath) const;


    /** \brief checks whether a file may be a ISO9660 image
     *
     * \param fileName  file name of the potential ISO9660 image
     * \return Returns true, if the file seems to be a ISO9660 image.
     *         Returns false, if not.
     */
    static bool isISO9660(const std::string& fileName);
  private:
    iso9660_t * m_iso; /**< handle to the ISO9660 file */

    void listFiles(std::vector<entry>& result, const std::string& directory) const;
}; //class

} //namespace

} //namespace

#endif // LIBTHORO_ISO9660_ARCHIVE_HPP
