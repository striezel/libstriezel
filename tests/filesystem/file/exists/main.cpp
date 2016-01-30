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
#include "../../../../filesystem/directory.hpp"
#include "../../../../filesystem/file.hpp"

int main(int argc, char** argv)
{
  if ((argc!=2) or (argv==nullptr) or (argv[1]==nullptr))
  {
    std::cout << "Error: You have to pass the path to the file directory as first and only parameter!" << std::endl;
    return 1;
  }

  //base directory where the files are located
  const std::string baseDir = libthoro::filesystem::slashify(std::string(argv[1]));


  // "test cases": file-existence-pairs
  const std::vector<std::pair<std::string, bool> > files =
  {
    { baseDir+"CMakeLists.txt", true},
    { baseDir+"file_zero_bytes.txt", true},
    { baseDir+"main.cpp", true},
    { baseDir+"someOtherFile.txt", false},
    { baseDir+"notHere.ext", false},
    #if defined(_WIN32)
    { "/dev/null", false},
    { "C:\\Windows\\explorer.exe", true},
    { "C:\\Windows\\notepad.exe", true},
    #elif defined(__linux__) || defined(linux)
    { "/dev/null", true},
    { "C:\\Windows\\explorer.exe", false},
    { "C:\\Windows\\notepad.exe", false},
    #else
      #error Unknown operating system!
    #endif
    { "/this/file/should/not.exist", false},
    { "C:\\this\\file\\should\\not.exist", false}
  };

  //iterate over all test cases
  for (const auto & item : files)
  {
    if (libthoro::filesystem::file::exists(item.first) != item.second)
    {
      std::cout << "Error: Existence check for " << item.first << " should return "
                << item.second << ", but it did not!" << std::endl;
      return 1;
    }
  } //for

  //Everything is OK.
  std::cout << "Test for file::exists() passed!" << std::endl;
  return 0;
}
