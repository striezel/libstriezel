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

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include "../../../filesystem/directory.hpp"
#include "../../../filesystem/file.hpp"
#include "../../../archive/xz/archive.hpp"

void showEntry(const libthoro::archive::entryLibarchive& e)
{
  std::cout << "name: " << e.name() << std::endl
            << "    size: " << e.size() << " byte(s), directory: "
            << (e.isDirectory() ? "yes" : "no") << std::endl;
}

/* Expected parameters: 1 - directory that contains the .xz file */

int main(int argc, char** argv)
{
  std::string xzDirectory = "";
  if (argc>1 && argv[1] != nullptr)
  {
    xzDirectory = libthoro::filesystem::unslashify(std::string(argv[1]));
    if (!libthoro::filesystem::directory::exists(xzDirectory))
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

  const std::string xzFileName = xzDirectory + libthoro::filesystem::pathDelimiter + "coreutils-7.1.tar.xz";

  try
  {
    libthoro::xz::archive xzFile(xzFileName);

    //list all entries
    const auto entries = xzFile.entries();
    for (const auto& e : entries)
    {
      showEntry(e);
    }

    //check number of entries via xz::entries().size()
    const int64_t entryCountExpected = 1;
    if (entries.size() != entryCountExpected)
    {
      std::cout << "Error: Expected to find " << entryCountExpected
                << " entries in .xz file, but xz::entries() returned "
                 << entries.size() << " entries instead!" << std::endl;
      return 1;
    }

    //zero-th entry should be "coreutils-7.1.tar"
    const auto & entry = entries[0];
    if ((entry.name() != "coreutils-7.1.tar")
       || (entry.size() != -1) //size is currently unknown
       || (entry.isDirectory()))
    {
      std::cout << "Error: First entry does not match expected values!" << std::endl;
      return 1;
    }
  } //try
  catch (std::exception& ex)
  {
    std::cout << "Error: An exception occurred while working with the .xz file: "
              << ex.what() << std::endl;
    return 1;
  } //try-catch

  //All OK.
  std::cout << "Test for libthoro::xz::archive::entries() was successful." << std::endl;
  return 0;
}
