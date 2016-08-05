/*
 -----------------------------------------------------------------------------
    This file is part of the test suite for striezel's common code library.
    Copyright (C) 2016  Dirk Stolle

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
#include "../../../archive/cab/archive.hpp"

/* List of test files in Cabinet file directory:
     first = file name, second = whether it should be detected as Cab or not
*/
const std::vector<std::pair<std::string, bool> > testCases = {
  { "test_zero.dat" , false},
  { "test_01.dat" , false},
  { "test_02.dat" , false},
  { "test_03.dat" , false},
  { "test_04.dat" , false},
  { "test_05.dat" , false},
  { "test_06.dat" , false},
  { "test_07.dat" , false},
  { "test_minimal.dat" , true},
  { "Windows8-RT-KB2999226-x64.msu" , true}
};

/* Expected parameters: 1 - directory that contains the Cabinet files */

int main(int argc, char** argv)
{
  std::string cabDirectory = "";
  if (argc>1 && argv[1] != nullptr)
  {
    cabDirectory = libstriezel::filesystem::unslashify(std::string(argv[1]));
    if (!libstriezel::filesystem::directory::exists(cabDirectory))
    {
      std::cout << "Error: Directory " << cabDirectory << " does not exist!" << std::endl;
      return 1;
    }
  }
  else
  {
    std::cout << "Error: First argument (Cabinet directory) is missing!" << std::endl;
    return 1;
  }
  //Iterate over test cases.
  for (const auto item : testCases)
  {
    //construct file name
    const std::string fileName = cabDirectory + libstriezel::filesystem::pathDelimiter + item.first;
    //existence check
    if (!libstriezel::filesystem::file::exists(fileName))
    {
      std::cout << "Error: File " << fileName << " does not exist!" << std::endl;
      return 1;
    }
    //check, if it is a Cabinet file archive
    const bool isCAB = libstriezel::cab::archive::isCab(fileName);
    std::cout << "isCab(" << fileName << ") = "
              << (isCAB ? "yes" : "no") << std::endl;
    //compare with expected values
    if (isCAB != item.second)
    {
      if (item.second)
        std::cout << "Error: File " << fileName << " should be a CAB archive, "
                  << "but it was NOT detected as CAB archive!" << std::endl;
      else
        std::cout << "Error: File " << fileName << " should NOT be a CAB archive, "
                  << "but it WAS detected as CAB archive!" << std::endl;
      return 1;
    }
  } //for
  //All OK.
  std::cout << "Tests for libstriezel::cab::archive::isCab() were successful." << std::endl;
  return 0;
}
