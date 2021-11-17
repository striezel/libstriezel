/*
 -----------------------------------------------------------------------------
    This file is part of the test suite for striezel's common code library.
    Copyright (C) 2016, 2021  Dirk Stolle

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
#include "../../../filesystem/directory.hpp"
#include "../../../filesystem/file.hpp"
#include "../../../archive/7z/archive.hpp"

/* List of test files in 7z file directory:
     first = file name, second = whether it should be detected as 7z or not
*/
const std::vector<std::pair<std::string, bool> > testCases = {
  { "test_zero.dat" , false},
  { "test_1.dat" , false},
  { "test_2.dat" , false},
  { "test_3.dat" , false},
  { "test_3.dat" , false},
  { "test_5.dat" , false},
  { "test_6.dat" , true},
  { "7z920_extra.7z" , true}
};

/* Expected parameters: 1 - directory that contains the 7z files */

int main(int argc, char** argv)
{
  std::string sevenZipDirectory = "";
  if (argc > 1 && argv[1] != nullptr)
  {
    sevenZipDirectory = libstriezel::filesystem::unslashify(std::string(argv[1]));
    if (!libstriezel::filesystem::directory::exists(sevenZipDirectory))
    {
      std::cout << "Error: Directory " << sevenZipDirectory << " does not exist!" << std::endl;
      return 1;
    }
  }
  else
  {
    std::cout << "Error: First argument (7z directory) is missing!" << std::endl;
    return 1;
  }
  // Iterate over test cases.
  for (const auto& item : testCases)
  {
    // construct file name
    const std::string fileName = sevenZipDirectory + libstriezel::filesystem::pathDelimiter + item.first;
    // existence check
    if (!libstriezel::filesystem::file::exists(fileName))
    {
      std::cout << "Error: File " << fileName << " does not exist!" << std::endl;
      return 1;
    }
    // check, if it is a 7-Zip archive
    const bool is7z = libstriezel::sevenZip::archive::is7z(fileName);
    std::cout << "is7z(" << fileName << ") = "
              << (is7z ? "yes" : "no") << std::endl;
    // compare with expected values
    if (is7z != item.second)
    {
      if (item.second)
        std::cout << "Error: File " << fileName << " should be a 7-Zip archive, "
                  << "but it was NOT detected as 7-Zip archive!" << std::endl;
      else
        std::cout << "Error: File " << fileName << " should NOT be a 7-Zip archive, "
                  << "but it WAS detected as 7-Zip archive!" << std::endl;
      return 1;
    }
  }
  // All OK.
  std::cout << "Tests for libstriezel::7z::archive::is7z() were successful." << std::endl;
  return 0;
}
