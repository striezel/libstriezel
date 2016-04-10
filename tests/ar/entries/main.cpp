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
#include "../../../ar/archive.hpp"

void showEntry(const libthoro::ar::entry& e)
{
  std::cout << "name: " << e.name() << std::endl
            << "    size: " << e.size() << " byte(s), directory: "
            << (e.isDirectory() ? "yes" : "no") << std::endl;
}

/* Expected parameters: 1 - directory that contains the .iso file */

int main(int argc, char** argv)
{
  std::string arDirectory = "";
  if (argc>1 && argv[1] != nullptr)
  {
    arDirectory = libthoro::filesystem::unslashify(std::string(argv[1]));
    if (!libthoro::filesystem::directory::exists(arDirectory))
    {
      std::cout << "Error: Directory " << arDirectory << " does not exist!" << std::endl;
      return 1;
    }
  }
  else
  {
    std::cout << "Error: First argument (Ar directory) is missing!" << std::endl;
    return 1;
  }

  const std::string arFileName = arDirectory + libthoro::filesystem::pathDelimiter + "2vcard_0.5-3_all.deb";

  try
  {
    libthoro::ar::archive arFile(arFileName);

    //list all entries
    const auto entries = arFile.entries();
    for (const auto& e : entries)
    {
      showEntry(e);
    }

    //check number of entries via ar::entries().size()
    const int64_t entryCountExpected = 3;
    if (entries.size() != entryCountExpected)
    {
      std::cout << "Error: Expected to find " << entryCountExpected
                << " entries in .iso file, but ar::entries() returned "
                 << entries.size() << " entries instead!" << std::endl;
      return 1;
    }

    //zero-th entry should be "debian-binary"
    const auto & entry = entries[0];
    if ((entry.name() != "debian-binary")
       || (entry.size() != 4)
       || (entry.isDirectory()))
    {
      std::cout << "Error: First entry does not match expected values!" << std::endl;
      return 1;
    }

    //3rd entry (index 2) should be "data.tar.gz"
    const auto & e2 = entries[2];
    if ((e2.name() != "data.tar.gz")
       || (e2.size() != 13174)
       || (e2.isDirectory()))
    {
      std::cout << "Error: Third entry does not match expected values!" << std::endl;
      return 1;
    }
  } //try
  catch (std::exception& ex)
  {
    std::cout << "Error: An exception occurred while working with the .deb file: "
              << ex.what() << std::endl;
    return 1;
  } //try-catch

  //All OK.
  std::cout << "Test for libthoro::ar::archive::entries() was successful." << std::endl;
  return 0;
}
