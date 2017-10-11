/*
 -----------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2011, 2012, 2015, 2017  Dirk Stolle

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

#ifndef LIBSTRIEZEL_STRINGUTILS_HPP
#define LIBSTRIEZEL_STRINGUTILS_HPP

#include <string>

/** \brief returns the lower case version of the given string
 *
 * \param str  the string
 */
std::string toLowerString(std::string str);


/** \brief removes all leading and trailing spaces and (horizontal) tabulators
 * from the given string
 *
 * \param str1  the affected string
 */
void trim(std::string& str1);


/** \brief removes all leading spaces and (horizontal) tabulators
 * from the given string
 *
 * \param str1  the affected string
 */
void trimLeft(std::string& str1);


/** \brief removes all trailing spaces and (horizontal) tabulators
 * from the given string
 *
 * \param str1  the affected string
 */
void trimRight(std::string& str1);


/** \brief checks whether a string ends with another string
 *
 * \param str     the string where the search takes place
 * \param suffix  the "end" of the string for which we want to check
 * \return Returns true, if the string @str ends with @suffix.
 *         Returns false otherwise.
 */
bool stringEndsWith(const std::string& str, const std::string& suffix);


/* Returns string representation of integer 'value' */
std::string intToString(const int value);

/* Returns string representation of integer 'value' */
std::string uintToString(const unsigned int value);


/** \brief converts the string representation of an integer number into an int
 *
 * \param str   the string that contains the number
 * \param value the int that will be used to store the result
 * \return true on success, false on failure
 * \remarks If false is returned, the value of parameter value is undefined.
 */
bool stringToInt(const std::string& str, int& value);


/** \brief tries to convert the string representation of an unsigned integer number
 * into an unsigned int
 *
 * \param str  the string that contains the number
 * \param value the unsigned int that will be used to store the result
 * \return true on success, false on failure
 * \remarks If false is returned, the value of parameter value is undefined.
 */
bool stringToUnsignedInt(const std::string& str, unsigned int& value);


/** \brief converts the string representation of a floating point value into a float
 *
 * \param str    the string that contains the number
 * \param value  the float that will be used to store the result
 * \return true on success, false on failure
 * \remarks If false is returned, the value of parameter value is undefined.
 */
bool stringToFloat(const std::string& str, float& value);


/** \brief converts a floating point value into its string representation
 *
 * \param f  the floating point value
 * \return string representation
 */
std::string floatToString(const float f);


/** \brief case-insensitive version of std::basic_string's find()
 *
 * \param haystack   the string to search in
 * \param needle     the substring
 * \param pos        offset where the search starts
 */
std::string::size_type find_ci(const std::string& haystack, const std::string& needle, std::string::size_type pos = 0);


/** \brief checks whether a string is empty or consists only of whitespace characters
 *
 * \param str   the string that shall be checked
 * \return Returns true, if the string is empty or contains only whitespace characters.
 */
bool isEmptyOrWhitespace(const std::string& str);

#endif // LIBSTRIEZEL_STRINGUTILS_HPP
