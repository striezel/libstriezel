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

#include <chrono>
#include <ctime>
#include <iostream>
#include <vector>
#include "../../../../filesystem/file.hpp"

// test for libthoro::filesystem::file::setModificationTime()

const std::vector<std::chrono::seconds> testDifferences =
        {
            std::chrono::seconds(-365*86400), // - one year
            std::chrono::seconds(-30*86400),  // - 30 days
            std::chrono::seconds(-86400),     // - 1 day
            std::chrono::seconds(-12*3600),   // - 12 hours
            std::chrono::seconds(-3600),      // - 1 hour
            std::chrono::seconds(-60),        // - 1 minute
            std::chrono::seconds(-1),         // - 1 second
            std::chrono::seconds(0),
            std::chrono::seconds(1),          // + 1 second
            std::chrono::seconds(60),         // + 1 minute
            std::chrono::seconds(3600),       // + 1 hour
            std::chrono::seconds(12*3600),    // + 12 hours
            std::chrono::seconds(86400),      // + 1 day
            std::chrono::seconds(30*86400),   // + 30 days
            std::chrono::seconds(365*86400)   // + one year
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

  const std::time_t currentTime = std::time(nullptr);

  int64_t dummySize = -2;
  std::time_t mTime = -2;
  //get modification time
  if (!libthoro::filesystem::file::getSizeAndModificationTime(fileName, dummySize, mTime))
  {
    std::cout << "Error: Could not get size and modification time of temporary file!" << std::endl;
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

  //current time as time point in std::chrono
  const auto currentTimePoint = std::chrono::system_clock::from_time_t(currentTime);

  for (const auto duration : testDifferences)
  {
    // Add duration to current time and convert to std::time_t.
    const std::time_t newModTime = std::chrono::system_clock::to_time_t(currentTimePoint + duration);

    //set new mtime
    if (!libthoro::filesystem::file::setModificationTime(fileName, newModTime))
    {
      std::cout << "Error: Could not set new modification time of temporary file!" << std::endl;
      libthoro::filesystem::file::remove(fileName);
      return 1;
    }

    //get new mtime
    if (!libthoro::filesystem::file::getSizeAndModificationTime(fileName, dummySize, mTime))
    {
      std::cout << "Error: Could not get modification time of temporary file!" << std::endl;
      libthoro::filesystem::file::remove(fileName);
      return 1;
    }

    //check modification time
    if (mTime != newModTime)
    {
      std::cout << "Error: Modification time of " << fileName << " should be "
                << std::asctime(std::localtime(&newModTime))
                << ", but getSizeAndModificationTime() returns "
                << std::asctime(std::localtime(&mTime)) << " instead!"
                << std::endl;
      libthoro::filesystem::file::remove(fileName);
      return 1;
    }

    //check time difference
    const auto modTimePoint = std::chrono::system_clock::from_time_t(mTime);
    if (currentTimePoint + duration != modTimePoint)
    {
      std::cout << "Error: Modification time difference does not match the expected duration!"
                << " Expected are " << duration.count() << " seconds. "
                << "However, the current difference is "
                << std::chrono::duration_cast<std::chrono::seconds>(currentTimePoint-modTimePoint).count()
                << " seconds." << std::endl;
      libthoro::filesystem::file::remove(fileName);
      return 1;
    }
  } //for

  //delete file
  libthoro::filesystem::file::remove(fileName);

  //Everything is OK.
  std::cout << "Test for file::setModificationTime() passed!" << std::endl;
  return 0;
}
