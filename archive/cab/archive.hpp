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

#ifndef LIBSTRIEZEL_CAB_ARCHIVE_HPP
#define LIBSTRIEZEL_CAB_ARCHIVE_HPP

#include "../archiveLibarchive.hpp"
#include "../entryLibarchive.hpp"

namespace libstriezel
{

namespace cab
{

//alias for entry
using entry = libstriezel::archive::entryLibarchive;

/** \brief class for handling Microsoft Cabinet archives
 */
class archive: public libstriezel::archive::archiveLibarchive
{
  public:
     /** \brief constructor - opens a Cabinet archive in read-only mode
      *
      * \param fileName  -  file name of the Cabinet archive
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


    /** \brief checks whether a file may be a Cabinet archive
     *
     * \param fileName  file name of the potential Cabinet archive
     * \return Returns true, if the file seems to be an Cabinet archive.
     *         Returns false, if not.
     */
    static bool isCab(const std::string& fileName);
  private:
    /** \brief apply format support for Cabinet archives
     */
    void applyFormats();
}; //class

} //namespace

} //namespace

#endif // LIBSTRIEZEL_AR_ARCHIVE_HPP
