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
#include "../../../archive/cab/archive.hpp"

void showEntry(const libthoro::cab::entry& e)
{
  std::cout << "name: " << e.name() << std::endl
            << "    size: " << e.size() << " byte(s), directory: "
            << (e.isDirectory() ? "yes" : "no") << std::endl;
}

/* Expected parameters: 1 - directory that contains the CAB file */

int main(int argc, char** argv)
{
  std::string cabDirectory = "";
  if (argc>1 && argv[1] != nullptr)
  {
    cabDirectory = libthoro::filesystem::unslashify(std::string(argv[1]));
    if (!libthoro::filesystem::directory::exists(cabDirectory))
    {
      std::cout << "Error: Directory " << cabDirectory << " does not exist!" << std::endl;
      return 1;
    }
  }
  else
  {
    std::cout << "Error: First argument (CAB directory) is missing!" << std::endl;
    return 1;
  }

  const std::string cabFileName = cabDirectory + libthoro::filesystem::pathDelimiter + "Windows8-RT-KB2999226-x64.msu";

  try
  {
    libthoro::cab::archive cabFile(cabFileName);

    //list all entries
    const auto entries = cabFile.entries();
    for (const auto& e : entries)
    {
      showEntry(e);
    }

    //check number of entries via cab::entries().size()
    const int64_t entryCountExpected = 4;
    if (entries.size() != entryCountExpected)
    {
      std::cout << "Error: Expected to find " << entryCountExpected
                << " entries in .msu file, but cab::entries() returned "
                 << entries.size() << " entries instead!" << std::endl;
      return 1;
    }

    //zero-th entry should be "WSUSSCAN.cab"
    const auto & entry = entries[0];
    if ((entry.name() != "WSUSSCAN.cab")
       || (entry.size() != 173622)
       || (entry.isDirectory()))
    {
      std::cout << "Error: First entry does not match expected values!" << std::endl;
      return 1;
    }

    //2nd entry (index 1) should be "Windows8-RT-KB2999226-x64.cab"
    // (Yes, it's a .cab inside a .cab file.)
    const auto & e2 = entries[1];
    if ((e2.name() != "Windows8-RT-KB2999226-x64.cab")
       || (e2.size() != 1184921)
       || (e2.isDirectory()))
    {
      std::cout << "Error: Second entry does not match expected values!" << std::endl;
      return 1;
    }
  } //try
  catch (std::exception& ex)
  {
    std::cout << "Error: An exception occurred while working with the .msu file: "
              << ex.what() << std::endl;
    return 1;
  } //try-catch

  //All OK.
  std::cout << "Test for libthoro::cab::archive::entries() was successful." << std::endl;
  return 0;
}
