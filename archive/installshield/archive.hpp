/*
 -----------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2017  Dirk Stolle

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

#ifndef LIBSTRIEZEL_INSTALLSHIELD_ARCHIVE_HPP
#define LIBSTRIEZEL_INSTALLSHIELD_ARCHIVE_HPP

#include <string>
#include <vector>
#include <libunshield.h>
#include "../entry.hpp"

namespace libstriezel
{

namespace installshield
{

class archive
{
  public:
     /** \brief constructor - opens an InstallShield archive in read-only mode
      *
      * \param fileName  -  file name of the archive
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


    /** \brief returns the number of entries in the archive
     *
     * \return Returns number of entries, if archive is open.
     * Returns -1, if an error occurred.
     */
    int64_t numEntries() const;


    /** \brief gets a vector of all files within the InstallShield archive
     *
     * \return Returns a vector of all entries within the InstallShield archive.
     * Returns an empty vector, if an error occurred.
     */
    std::vector<libstriezel::archive::entry> entries() const;


    /** \brief extracts the file at a given index to the specified destination
     *
     * \param destFileName  the destination file name - file must not exist yet
     * \param index         index of the entry that shall be extracted
     * \return Returns true, if the file could be extracted successfully.
     *         Returns false, if the extraction failed.
     */
    bool extractTo(const std::string& destFileName, int64_t index) const;


    /** \brief extracts the file with the given name to the specified destination
     *
     * \param destFileName  the destination file name - file must not exist yet
     * \param archiveFilePath      path of the file that shall be extracted
     * \return Returns true, if the file could be extracted successfully.
     *         Returns false, if the extraction failed.
     */
    bool extractTo(const std::string& destFileName, const std::string& archiveFilePath);


    /** \brief checks whether a file may be an InstallShield archive
     *
     * \param fileName  file name of the potential InstallShield archive
     * \return Returns true, if the file seems to be a InstallShield archive.
     *         Returns false, if not.
     */
    static bool isInstallShield(const std::string& fileName);
  private:
    /** \brief gets the error message for the archive
     *
     * \return Returns a string containing the error message.
     *         Might return an empty string, if there is no error.
     */
    std::string getError() const;


    Unshield * m_archive; /**< archive handle */
}; //class

} //namespace

} //namespace

#endif // LIBSTRIEZEL_INSTALLSHIELD_ARCHIVE_HPP
