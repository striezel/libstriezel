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
#include "../../../archive/xz/archive.hpp"

/* List of test files in XZ file directory:
     first = file name, second = whether it should be detected as Ar or not
*/
const std::vector<std::pair<std::string, bool> > testCases = {
  { "test_zero.dat" , false},
  { "test_01.dat" , false},
  { "test_02.dat" , false},
  { "test_03.dat" , false},
  { "test_04.dat" , false},
  { "test_05.dat" , false},
  { "test_06.dat" , true},
  { "coreutils-7.1.tar.xz" , true}
};

/* Expected parameters: 1 - directory that contains the XZ files */

int main(int argc, char** argv)
{
  std::string xzDirectory = "";
  if (argc > 1 && argv[1] != nullptr)
  {
    xzDirectory = libstriezel::filesystem::unslashify(std::string(argv[1]));
    if (!libstriezel::filesystem::directory::exists(xzDirectory))
    {
      std::cout << "Error: Directory " << xzDirectory << " does not exist!" << std::endl;
      return 1;
    }
  }
  else
  {
    std::cout << "Error: First argument (XZ directory) is missing!" << std::endl;
    return 1;
  }
  // Iterate over test cases.
  for (const auto& item : testCases)
  {
    // construct file name
    const std::string fileName = xzDirectory + libstriezel::filesystem::pathDelimiter + item.first;
    // existence check
    if (!libstriezel::filesystem::file::exists(fileName))
    {
      std::cout << "Error: File " << fileName << " does not exist!" << std::endl;
      return 1;
    }
    // check, if it is an xz archive
    const bool isXZ = libstriezel::xz::archive::isXz(fileName);
    std::cout << "isXz(" << fileName << ") = "
              << (isXZ ? "yes" : "no") << std::endl;
    // compare with expected values
    if (isXZ != item.second)
    {
      if (item.second)
        std::cout << "Error: File " << fileName << " should be an XZ archive, "
                  << "but it was NOT detected as XZ archive!" << std::endl;
      else
        std::cout << "Error: File " << fileName << " should NOT be an XZ archive, "
                  << "but it WAS detected as XZ archive!" << std::endl;
      return 1;
    }
  }
  // All OK.
  std::cout << "Tests for libstriezel::xz::archive::isXz() were successful." << std::endl;
  return 0;
}
