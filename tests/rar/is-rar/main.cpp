/*
 -----------------------------------------------------------------------------
    This file is part of the test suite for striezel's common code library.
    Copyright (C) 2016, 2017, 2021  Dirk Stolle

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
#include "../../../archive/rar/archive.hpp"

/* List of test files in rar file directory:
     first = file name, second = whether it should be detected as rar or not
*/
const std::vector<std::pair<std::string, bool> > testCases = {
  { "test_zero.dat" , false},
  { "test_4.dat" , false},
  { "test_7.dat" , true},
  { "BEER_v112-04_Update.rar" , true}
};

/* Expected parameters: 1 - directory that contains the rar files */

int main(int argc, char** argv)
{
  std::string rarDirectory = "";
  if (argc>1 && argv[1] != nullptr)
  {
    rarDirectory = libstriezel::filesystem::unslashify(std::string(argv[1]));
    if (!libstriezel::filesystem::directory::exists(rarDirectory))
    {
      std::cout << "Error: Directory " << rarDirectory << " does not exist!" << std::endl;
      return 1;
    }
  }
  else
  {
    std::cout << "Error: First argument (rar directory) is missing!" << std::endl;
    return 1;
  }
  // Iterate over test cases.
  for (const auto& item : testCases)
  {
    // construct file name
    const std::string fileName = rarDirectory + libstriezel::filesystem::pathDelimiter + item.first;
    // existence check
    if (!libstriezel::filesystem::file::exists(fileName))
    {
      std::cout << "Error: File " << fileName << " does not exist!" << std::endl;
      return 1;
    }
    // check, if it is a rar archive
    const bool isRar = libstriezel::rar::archive::isRar(fileName);
    std::cout << "isRar(" << fileName << ") = "
              << (isRar ? "yes" : "no") << std::endl;
    // compare with expected values
    if (isRar != item.second)
    {
      if (item.second)
        std::cout << "Error: File " << fileName << " should be a rar archive, "
                  << "but it was NOT detected as rar archive!" << std::endl;
      else
        std::cout << "Error: File " << fileName << " should NOT be a rar archive, "
                  << "but it WAS detected as rar archive!" << std::endl;
      return 1;
    }
  }
  // All OK.
  std::cout << "Tests for libstriezel::rar::archive::isRar() were successful." << std::endl;
  return 0;
}
