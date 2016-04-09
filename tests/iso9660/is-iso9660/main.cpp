/*
 -----------------------------------------------------------------------------
    This file is part of the test suite for Thoronador's common code library.
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
#include <string>
#include <vector>
#include <utility>
#include "../../../filesystem/directory.hpp"
#include "../../../filesystem/file.hpp"
#include "../../../iso9660/archive.hpp"

/* List of test files in ISO9660 file directory:
     first = file name, second = whether it should be detected as ISO9660 or not
*/
const std::vector<std::pair<std::string, bool> > testCases = {
  { "test_zero.dat" , false},
  { "test_16k.dat" , false},
  { "test_32k.dat" , false},
  { "test_minimal-iso.dat" , true},
  { "dsl-4.11.rc2.iso" , true}
};

/* Expected parameters: 1 - directory that contains the .iso files */

int main(int argc, char** argv)
{
  std::string isoDirectory = "";
  if (argc>1 && argv[1] != nullptr)
  {
    isoDirectory = libthoro::filesystem::unslashify(std::string(argv[1]));
    if (!libthoro::filesystem::directory::exists(isoDirectory))
    {
      std::cout << "Error: Directory " << isoDirectory << " does not exist!" << std::endl;
      return 1;
    }
  }
  else
  {
    std::cout << "Error: First argument (ISO directory) is missing!" << std::endl;
    return 1;
  }
  //Iterate over test cases.
  for (const auto item : testCases)
  {
    //construct file name
    const std::string fileName = isoDirectory + libthoro::filesystem::pathDelimiter + item.first;
    //existence check
    if (!libthoro::filesystem::file::exists(fileName))
    {
      std::cout << "Error: File " << fileName << " does not exist!" << std::endl;
      return 1;
    }
    //check, if it is an ISO9660 image
    const bool isISO = libthoro::iso9660::archive::isISO9660(fileName);
    std::cout << "isISO9660(" << fileName << ") = "
              << (isISO ? "yes" : "no") << std::endl;
    //compare with expected values
    if (isISO != item.second)
    {
      if (item.second)
        std::cout << "Error: File " << fileName << " should be an ISO9660 image, "
                  << "but it was NOT detected as ISO9660 image!" << std::endl;
      else
        std::cout << "Error: File " << fileName << " should NOT be an ISO9660 image, "
                  << "but it WAS detected as ISO9660 image!" << std::endl;
      return 1;
    }
  } //for
  //All OK.
  std::cout << "Tests for libthoro::iso9660::archive::isISO9660() were successful." << std::endl;
  return 0;
}
