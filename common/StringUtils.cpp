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

#include "StringUtils.h"
#include <sstream>

std::string toLowerString(std::string str)
{
  unsigned int i;
  const int diff = 'a'-'A';
  for (i=0; i<str.size(); ++i)
  {
    if (str[i]>='A' and str[i]<='Z')
    {
      str[i] = str[i] + diff;
    }
  }//for
  return str;
}

void trimLeft(std::string& str1)
{
  if (str1=="") return;
  //trim stuff at begin
  int len = str1.length();
  int pos = 0;
  bool go_on = true;
  while (go_on)
  {
    char ch = str1.at(pos);
    if ((ch==' ') or (ch=='\t'))
    {
      ++pos;
      go_on = (pos<len);
    }
    else
    {
      go_on = false;
    }
  }//while
  if (pos>=len)
  {
    str1 = "";
    return;
  }
  else if (pos>0)
  {
    str1.erase(0, pos);
  }
  return;
}

void trimRight(std::string& str1)
{
  if (str1.empty()) return;
  //trim stuff at end
  int len = str1.length();
  int pos = len-1;
  bool go_on = true;
  while (go_on)
  {
    char ch = str1.at(pos);
    if ((ch==' ') or (ch=='\t'))
    {
      --pos;
      go_on = (pos>=0);
    }
    else
    {
      go_on = false;
    }
  }//while
  if (pos==-1)
  {
    str1 = "";
  }
  else if (pos<len-1)
  {
    str1.erase(pos+1);
  }
  return;
}

void trim(std::string& str1)
{
  trimLeft(str1);
  trimRight(str1);
  return;
} //end of trim

std::string intToString(const int value)
{
  std::stringstream s_str;
  s_str << value;
  return s_str.str();
}
