/*
 -------------------------------------------------------------------------------
    This file is part of the Morrowind Tools Project.
    Copyright (C) 2011, 2015  Thoronador

    The Morrowind Tools are free software: you can redistribute them and/or
    modify them under the terms of the GNU General Public License as published
    by the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Morrowind Tools are distributed in the hope that they will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with the Morrowind Tools.  If not, see <http://www.gnu.org/licenses/>.
 -------------------------------------------------------------------------------
*/

#ifndef LIBTHORO_FILEFUNCTIONS_H
#define LIBTHORO_FILEFUNCTIONS_H

#include <string>
#include <stdint.h>
#include <vector>

namespace libthoro
{

namespace filesystem
{

class File
{
  public:
    /* returns the file size of file fileName. If the file does not exist or if an
       error occurred, -1 will be returned.

       parameters:
           FileName - the file whose size shall be obtained
    */
    static int64_t getSize64(const std::string& fileName);


    /* sets the time when file FileName was last modified. If the file does not
       exist or if an error occurred, false will be returned.

       parameters:
           FileName - the file whose time of last modification shall be changed
    */
    static bool setModificationTime(const std::string& FileName, const time_t new_mtime);


    /* returns the file size of file FileName and the time of its last modification.
       If the file does not exist or if an error occurred, false will be returned and
       the values will be set to -1.

       parameters:
           FileName - the file whose size shall be obtained
           FileSize - reference to the 64bit integer that will store the file's size
           FileTime - reference to the time_t that will store the file's mtime
    */
    static bool getSizeAndModificationTime(const std::string& FileName, int64_t& FileSize, time_t& FileTime);


    /* Checks for existence of file FileName and returns true, if it exists.

       parameters:
           FileName - the file whose existence shall be determined
    */
    static bool exists(const std::string& FileName);


    /* tries to delete the file fileName and returns true, if it could be deleted,
       or false if an error occurred.

       parameters:
           fileName - the file that shall be deleted
    */
    static bool remove(const std::string& fileName);


    /** \brief tries to create a temporary file
     *
     * \param tempFileName  string that will hold the file name of the temp file
     * \return Returns true, if a temporary file was created. Returns false otherwise.
     */
    static bool createTemp(std::string& tempFileName);


    /** \brief tries to read the complete(!) content of a file into a string
     *
     * \param fileName  the name of the file which shall be read
     * \param content   variable that will be used to store the read content
     * \return Returns true, if the complete file was successfully read.
     *         Returns false, if an error occurred.
     * \remarks This functions should only be used to read the content of
     *          small files, because storing several gigabytes of data in a
     *          string will cause problems on most systems.
     */
    static bool readIntoString(const std::string& fileName, std::string& content);
}; //class File


/* returns a human readable, nicer string representing the file size passed
   as fileSize

   parameters:
       fileSize - size of the file in bytes
*/
std::string getSizeString(const int64_t fileSize);


/* structure for file list entries */
struct FileEntry {
         std::string fileName;
         bool isDirectory;
};//struct

/* returns a list of all files in the given directory as a vector */
std::vector<FileEntry> getDirectoryFileList(const std::string& Directory);

/* splits the file name in fileName into path, file and extension

   parameters:
       fileName      - the full file name
       pathSeparator - the path separator, i.e. '/' or '\\'
       path          - string that will be used to store the path component
       name          - string that will be used to store the name component
       extension     - string that will be used to store the extension

   remarks:
       Calling the function like splitPathFileExtension("/home/user/path/readme.txt", '/', ...)
       will give the following results:
           path: "/home/user/path/" (i.e. includes path seperator at the end)
           name: "readme"
           ext.: "txt" (i.e. without dot)
*/
void splitPathFileExtension(const std::string& fileName, const char pathSeparator, std::string& path, std::string& name, std::string& extension);

} //namespace filesystem

} //namespace libthoro

#endif // LIBTHORO_FILEFUNCTIONS_H
