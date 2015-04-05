/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's common code library.
    Copyright (C) 2012, 2015  Thoronador

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

#ifndef LIBTHORO_STRINGCONVERSION_HPP
#define LIBTHORO_STRINGCONVERSION_HPP

#include <string>

namespace libthoro
{

namespace encoding
{

///error code constants
const int cseNone = 0;
const int cseFile = 1;
const int cseOutOfMemory = 2;
const int cseNotSupported = 3;
const int cseBadArgument = 4;
const int cseInvalidCode = 5;
const int cseBufferToSmall = 6;
const int cseUnknown = 7;

/** \brief converts a string from one codeset to another
 *
 * \param from   name of the original codeset (e.g. "utf8")
 * \param to     name of the target codeset (e.g. "iso-8859-15")
 * \param text   the text in the original encoding
 * \param output string that shall store the text in the target encoding
 * \remarks If this function fails, the string in output is not touched.
 * \return Returns zero in case of success.
 *         Returns non-zero error code in case of error.
 */
int convertString(const std::string& from, const std::string& to, const std::string& text, std::string& output);


/** \brief converts the UTF8-encoded string to an ISO-8859-1-encoded string
 *
 * \param in_utf8  input string, UTF8-encoded
 * \param out_iso  used to return the output string, ISO-88559-1-encoded
 * \return Returns true in case of success, false in case of error.
 * \remarks If an error occurred, the output string stays unchanged.
 */
inline bool utf8_to_iso8859_1(const std::string& in_utf8, std::string& out_iso)
{
  return (convertString("utf8", "iso-8859-1", in_utf8, out_iso)==cseNone);
}


/** \brief converts the ISO-8859-1-encoded string to an UTF8-encoded string
 *
 * \param in_iso  input string, ISO-8859-1-encoded (also called "latin1")
 * \param out_utf8 variable that is used to return the UTF8-encoded output
 * \return Returns true in case of success, false in case of error.
 * \remarks If an error occurred, the output string stays unchanged.
 */
inline bool iso8859_1_to_utf8(const std::string& in_iso, std::string& out_utf8)
{
  return (convertString("iso-8859-1", "utf8", in_iso, out_utf8)==cseNone);
}

} //namespace encoding

} //namespace libthoro

#endif // LIBTHORO_STRINGCONVERSION_HPP
