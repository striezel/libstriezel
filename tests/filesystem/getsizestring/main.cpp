/*
 -----------------------------------------------------------------------------
    This file is part of a test suite for Thoronador's common code library.
    Copyright (C) 2016  Thoronador

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

#include <iostream>
#include <utility>
#include <vector>
#include "../../../filesystem/file.hpp"

const std::vector< std::pair<int64_t, std::string> > cases
  = {
    { 1, "1 byte" },
    { 1024, "1024 byte" },
    { 1025, "1 KB" },
    { 1536, "1.5 KB" },
    { 1792, "1.75 KB" },
    { 4096, "4 KB" },
    { 1024*1024, "1024 KB" },
    { 1024*1024+1, "1 MB" },
    { 1792*1024, "1.75 MB" },
    { 1024*1024*20, "20 MB" },
    { 33816576, "32.25 MB" },
    { 1073479680, "1023.75 MB" },
    { 1024*1024*1024, "1024 MB" },
    { 1024*1024*1024+1, "1 GB" },
    { 1792*1024*1024, "1.75 GB" },
    /* Cast first component to int64_t to force the resulting type to be int64_t
       as well, because otherwise the integer expression would cause an overflow
       on 32 bit systems. */
    { static_cast<int64_t>(1024)*1024*1024*1024, "1024 GB" }
  };

int main()
{
  for (const auto & item : cases)
  {
    const auto str = libthoro::filesystem::getSizeString(item.first);
    std::cout << "getSizeString(" << item.first << ") = " << str << std::endl;
    if (str != item.second)
    {
      std::cout << "Error: getSizeString(" << item.first << ") should return "
                << item.second << "!" << std::endl;
      return 1;
    }
  } //for
  std::cout << "Test for getSizeString() passed!" << std::endl;
  return 0;
}
