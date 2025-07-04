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

#include "StringUtils.hpp"
#include <cctype>
#include <sstream>
#include <limits>

std::string toLowerString(std::string str)
{
  const int diff = 'a'-'A';
  for (unsigned int i = 0; i < str.size(); ++i)
  {
    if (str[i] >= 'A' && str[i] <= 'Z')
    {
      str[i] = str[i] + diff;
    }
  }//for
  return str;
}

void trimLeft(std::string& str1)
{
  if (str1.empty()) return;
  //trim stuff at begin
  int len = str1.length();
  int pos = 0;
  bool go_on = true;
  while (go_on)
  {
    const char ch = str1.at(pos);
    if ((ch == ' ') || (ch == '\t'))
    {
      ++pos;
      go_on = (pos < len);
    }
    else
    {
      go_on = false;
    }
  }//while
  if (pos>=len)
  {
    str1.clear();
    return;
  }
  else if (pos > 0)
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
    const char ch = str1.at(pos);
    if ((ch == ' ') || (ch == '\t'))
    {
      --pos;
      go_on = (pos >= 0);
    }
    else
    {
      go_on = false;
    }
  }//while
  if (pos == -1)
  {
    str1.clear();
  }
  else if (pos < len - 1)
  {
    str1.erase(pos + 1);
  }
  return;
}

void trim(std::string& str1)
{
  trimLeft(str1);
  trimRight(str1);
  return;
} //end of trim

bool stringEndsWith(const std::string& str, const std::string& suffix)
{
  const std::string::size_type strLen = str.size();
  const std::string::size_type suffixLen = suffix.size();
  if (strLen < suffixLen)
    return false;
  return (str.substr(strLen - suffixLen).compare(suffix) == 0);
}

std::string intToString(const int value)
{
  std::ostringstream s_str;
  s_str << value;
  return s_str.str();
}

std::string uintToString(const unsigned int value)
{
  std::ostringstream s_str;
  s_str << value;
  return s_str.str();
}

bool stringToInt(const std::string& str, int& value)
{
  if (str.empty()) return false;
  value = 0;
  unsigned int i;
  const int cTenthLimit = std::numeric_limits<int>::max() / 10;
  const int cRealLimit = std::numeric_limits<int>::max();
  bool negative;
  if (str.at(0) == '-')
  {
    i = 1;
    negative = true;
  }
  else
  {
    i = 0;
    negative = false;
  }
  for ( ; i < str.size(); ++i)
  {
    if ((str.at(i) >= '0') && (str.at(i) <= '9'))
    {
      /* If the result of the multiplication in the next line would go out of
         the type range, then the result is not useful anyway, so quit here. */
      if (value > cTenthLimit)
        return false;
      value = value * 10;
      /* If the result of the addition in the next line would go out of the
         type's range, then the result is not useful anyway, so quit here. */
      if (value > cRealLimit-(str.at(i) - '0')) return false;
      value = value + (str.at(i) - '0');
    }//if
    else
    {
      //unknown or invalid character detected
      return false;
    }
  }//for
  if (negative)
    value = -value;
  return true;
}

bool stringToUnsignedInt(const std::string& str, unsigned int& value)
{
  if (str.empty()) return false;
  value = 0;
  const unsigned int cTenthLimit = std::numeric_limits<unsigned int>::max() / 10;
  const unsigned int cRealLimit = std::numeric_limits<unsigned int>::max();
  std::string::size_type i = 0;
  for ( ; i < str.size(); ++i)
  {
    if ((str.at(i) >= '0') && (str.at(i) <= '9'))
    {
      /* If the result of the multiplication in the next line would go out of
         the type range, then the result is not useful anyway, so quit here. */
      if (value > cTenthLimit)
        return false;
      value = value * 10;
      /* If the result of the addition in the next line would go out of the
         type's range, then the result is not useful anyway, so quit here. */
      if (value > cRealLimit - (str.at(i) - '0'))
        return false;
      value = value + (str.at(i) - '0');
    }//if
    else
    {
      //unknown or invalid character detected
      return false;
    }
  }//for
  return true;
}

//tries to get the floating point representation of a string
bool stringToFloat(const std::string& str, float& value)
{
  if (str.empty())
    return false;
  value = 0.0f;
  unsigned int i, next_look;
  bool negative;
  if (str.at(0) == '-')
  {
    i = 1;
    negative = true;
    next_look = 1;
  }
  else
  {
    i = 0;
    negative = false;
    next_look = 0;
  }
  for ( ; i < str.length(); ++i)
  {
    if ((str.at(i) >= '0') && (str.at(i) <= '9'))
    {
      value = value * 10.0f;
      value = value + (str.at(i) - '0');
      ++next_look;
    }//if
    else if (str.at(i) == '.')
    {
      //decimal separator found - break out of loop
      next_look = i + 1;
      break;
    }
    else
    {
      //unknown or invalid character detected
      return false;
    }
  }//for
  //now go for the stuff after the separator
  float second = 0.0f;
  for (i = str.length() - 1; i >= next_look; --i)
  {
    if ((str.at(i) >= '0') && (str.at(i) <= '9'))
    {
      second = second + (str.at(i) - '0');
      second = second / 10.0f;
    }//if
    else
    {
      //unknown or invalid character detected
      return false;
    }
  }//for, second loop
  value = value + second;
  if (negative)
    value = -value;
  return true;
}

std::string floatToString(const float f)
{
  std::stringstream s_str;
  s_str << f;
  return s_str.str();
}

std::string::size_type find_ci(const std::string& haystack, const std::string& needle, std::string::size_type pos)
{
  const std::string::size_type lenHay = haystack.length();
  const std::string::size_type lenNeedle = needle.length();

  if ((pos >= lenHay) || (lenNeedle > lenHay) || (lenNeedle <= 0)) return std::string::npos;

  for (; pos < lenHay; ++pos)
  {
    std::string::size_type s = 0;
    for (std::string::size_type j = 0; j < lenNeedle; ++j)
    {
      if (!std::string::traits_type::eq(std::toupper(haystack[pos+j]), std::toupper(needle[j])))
        break;
      ++s;
    } //for j
    if (lenNeedle == s)
      return pos;
  } //for i/pos
  return std::string::npos;
}

bool isEmptyOrWhitespace(const std::string& str)
{
  if (str.empty())
    return true;
  std::string::const_iterator iter = str.begin();
  while (iter != str.end())
  {
    const int charAsInt = static_cast<int>(*iter);
    if ((charAsInt > 127) || (!std::isspace(*iter)))
      return false;
    ++iter;
  } //while
  return true;
}
