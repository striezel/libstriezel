/*
 -----------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2016, 2017, 2021  Dirk Stolle

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

#ifndef LIBSTRIEZEL_XZ_ARCHIVE_HPP
#define LIBSTRIEZEL_XZ_ARCHIVE_HPP

#include <string>
#include <vector>
#include <archive.h>
#include "../archiveLibarchive.hpp"
#include "../entryLibarchive.hpp"

namespace libstriezel::xz
{

/** \brief class for handling xz archives
 */
class archive: public libstriezel::archive::archiveLibarchive
{
  public:
     /** \brief constructor - opens an xz archive in read-only mode
      *
      * \param fileName  -  file name of the xz archive
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


    /** \brief Checks whether a file may be an xz archive.
     *
     * \param fileName  file name of the potential xz archive
     * \return Returns true, if the file seems to be an xz archive.
     *         Returns false, if not.
     */
    static bool isXz(const std::string& fileName);


    /** \brief Extracts the file to the specified destination.
     *
     * \param destFileName  the destination file name - file must not exist yet
     * \param archiveFilePath      path of the file that shall be extracted
     * \return Returns true, if the file could be extracted successfully.
     *         Returns false, if the extraction failed.
     */
    virtual bool extractTo(const std::string& destFileName, const std::string& archiveFilePath) override;
  private:
    /** \brief Apply format support for xz archives.
     */
    void applyFormats() override;

    /** \brief Smoothen some edges on the entry data.
     */
    void postprocessEntry(const std::string& fileName);
};

} // namespace

#endif // LIBSTRIEZEL_XZ_ARCHIVE_HPP
