/*
 -----------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2016  Dirk Stolle

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

#ifndef LIBSTRIEZEL_ARCHIVE_ISO9660_ARCHIVE_HPP
#define LIBSTRIEZEL_ARCHIVE_ISO9660_ARCHIVE_HPP

#include <string>
#include <vector>
#include "../archiveLibarchive.hpp"

namespace libstriezel
{

namespace archive
{

namespace iso9660
{

/** \brief class for handling Ar archives
 */
class archive: public libstriezel::archive::archiveLibarchive
{
  public:
     /** \brief constructor - opens an ar archive in read-only mode
      *
      * \param fileName  -  file name of the ar archive
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


    /** \brief checks whether a file may be an ISO9660 image
     *
     * \param fileName  file name of the potential ISO9660 image
     * \return Returns true, if the file seems to be an ISO9660 image.
     *         Returns false, if not.
     */
    static bool isISO9660(const std::string& fileName);
  private:
    /** \brief apply format support for ISO9660 images
     */
    void applyFormats();
}; //class

} //namespace

} //namespace

} //namespace

#endif // LIBSTRIEZEL_ARCHIVE_ISO9660_ARCHIVE_HPP
