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

#include <ctime>
#include <fstream>
#include <iostream>
#include <vector>
#include "../../../../filesystem/file.hpp"

const std::vector<std::size_t> testSizes =
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
            1000000, //one million bytes
            1024*1024, // 1 MB
            5*1024*1024+123 // somewhere near 5 MB
        };

int main()
{
  //create temp file for tests
  std::string fileName = "";
  if (!libthoro::filesystem::file::createTemp(fileName))
  {
    std::cout << "Error: Could not create temporary file!"   << std::endl;
    return 1;
  }

  for (const auto item : testSizes)
  {
    //generate file with specified size
    std::ofstream stream;
    stream.open(fileName, std::ios_base::trunc | std::ios_base::binary | std::ios_base::out);
    if (!stream.good() || !stream.is_open())
    {
      std::cout << "Error: could not create/open temporary file!" << std::endl;
      return 1;
    }
    std::size_t i = 0;
    for (i=0; i<item; ++i)
    {
      stream.put('\0');
    } //for
    if (!stream.good())
    {
      std::cout << "Error: Could not write data to temporary file!" << std::endl;
      stream.close();
      libthoro::filesystem::file::remove(fileName);
      return 1;
    }
    stream.close();

    const std::time_t currentTime = std::time(nullptr);

    int64_t s = -2;
    std::time_t mTime = -2;
    //check file size and modification time
    if (!libthoro::filesystem::file::getSizeAndModificationTime(fileName, s, mTime))
    {
      std::cout << "Error: Could not get size and modification time of temporary file!" << std::endl;
      libthoro::filesystem::file::remove(fileName);
      return 1;
    }

    //check size
    if (s != item)
    {
      std::cout << "Error: File size of " << fileName << " should be " << item
                << " bytes, but getSizeAndModificationTime() returns " << s
                << " instead!" << std::endl;
      libthoro::filesystem::file::remove(fileName);
      return 1;
    }
    //check modification time
    if (mTime != currentTime)
    {
      std::cout << "Error: Modification time of " << fileName << " should be "
                << std::asctime(std::localtime(&currentTime))
                << ", but getSizeAndModificationTime() returns "
                << std::asctime(std::localtime(&mTime)) << " instead!"
                << std::endl;
      libthoro::filesystem::file::remove(fileName);
      return 1;
    }
  } //for

  //delete file
  libthoro::filesystem::file::remove(fileName);

  //check return value for non-existent file
  int64_t s = -2;
  std::time_t mTime = -2;
  //check file size and modification time
  if (libthoro::filesystem::file::getSizeAndModificationTime(fileName, s, mTime))
  {
    std::cout << "Error: Could get size and modification time of non-existent file!"
              << " Something is clearly wrong here." << std::endl;
    return 1;
  }
  if (s != -1)
  {
    std::cout << "Error: Size of non-existent file is not -1! It is " << s
              << " instead. Something is clearly wrong here." << std::endl;
    return 1;
  }
  if (mTime != static_cast<std::time_t>(-1))
  {
    std::cout << "Error: Modification time of non-existent file is not -1! It is "
              << mTime << " instead. Something is clearly wrong here." << std::endl;
    return 1;
  }

  //Everything is OK.
  std::cout << "Test for file::getSizeAndModificationTime() passed!" << std::endl;
  return 0;
}
