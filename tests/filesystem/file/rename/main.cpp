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

int main(int argc, char** argv)
{
  //test scenario: create temporary file and then rename/move it
  std::string originalFileName = "";
  if (!libstriezel::filesystem::file::createTemp(originalFileName))
  {
    std::cout << "Error: Could not create temporary file!" << std::endl;
    return 1;
  }

  const std::string newFileName = originalFileName + "_new";
  //file shall not exist yet
  if (libstriezel::filesystem::file::exists(newFileName))
  {
    std::cout << "Error: File " << newFileName << " already exists!" << std::endl;
    return 1;
  }

  //move file
  if (!libstriezel::filesystem::file::rename(originalFileName, newFileName))
  {
    std::cout << "Error: Could not rename file " << originalFileName << " to "
              << newFileName << "!" << std::endl;
    return 1;
  }

  //old file should not exist anymore ...
  if (libstriezel::filesystem::file::exists(originalFileName))
  {
    std::cout << "Error: File " << originalFileName << " still exists after rename!" << std::endl;
    return 1;
  }

  //... but "new" file has to exist
  if (!libstriezel::filesystem::file::exists(newFileName))
  {
    std::cout << "Error: \"New\" file " << newFileName << " does not exists after rename!" << std::endl;
    return 1;
  }

  //remove file

  if (!libstriezel::filesystem::file::remove(newFileName))
  {
    std::cout << "Error: Could not remove file " << newFileName << "!" << std::endl;
    return 1;
  }

  //Everything is OK.
  std::cout << "Test for file::rename() passed!" << std::endl;
  return 0;
}
