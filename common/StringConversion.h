/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's common code library.
    Copyright (C) 2012 thoronador

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

#ifndef LIBTHORO_STRINGCONVERSION_H
#define LIBTHORO_STRINGCONVERSION_H

#include <iconv.h>
#include <cerrno>
#include <cstring>

namespace Thoro
{

//error code constants
const int cseNone = 0;
const int cseFile = 1;
const int cseOutOfMemory = 2;
const int cseNotSupported = 3;
const int cseBadArgument = 4;
const int cseInvalidCode = 5;
const int cseBufferToSmall = 6;
const int cseUnknown = 7;

/* converts a string from one codeset to another

   parameters:
       from - name of the original codeset (e.g. "utf8")
       to   - name of the target codeset (e.g. "iso-8859-15")
       text - the text in the original encoding
       output - string that shall store the text in the target encoding

   remarks:
       If this function fails, the string in output is not touched.
*/
int convertString(const std::string& from, const std::string& to, const std::string& text, std::string& output)
{
  if (text.empty())
  {
    output = "";
    return cseNone;
  }
  iconv_t cd = iconv_open(to.c_str(), from.c_str());
  if (cd==(iconv_t) -1)
  {
    //error occured
    switch (errno)
    {
      case EMFILE:
      case ENFILE:
           return cseFile;
           break;
      case ENOMEM:
           return cseOutOfMemory;
           break;
      case EINVAL:
           return cseNotSupported;
           break;
      default:
           return cseUnknown;
    }//swi
  }//if
  const std::string::size_type text_len = text.length();
  char * buffer = NULL;
  try
  {
    buffer = new char[text_len+1];
  }
  catch (...)
  {
    iconv_close(cd);
    return cseOutOfMemory;
  }
  const char * buffPointer = buffer;
  memcpy(buffer, text.c_str(), text_len+1);
  char * buffOutput = NULL;
  try
  {
    buffOutput = new char[4*text_len+1];
  }
  catch (...)
  {
    iconv_close(cd);
    delete[] buffer;
    return cseOutOfMemory;
  }
  const char * buffOutputPtr = buffOutput;
  memset(buffOutput, '\0', 4*text_len+1);

  size_t in_size_left = text_len;
  size_t out_size_left = 4*text_len+1;

  size_t result = iconv(cd, &buffer, &in_size_left, &buffOutput, &out_size_left);
  if (((size_t)-1) == result)
  {
    //error occured
    delete[] buffPointer;
    delete[] buffOutputPtr;
    iconv_close(cd);
    switch (errno)
    {
      case EBADF:
           return cseBadArgument;
      case EILSEQ:
      case EINVAL:
           return cseInvalidCode;
      case E2BIG:
           return cseBufferToSmall;
      default:
           return cseUnknown;
    }//swi
  }//if error
  iconv_close(cd);
  delete [] buffPointer;
  output = std::string(buffOutputPtr);
  delete [] buffOutputPtr;
  return cseNone;
}//function

/* converts the UTF8-encoded string given as input to a ISO-8859-1-encoded
   string and returns the converted string in out_iso. Returns true in case of
   success, false in case of error. If an error occured, the output string
   stays unchanged.
*/
inline bool utf8_to_iso8859_1(const std::string& in_utf8, std::string& out_iso)
{
  return (convertString("utf8", "iso-8859-1", in_utf8, out_iso)==cseNone);
}

/* converts the ISO-8859-1-encoded string given as input to a UTF8-encoded
   string and returns the converted string in out_utf8. Returns true in case of
   success, false in case of error. If an error occured, the output string
   stays unchanged.
*/
inline bool iso8859_1_to_utf8(const std::string& in_iso, std::string& out_utf8)
{
  return (convertString("iso-8859-1", "utf8", in_iso, out_utf8)==cseNone);
}

} //namespace

#endif // LIBTHORO_STRINGCONVERSION_H
