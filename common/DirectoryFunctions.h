/*
 -------------------------------------------------------------------------------
    This file is part of the Thoronador's common code library.
    Copyright (C) 2011, 2012  Thoronador

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

#ifndef LIBTHORO_DIRECTORYFUNCTIONS_H
#define LIBTHORO_DIRECTORYFUNCTIONS_H

#include <string>

namespace Thoro
{
#if defined(_WIN32)
  const char pathDelimiter = '\\';
#elif defined(__linux__) || defined(linux)
  const char pathDelimiter = '/';
#else
  #error Unknown operating system!
#endif
} //namespace

/* Checks for existence of directory dirName and returns true, if it exists.

  parameters:
      dirName - the directory whose existence shall be determined
*/
bool directoryExists(const std::string& dirName);

/* tries to create the directory dirName and returns true, if the directory
   could be created, false otherwise.

   parameters:
       dirName - path/name of the directory that shall be created
*/
bool createDirectory(const std::string& dirName);

/* tries to create the directory dirName and returns true, if the directory
   could be created, false otherwise. If neccessary, the required parent
   directories will be created, too.

   parameters:
       dirName - path/name of the directory that shall be created
*/
bool createDirectoryRecursive(const std::string& dirName);

/* tries to determine the absolute path to the current user's home directory
   and returns that path in result in case of success. If the directory's path
   cannot be determined, the function returns false and the result string is
   unchanged.

   parameters:
       result - the string that will hold the result, if successful
*/
bool getHomeDirectory(std::string& result);

#endif // LIBTHORO_DIRECTORYFUNCTIONS_H
