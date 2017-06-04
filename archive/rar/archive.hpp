/*
 -----------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2016, 2017  Dirk Stolle

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

#ifndef LIBSTRIEZEL_RAR_ARCHIVE_HPP
#define LIBSTRIEZEL_RAR_ARCHIVE_HPP

#include <string>
#include <vector>
#include <archive.h>
#include "../archiveLibarchive.hpp"
#include "../entryLibarchive.hpp"

namespace libstriezel
{

namespace rar
{

/** \brief archive class for Roschal archives (*.rar)
 *
 * \remarks Due to the fact that Rar is a proprietary format not all files
 * contained in a Roschal archive may be extractable with the help of this
 * class.
 * So use it with care.
 */
class archive: public libstriezel::archive::archiveLibarchive
{
  public:
     /** \brief constructor - opens a Roschal archive in read-only mode
      *
      * \param fileName  -  file name of the Roschal archive
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


    /** \brief checks whether a file may be a Roschal archive
     *
     * \param fileName  file name of the potential Roschal archive
     * \return Returns true, if the file seems to be a rar archive.
     *         Returns false, if not.
     */
    static bool isRar(const std::string& fileName);
  private:
    /** \brief apply format support for rar
     */
    void applyFormats();
}; //class

} //namespace

} //namespace

#endif // LIBSTRIEZEL_RAR_ARCHIVE_HPP
