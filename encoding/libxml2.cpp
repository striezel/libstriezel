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

#include "libxml2.hpp"
#include <cstring>
#include <iostream>
#include <libxml/encoding.h>

namespace libstriezel
{

namespace encoding
{

bool libxml2::UTF8_to_ISO_8859_1(const std::string& strUTF8, std::string& strISO)
{
  //maximum output size should not be more than length of UTF string (+ NUL)
  int outLen = strUTF8.size();
  unsigned char * outBuff = new unsigned char[outLen+1];
  memset(outBuff, '\0', outLen+1);

  const unsigned char * inPseudoBuff = reinterpret_cast<const unsigned char*>(strUTF8.c_str());
  int inLen = strUTF8.size();
  const int ret = UTF8Toisolat1(outBuff, &outLen, inPseudoBuff, &inLen);
  if (ret <= 0)
  {
    //conversion failed
    delete[] outBuff;
    return false;
  }

  if (ret > strUTF8.size())
  {
    //seems strange, would be more than buffer can hold
    delete[] outBuff;
    return false;
  }

  strISO = std::string(reinterpret_cast<const char*>(outBuff));
  delete[] outBuff;
  if (strISO.size() < ret)
  {
    std::cout << "Encoding error: got only " << strISO.size() << " chars, but expected " << ret << " chars.\n";
    return false;
  }
  return true;
}

} //namespace encoding

} //namespace libstriezel
