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
#include "../../../archive/tar/archive.hpp"

void showEntry(const libstriezel::archive::entryLibarchive& e)
{
  std::cout << "name: " << e.name() << std::endl
            << "    size: " << e.size() << " byte(s), directory: "
            << (e.isDirectory() ? "yes" : "no") << std::endl;
}

/* Expected parameters: 1 - directory that contains the tar file */

int main(int argc, char** argv)
{
  std::string tarDirectory = "";
  if (argc>1 && argv[1] != nullptr)
  {
    tarDirectory = libstriezel::filesystem::unslashify(std::string(argv[1]));
    if (!libstriezel::filesystem::directory::exists(tarDirectory))
    {
      std::cout << "Error: Directory " << tarDirectory << " does not exist!" << std::endl;
      return 1;
    }
  }
  else
  {
    std::cout << "Error: First argument (tar directory) is missing!" << std::endl;
    return 1;
  }

  const std::string tarFileName = tarDirectory + libstriezel::filesystem::pathDelimiter + "grep-2.0.tar";

  try
  {
    libstriezel::tar::archive tarFile(tarFileName);

    //list all entries
    const auto entries = tarFile.entries();
    for (const auto& e : entries)
    {
      showEntry(e);
    }

    //check number of entries via tar::entries().size()
    const int64_t entryCountExpected = 36;
    if (entries.size() != entryCountExpected)
    {
      std::cout << "Error: Expected to find " << entryCountExpected
                << " entries in .deb file, but tar::entries() returned "
                 << entries.size() << " entries instead!" << std::endl;
      return 1;
    }

    //zero-th entry should be "grep-2.0/", a directory
    const auto & entry = entries[0];
    if ((entry.name() != "grep-2.0/")
       || (entry.size() != 0)
       || (!entry.isDirectory()))
    {
      std::cout << "Error: First entry does not match expected values!" << std::endl;
      showEntry(entry);
      return 1;
    }

    //10th entry (index 9) should be "grep-2.0/COPYING", a file
    const auto & e9 = entries[9];
    if ((e9.name() != "grep-2.0/COPYING")
       || (e9.size() != 17982)
       || (e9.isDirectory()))
    {
      std::cout << "Error: Tenth entry does not match expected values!" << std::endl;
      showEntry(e9);
      return 1;
    }
  } //try
  catch (std::exception& ex)
  {
    std::cout << "Error: An exception occurred while working with the .tar file: "
              << ex.what() << std::endl;
    return 1;
  } //try-catch

  //All OK.
  std::cout << "Test for libstriezel::tar::archive::entries() was successful." << std::endl;
  return 0;
}
