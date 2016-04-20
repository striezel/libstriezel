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

#ifndef LIBTHORO_TAR_ARCHIVE_HPP
#define LIBTHORO_TAR_ARCHIVE_HPP

#include <string>
#include <vector>
#include <archive.h>
#include "../entryLibarchive.hpp"

namespace libthoro
{

namespace tar
{

//alias for entry class
using entry = libthoro::archive::entryLibarchive;

/** \brief archive class for tar (tape archive) files
 */
class archive
{
  public:
     /** \brief constructor - opens a tar archive in read-only mode
      *
      * \param fileName  -  file name of the tar archive
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


    /** \brief gets a vector of all files within the tar archive
     *
     * \return Returns a vector of all entries within the tar archive.
     * Returns an empty vector, if an error occurred.
     */
    std::vector<libthoro::archive::entryLibarchive> entries() const;


    /** \brief extracts the file at a given index to the specified destination
     *
     * \param destFileName  the destination file name - file must not exist yet
     * \param tarFilePath         path of the file that shall be extracted
     * \return Returns true, if the file could be extracted successfully.
     *         Returns false, if the extraction failed.
     */
    bool extractTo(const std::string& destFileName, const std::string& tarFilePath);


    /** \brief checks whether the archive contains a certain file
     *
     * \param fileName  path of the file whose existence shall be checked
     * \return Returns true, if the file exists. Returns false otherwise.
     */
    bool contains(const std::string& fileName) const;


    /** \brief checks whether a file may be a tar archive
     *
     * \param fileName  file name of the potential tar archive
     * \return Returns true, if the file seems to be a tar archive.
     *         Returns false, if not.
     */
    static bool isTar(const std::string& fileName);
  private:
    /** \brief fills the list of archive entries
     */
    void fillEntries();

    /** \brief re-opens the archive
     */
    void reopen();


    struct ::archive * m_archive; /**< archive handle */
    std::vector<libthoro::archive::entryLibarchive> m_entries; /**< the entries in the archive */
    std::string m_fileName; /**< original file name of archive */
}; //class

} //namespace

} //namespace

#endif // LIBTHORO_TAR_ARCHIVE_HPP
