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
#include <set>
#include "../../../../filesystem/file.hpp"

int main(int argc, char** argv)
{
  //test scenario: create temporary file and then delete it
  std::string fileName = "";
  if (!libthoro::filesystem::file::createTemp(fileName))
  {
    std::cout << "Error: Could not create temporary file!" << std::endl;
    return 1;
  }

  //file should exist now
  if (!libthoro::filesystem::file::exists(fileName))
  {
    std::cout << "Error: File " << fileName << " does not exist!" << std::endl;
    return 1;
  }

  //remove file
  if (!libthoro::filesystem::file::remove(fileName))
  {
    std::cout << "Error: Could not remove file " << fileName << "!" << std::endl;
    return 1;
  }

  //file should not exist anymore
  if (libthoro::filesystem::file::exists(fileName))
  {
    std::cout << "Error: File " << fileName << " still exists after remove!" << std::endl;
    return 1;
  }

  //Everything is OK.
  std::cout << "Test for file::remove() passed!" << std::endl;
  return 0;
}
