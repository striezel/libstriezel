/*
 -----------------------------------------------------------------------------
    This file is part of the test suite for striezel's common code library.
    Copyright (C) 2016, 2017  Dirk Stolle

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
#include "../../../filesystem/directory.hpp"
#include "../../../filesystem/file.hpp"
#include "../../../archive/rar/archive.hpp"

void showEntry(const libstriezel::archive::entryLibarchive& e)
{
  std::cout << "name: " << e.name() << std::endl
            << "    size: " << e.size() << " byte(s), directory: "
            << (e.isDirectory() ? "yes" : "no") << std::endl;
}

/* Expected parameters: 1 - directory that contains the rar file */

int main(int argc, char** argv)
{
  std::string rarDirectory = "";
  if (argc>1 && argv[1] != nullptr)
  {
    rarDirectory = libstriezel::filesystem::unslashify(std::string(argv[1]));
    if (!libstriezel::filesystem::directory::exists(rarDirectory))
    {
      std::cout << "Error: Directory " << rarDirectory << " does not exist!" << std::endl;
      return 1;
    }
  }
  else
  {
    std::cout << "Error: First argument (rar directory) is missing!" << std::endl;
    return 1;
  }

  const std::string rarFileName = rarDirectory + libstriezel::filesystem::pathDelimiter + "BEER_v112-04_Update.rar";

  try
  {
    libstriezel::rar::archive rarFile(rarFileName);

    //list all entries
    const auto entries = rarFile.entries();
    for (const auto& e : entries)
    {
      showEntry(e);
    }

    //check number of entries via rar::entries().size()
    const int64_t entryCountExpected = 25;
    if (entries.size() != entryCountExpected)
    {
      std::cout << "Error: Expected to find " << entryCountExpected
                << " entries in .rar file, but rar::entries() returned "
                 << entries.size() << " entries instead!" << std::endl;
      return 1;
    }

    //zero-th entry should be "Icons/NoM/MCA_cyro_rum.tga", a file
    const auto & entry = entries[0];
    if ((entry.name() != "Icons/NoM/MCA_cyro_rum.tga")
       || (entry.size() != 1814)
       || (entry.isDirectory()))
    {
      std::cout << "Error: First entry does not match expected values!" << std::endl;
      showEntry(entry);
      return 1;
    }

    //4th entry (index 3) should be "Icons/NoM", a directory
    const auto & e4 = entries[3];
    if ((e4.name() != "Icons/NoM")
       || (e4.size() != 0)
       || (!e4.isDirectory()))
    {
      std::cout << "Error: Fourth entry does not match expected values!" << std::endl;
      showEntry(e4);
      return 1;
    }
  } //try
  catch (std::exception& ex)
  {
    std::cout << "Error: An exception occurred while working with the .rar file: "
              << ex.what() << std::endl;
    return 1;
  } //try-catch

  //All OK.
  std::cout << "Test for libstriezel::rar::archive::entries() was successful." << std::endl;
  return 0;
}
