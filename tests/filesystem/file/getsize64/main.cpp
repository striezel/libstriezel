/*
 -----------------------------------------------------------------------------
    This file is part of a test suite for striezel's common code library.
    Copyright (C) 2016, 2024  Dirk Stolle

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

#include <fstream>
#include <iostream>
#include <vector>
#include "../../../../filesystem/file.hpp"

const std::vector<std::int64_t> testSizes =
        {
            0,
            1,
            16,
            256,
            1000,
            4096,
            4097,
            25000,
            128000,
            1000000, // one million bytes
            1024*1024, // 1 MB
            5*1024*1024+123 // somewhere near 5 MB
        };

int main()
{
  // create temp file for tests
  std::string fileName = "";
  if (!libstriezel::filesystem::file::createTemp(fileName))
  {
    std::cout << "Error: Could not create temporary file!" << std::endl;
    return 1;
  }

  for (const auto item : testSizes)
  {
    // generate file from data
    std::ofstream stream;
    stream.open(fileName, std::ios_base::trunc | std::ios_base::binary | std::ios_base::out);
    if (!stream.good() || !stream.is_open())
    {
      std::cout << "Error: Could not create/open temporary file!" << std::endl;
      return 1;
    }
    for (std::int64_t i = 0; i < item; ++i)
    {
      stream.put('\0');
    }
    if (!stream.good())
    {
      std::cout << "Error: Could not write data to temporary file!" << std::endl;
      stream.close();
      libstriezel::filesystem::file::remove(fileName);
      return 1;
    }
    stream.close();

    // check file size
    const auto s = libstriezel::filesystem::file::getSize64(fileName);
    if (s != item)
    {
      std::cout << "Error: File size of " << fileName << " should be " << item
                << " bytes, but getSize64() returns " << s << " instead!" << std::endl;
      libstriezel::filesystem::file::remove(fileName);
      return 1;
    }
  }

  // delete file
  libstriezel::filesystem::file::remove(fileName);
  // Everything is OK.
  std::cout << "Test for file::getSize64() passed!" << std::endl;
  return 0;
}
