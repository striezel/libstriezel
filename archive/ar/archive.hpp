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

#ifndef LIBTHORO_AR_ARCHIVE_HPP
#define LIBTHORO_AR_ARCHIVE_HPP

#include <string>
#include <vector>
#include <archive.h>
#include "../archiveLibarchive.hpp"
#include "../entryLibarchive.hpp"

namespace libthoro
{

namespace ar
{

//alias for entry
using entry = libthoro::archive::entryLibarchive;

/** \brief class for handling Ar archives
 */
class archive: public libthoro::archive::archiveLibarchive
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


    /** \brief checks whether a file may be an ar archive
     *
     * \param fileName  file name of the potential ar archive
     * \return Returns true, if the file seems to be an ar archive.
     *         Returns false, if not.
     */
    static bool isAr(const std::string& fileName);
  private:
    /** \brief apply format support for Ar archives
     */
    void applyFormats();
}; //class

} //namespace

} //namespace

#endif // LIBTHORO_AR_ARCHIVE_HPP
