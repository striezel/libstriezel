/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's common code library.
    Copyright (C) 2012  Thoronador

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

#include "StringConversion.hpp"
#include <iconv.h>
#include <cerrno>
#include <cstring>

namespace libthoro
{

namespace encoding
{

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
    //error occurred
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
    //error occurred
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

} //namespace encoding

} //namespace libthoro
