/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2011, 2012, 2015  Thoronador

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

#ifndef LIBTHORO_STRINGUTILS_HPP
#define LIBTHORO_STRINGUTILS_HPP

#include <string>

/* returns the lower case version of the given string

   parameters:
       str - the string
*/
std::string toLowerString(std::string str);

/* removes all leading and trailing spaces and (horizontal) tabulators from the
   given string

   parameters:
       str1 - the affected string
*/
void trim(std::string& str1);

/* removes all leading spaces and (horizontal) tabulators from the given string

   parameters:
       str1 - the affected string
*/
void trimLeft(std::string& str1);

/* removes all trailing spaces and (horizontal) tabulators from the given string

   parameters:
       str1 - the affected string
*/
void trimRight(std::string& str1);


/* checks whether a string ends with another string

   parameters:
       str    - the string where the search takes place
       suffix - the "end" of the string for which we want to check

   return value:
       Returns true, if the string @str ends with @suffix.
       Returns false otherwise.
*/
bool stringEndsWith(const std::string& str, const std::string& suffix);

/* Returns string representation of integer 'value' */
std::string intToString(const int value);

/* Returns string representation of integer 'value' */
std::string uintToString(const unsigned int value);

/* tries to convert the string representation of an integer number into
   an int and returns true on success, false on failure.

   parameters:
       str   - the string that contains the number
       value - the int that will be used to store the result

   remarks:
       If false is returned, the value of parameter value is undefined.
*/
bool stringToInt(const std::string& str, int& value);

/* tries to convert the string representation of an unsigned integer number
   into an unsigned int and returns true on success, false on failure.

   parameters:
       str   - the string that contains the number
       value - the unsigned int that will be used to store the result

   remarks:
       If false is returned, the value of parameter value is undefined.
*/
bool stringToUnsignedInt(const std::string& str, unsigned int& value);

/* tries to convert the string representation of a floating point value into
   a float and returns true on success, false on failure.

   parameters:
       str   - the string that contains the number
       value - the float that will be used to store the result

   remarks:
       If false is returned, the value of parameter value is undefined.
*/
bool stringToFloat(const std::string& str, float& value);

/* converts a floating point value into its string representation

   parameters:
       f - the floating point value
*/
std::string floatToString(const float f);

/* case-insensitive version of std::basic_string's find()

   parameters:
       haystack - the string to search in
       needle   - the substring
       pos      - offset where the search starts
*/
std::string::size_type find_ci(const std::string& haystack, const std::string& needle, std::string::size_type pos = 0);

#endif // LIBTHORO_STRINGUTILS_HPP
