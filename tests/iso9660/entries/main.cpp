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
#include "../../../filesystem/directory.hpp"
#include "../../../filesystem/file.hpp"
#include "../../../archive/iso9660/archive.hpp"

void showEntry(const libthoro::archive::entryLibarchive& e)
{
  std::cout << "name: " << e.name() << std::endl
            << "    size: " << e.size() << " byte(s), directory: "
            << (e.isDirectory() ? "yes" : "no") << std::endl;
}

/* Expected parameters: 1 - directory that contains the .iso file */

int main(int argc, char** argv)
{
  std::string isoDirectory = "";
  if (argc>1 && argv[1] != nullptr)
  {
    isoDirectory = libthoro::filesystem::unslashify(std::string(argv[1]));
    if (!libthoro::filesystem::directory::exists(isoDirectory))
    {
      std::cout << "Error: Directory " << isoDirectory << " does not exist!" << std::endl;
      return 1;
    }
  }
  else
  {
    std::cout << "Error: First argument (ISO directory) is missing!" << std::endl;
    return 1;
  }

  const std::string isoFileName = isoDirectory + libthoro::filesystem::pathDelimiter + "dsl-4.11.rc2.iso";

  try
  {
    libthoro::archive::iso9660::archive isoFile(isoFileName);

    //list all entries
    const auto entries = isoFile.entries();
    for (const auto& e : entries)
    {
      showEntry(e);
    }

    //check number of entries via zip::entries().size()
    const int64_t entryCountExpected = 16; // 15 entries + one root directory
    if (entries.size() != entryCountExpected)
    {
      std::cout << "Error: Expected to find " << entryCountExpected
                << " entries in .iso file, but iso9660::entries() returned "
                 << entries.size() << " entries instead!" << std::endl;
      return 1;
    }

    //second entry should be "boot"
    const auto & entry = entries[2];
    if ((entry.name() != "boot")
       || (entry.size() != 2048)
       || (!entry.isDirectory()))
    {
      std::cout << "Error: Second entry does not match expected values!" << std::endl;
      return 1;
    }

    //8th entry (index 9) should be "boot/isolinux/boot.msg"
    const auto & e2 = entries[8];
    if ((e2.name() != "boot/isolinux/boot.msg")
       || (e2.size() != 123)
       || (e2.isDirectory()))
    {
      std::cout << "Error: 8th entry does not match expected values!" << std::endl;
      return 1;
    }
  } //try
  catch (std::exception& ex)
  {
    std::cout << "Error: An exception occurred while working with the .iso file: "
              << ex.what() << std::endl;
    return 1;
  } //try-catch

  //All OK.
  std::cout << "Test for libthoro::archive::iso9660::archive::entries() was successful." << std::endl;
  return 0;
}
