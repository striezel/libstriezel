/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2011 thoronador

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

#ifndef STRINGUTILS_H
#define STRINGUTILS_H

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

/* Returns string representation of integer 'value' */
std::string intToString(const int value);

#endif // STRINGUTILS_H

