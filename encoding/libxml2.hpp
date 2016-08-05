/*
 -------------------------------------------------------------------------------
    This file is part of the Private Message Database.
    Copyright (C) 2015  Dirk Stolle

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

#ifndef LIBSTRIEZEL_ENCODING_LIBXML2_HPP
#define LIBSTRIEZEL_ENCODING_LIBXML2_HPP

#include <string>

namespace libstriezel
{

namespace encoding
{

/** \brief class that uses libxml2's encoding capabilities
 *
 * \remarks Since libxml2 implementations usually make use of iconv internally,
 *          you might as well use the string conversion functions provided in
 *          the header StringConversion.hpp, which directly uses iconv.
 */
class libxml2
{
  public:
    /** \brief converts an UTF-8-encoded string to an ISO-8859-1 encoded string
     *
     * \param strUTF8  input string, UTF8-encoded
     * \param strISO   used to return the output string, ISO-88559-1-encoded
     * \return Returns true in case of success, false in case of error.
     *
     */

    static bool UTF8_to_ISO_8859_1(const std::string& strUTF8, std::string& strISO);
}; //class

} //namespace encoding

} //namesoace libstriezel

#endif // LIBSTRIEZEL_ENCODING_LIBXML2_HPP
