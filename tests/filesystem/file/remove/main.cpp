/*
 -----------------------------------------------------------------------------
    This file is part of a test suite for striezel's common code library.
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
#include <set>
#include "../../../../filesystem/file.hpp"

int main()
{
  // test scenario: create temporary file and then delete it
  std::string fileName = "";
  if (!libstriezel::filesystem::file::createTemp(fileName))
  {
    std::cout << "Error: Could not create temporary file!" << std::endl;
    return 1;
  }

  // file should exist now
  if (!libstriezel::filesystem::file::exists(fileName))
  {
    std::cout << "Error: File " << fileName << " does not exist!" << std::endl;
    return 1;
  }

  // remove file
  if (!libstriezel::filesystem::file::remove(fileName))
  {
    std::cout << "Error: Could not remove file " << fileName << "!" << std::endl;
    return 1;
  }

  // file should not exist anymore
  if (libstriezel::filesystem::file::exists(fileName))
  {
    std::cout << "Error: File " << fileName << " still exists after remove!" << std::endl;
    return 1;
  }

  // Everything is OK.
  std::cout << "Test for file::remove() passed!" << std::endl;
  return 0;
}
