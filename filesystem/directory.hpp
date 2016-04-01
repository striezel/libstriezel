/*
 -------------------------------------------------------------------------------
    This file is part of the Thoronador's common code library.
    Copyright (C) 2011, 2012, 2015, 2016  Thoronador

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
 -------------------------------------------------------------------------------
*/

#ifndef LIBTHORO_DIRECTORY_HPP
#define LIBTHORO_DIRECTORY_HPP

#include <string>

namespace libthoro
{

namespace filesystem
{

#if defined(_WIN32)
  const char pathDelimiter = '\\';
#elif defined(__linux__) || defined(linux)
  const char pathDelimiter = '/';
#else
  #error Unknown operating system!
#endif


class directory
{
  public:
    /* Checks for existence of directory dirName and returns true, if it exists.

       parameters:
           dirName - the directory whose existence shall be determined
    */
    static bool exists(const std::string& dirName);


    /* tries to create the directory dirName and returns true, if the directory
       could be created, false otherwise.

       parameters:
           dirName - path/name of the directory that shall be created
    */
    static bool create(const std::string& dirName);


    /* tries to create the directory dirName and returns true, if the directory
       could be created, false otherwise. If necessary, the required parent
       directories will be created, too.

       parameters:
           dirName - path/name of the directory that shall be created
    */
    static bool createRecursive(const std::string& dirName);


    /** \brief tries to create a temporary directory
     *
     * \param tempDirName  string that will hold the name of the temporary directory
     * \return Returns true, if a temporary directory was created. Returns false otherwise.
     */
    static bool createTemp(std::string& tempDirName);


     /** \brief tries to remove the given directory
      *
      * \param dirName  the name of the directory that shall be removed
      * \remarks Details of removing a directory are up to the operating
      *          system's implementation, but usually a directory can only
      *          be removed, if it is empty and no process uses it as a
      *          working directory. Therefore, trying to remove a non-empty
      *          directory will fail.
      * \return Returns true, if the directory was removed.
      *         Returns false otherwise.
      */
    static bool remove(const std::string& dirName);


    /* tries to determine the absolute path to the current user's home directory
       and returns that path in result in case of success. If the directory's path
       cannot be determined, the function returns false and the result string is
       unchanged.

       parameters:
           result - the string that will hold the result, if successful
    */
    static bool getHome(std::string& result);
}; //class Directory


/* adds a slash or backslash (or whatever is the path delimiter on the current
   system) to the given path, if the path is not empty and has no path delimiter
   as the last character yet.

   parameters:
       path - the path that should (possibly) have an (back)slash
*/
std::string slashify(const std::string& path);

/* removes a slash or backslash (or whatever is the path delimiter on the
   current system) from the given path, if the path is not empty and has a path
   delimiter as the last character.

   parameters:
       path - the path that (possibly) has an (back)slash too much
*/
std::string unslashify(const std::string& path);

} //namespace filesystem

} //namespace libthoro

#endif // LIBTHORO_DIRECTORY_HPP
