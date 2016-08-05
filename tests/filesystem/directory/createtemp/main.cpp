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
#include "../../../../filesystem/directory.hpp"

int main(int argc, char** argv)
{
  std::set<std::string> tempDirNames;

  //create 20 temporary files
  int i;
  for (i=0; i<20; ++i)
  {
    std::string dirName = "";
    if (!libstriezel::filesystem::directory::createTemp(dirName))
    {
      std::cout << "Error: Could not create temporary directory!" << std::endl;
      return 1;
    }

    //make sure that names vary
    if (tempDirNames.find(dirName) != tempDirNames.end())
    {
      std::cout << "Error: Temporary directory with the name " << dirName
                << " has already been created before!" << std::endl;
      return 1;
    }
    tempDirNames.insert(dirName);

    //remove temporary directory to keep file system clean
    if (!libstriezel::filesystem::directory::remove(dirName))
    {
      std::cout << "Error: Could not remove temporary directory " << dirName
                << "!" << std::endl;
      return 1;
    }
  } //for

  //create several temp. directories simultaneously
  tempDirNames.clear();
  for (i=0; i<50; ++i)
  {
    std::string dirName = "";
    if (!libstriezel::filesystem::directory::createTemp(dirName))
    {
      std::cout << "Error: Could not create temporary directory!" << std::endl;
      return 1;
    }

    //make sure that names vary
    if (tempDirNames.find(dirName) != tempDirNames.end())
    {
      std::cout << "Error: Temporary directory with the name " << dirName
                << " has already been created before!" << std::endl;
      return 1;
    }
    tempDirNames.insert(dirName);
  } //for

  //remove temporary directories
  for(const auto & dn : tempDirNames)
  {
    if (!libstriezel::filesystem::directory::remove(dn))
    {
      std::cout << "Error: Could not remove temporary directory " << dn
                << "!" << std::endl;
      return 1;
    }
  } //for

  //Everything is OK.
  std::cout << "Test for directory::createTemp() passed!" << std::endl;
  return 0;
}
