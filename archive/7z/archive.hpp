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

#ifndef LIBSTRIEZEL_7Z_ARCHIVE_HPP
#define LIBSTRIEZEL_7Z_ARCHIVE_HPP

#include <string>
#include <vector>
#include <archive.h>
#include "../archiveLibarchive.hpp"
#include "../entryLibarchive.hpp"

namespace libstriezel
{

namespace sevenZip
{

/** \brief archive class for 7z (seven zip) files
 */
class archive: public libstriezel::archive::archiveLibarchive
{
  public:
     /** \brief constructor - opens a 7z archive in read-only mode
      *
      * \param fileName  -  file name of the 7z archive
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


    /** \brief checks whether a file may be a 7z archive
     *
     * \param fileName  file name of the potential 7z archive
     * \return Returns true, if the file seems to be a 7z archive.
     *         Returns false, if not.
     */
    static bool is7z(const std::string& fileName);
  private:
    /** \brief apply format support for 7z archives
     */
    void applyFormats();
}; //class

} //namespace

//namespace alias
namespace _7z = sevenZip;

} //namespace

#endif // LIBSTRIEZEL_7Z_ARCHIVE_HPP
