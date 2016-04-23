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

#ifndef LIBTHORO_ARCHIVE_ARCHIVELIBARCHIVE_HPP
#define LIBTHORO_ARCHIVE_ARCHIVELIBARCHIVE_HPP

#include <string>
#include <vector>
#include <archive.h>
#include "entryLibarchive.hpp"

namespace libthoro
{

namespace archive
{

/** \brief archive class for files opened with libarchive
 */
class archiveLibarchive
{
  public:
     /** \brief default constructor
      *
      * \param fileName  -  file name of the archive
      */
    archiveLibarchive(const std::string& fileName);


    /** \brief destructor
     */
    virtual ~archiveLibarchive();


    /* Delete unwanted default copy constructor, assignment operator and
       move constructor. */
    archiveLibarchive(const archiveLibarchive& op) = delete;
    archiveLibarchive & operator=(const archiveLibarchive& op) = delete;
    archiveLibarchive(const archiveLibarchive&& op) = delete;


    /** \brief gets a vector of all files within the archive
     *
     * \return Returns a vector of all entries within the archive.
     * Returns an empty vector, if an error occurred.
     */
    std::vector<libthoro::archive::entryLibarchive> entries() const;


    /** \brief checks whether the archive contains a certain file
     *
     * \param fileName  path of the file whose existence shall be checked
     * \return Returns true, if the file exists. Returns false otherwise.
     */
    bool contains(const std::string& fileName) const;
  protected:
    /** \brief fills the list of archive entries
     */
    void fillEntries();

    /** \brief re-opens the archive
     */
    void reopen();

    /** \brief apply format support for supported archive types
     */
    virtual void applyFormats() = 0;


    struct ::archive * m_archive; /**< archive handle */
    std::vector<libthoro::archive::entryLibarchive> m_entries; /**< the entries in the archive */
    std::string m_fileName; /**< original file name of archive */
}; //class

} //namespace

} //namespace

#endif // LIBTHORO_ARCHIVE_ARCHIVELIBARCHIVE_HPP
