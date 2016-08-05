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
  std::set<std::string> tempFileNames;

  //create 20 temporary files
  int i;
  for (i=0; i<20; ++i)
  {
    std::string fileName = "";
    if (!libstriezel::filesystem::file::createTemp(fileName))
    {
      std::cout << "Error: Could not create temporary file!" << std::endl;
      return 1;
    }

    //make sure that names vary
    if (tempFileNames.find(fileName) != tempFileNames.end())
    {
      std::cout << "Error: Temporary file with the name " << fileName
                << " has already been created before!" << std::endl;
      return 1;
    }
    tempFileNames.insert(fileName);

    //remove temporary file to keep file system clean
    if (!libstriezel::filesystem::file::remove(fileName))
    {
      std::cout << "Error: Could not remove temporary file " << fileName
                << "!" << std::endl;
      return 1;
    }
  } //for

  //create several temp. files simultaneously
  tempFileNames.clear();
  for (i=0; i<50; ++i)
  {
    std::string fileName = "";
    if (!libstriezel::filesystem::file::createTemp(fileName))
    {
      std::cout << "Error: Could not create temporary file!" << std::endl;
      return 1;
    }

    //make sure that names vary
    if (tempFileNames.find(fileName) != tempFileNames.end())
    {
      std::cout << "Error: Temporary file with the name " << fileName
                << " has already been created before!" << std::endl;
      return 1;
    }
    tempFileNames.insert(fileName);
  } //for

  //remove temporary files
  for(const auto & fn : tempFileNames)
  {
    if (!libstriezel::filesystem::file::remove(fn))
    {
      std::cout << "Error: Could not remove temporary file " << fn
                << "!" << std::endl;
      return 1;
    }
  } //for

  //Everything is OK.
  std::cout << "Test for file::createTemp() passed!" << std::endl;
  return 0;
}
