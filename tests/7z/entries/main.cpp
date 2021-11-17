/*
 -----------------------------------------------------------------------------
    This file is part of the test suite for striezel's common code library.
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

#include <algorithm>
#include <iostream>
#include "../../../filesystem/directory.hpp"
#include "../../../filesystem/file.hpp"
#include "../../../archive/7z/archive.hpp"

void showEntry(const libstriezel::archive::entryLibarchive& e)
{
  std::cout << "name: " << e.name() << std::endl
            << "    size: " << e.size() << " byte(s), directory: "
            << (e.isDirectory() ? "yes" : "no") << std::endl;
}

/* Expected parameters: 1 - directory that contains the 7z file */

int main(int argc, char** argv)
{
  std::string sevenZipDirectory = "";
  if (argc>1 && argv[1] != nullptr)
  {
    sevenZipDirectory = libstriezel::filesystem::unslashify(std::string(argv[1]));
    if (!libstriezel::filesystem::directory::exists(sevenZipDirectory))
    {
      std::cout << "Error: Directory " << sevenZipDirectory << " does not exist!" << std::endl;
      return 1;
    }
  }
  else
  {
    std::cout << "Error: First argument (7z directory) is missing!" << std::endl;
    return 1;
  }

  const std::string sevenZipFileName = sevenZipDirectory + libstriezel::filesystem::pathDelimiter + "7z920_extra.7z";

  try
  {
    libstriezel::sevenZip::archive svenZipFile(sevenZipFileName);

    //list all entries
    const auto entries = svenZipFile.entries();
    for (const auto& e : entries)
    {
      showEntry(e);
    }

    //check number of entries via 7z::entries().size()
    const int64_t entryCountExpected = 24;
    if (entries.size() != entryCountExpected)
    {
      std::cout << "Error: Expected to find " << entryCountExpected
                << " entries in .7z file, but 7z::entries() returned "
                 << entries.size() << " entries instead!" << std::endl;
      return 1;
    }

    //zero-th entry should be "Installer/cr.bat"
    const auto & entry = entries[0];
    if ((entry.name() != "Installer/cr.bat")
       || (entry.size() != 207)
       || (entry.isDirectory()))
    {
      std::cout << "Error: First entry does not match expected values!" << std::endl;
      showEntry(entry);
      return 1;
    }

    //4th entry (index 3) should be "history.txt", a file
    const auto & e3 = entries[3];
    if ((e3.name() != "history.txt")
       || (e3.size() != 2093)
       || (e3.isDirectory()))
    {
      std::cout << "Error: Fourth entry does not match expected values!" << std::endl;
      showEntry(e3);
      return 1;
    }
  } //try
  catch (std::exception& ex)
  {
    std::cout << "Error: An exception occurred while working with the .7z file: "
              << ex.what() << std::endl;
    return 1;
  } //try-catch

  //All OK.
  std::cout << "Test for libstriezel::7z::archive::entries() was successful." << std::endl;
  return 0;
}
