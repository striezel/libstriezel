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
#include "../../../archive/zip/archive.hpp"

void showEntry(const libthoro::zip::entry& e)
{
  std::cout << "idx.: " << e.index() << ", name: " << e.name() << std::endl
            << "    size: " << e.size() << " byte(s), directory: "
            << (e.isDirectory() ? "yes" : "no") << std::endl;
}

/* Expected parameters: 1 - directory that contains the zip files */

int main(int argc, char** argv)
{
  std::string zipDirectory = "";
  if (argc>1 && argv[1] != nullptr)
  {
    zipDirectory = libthoro::filesystem::unslashify(std::string(argv[1]));
    if (!libthoro::filesystem::directory::exists(zipDirectory))
    {
      std::cout << "Error: Directory " << zipDirectory << " does not exist!" << std::endl;
      return 1;
    }
  }
  else
  {
    std::cout << "Error: First argument (ZIP directory) is missing!" << std::endl;
    return 1;
  }

  const std::string zipFileName = zipDirectory + libthoro::filesystem::pathDelimiter + "zlib.zip";

  try
  {
    libthoro::zip::archive zipFile(zipFileName);

    //check number of entries via zip::numEntries()
    const int64_t entryCountExpected = 274;
    const int64_t entryCountActual   = zipFile.numEntries();
    if (entryCountActual != entryCountExpected)
    {
      std::cout << "Error: Expected to find " << entryCountExpected
                << " entries in ZIP file, but " << entryCountActual
                << " were found instead!" << std::endl;
      return 1;
    }

    //check number of entries via zip::entries().size()
    const auto entries = zipFile.entries();
    if (entries.size() != entryCountExpected)
    {
      std::cout << "Error: Expected to find " << entryCountExpected
                << " entries in ZIP file, but zip::entries() returned "
                 << entries.size() << " entries instead!" << std::endl;
      return 1;
    }

    //list all entries
    for (const auto& e : entries)
    {
      showEntry(e);
    }

    //zero-th entry should be "zlib-1.2.8/"
    const auto & entry = entries[0];
    if ((entry.name() != "zlib-1.2.8/")
       || (entry.index() != 0) || (entry.size() != 0)
       || (!entry.isDirectory()))
    {
      std::cout << "Error: First entry does not match expected values!" << std::endl;
      return 1;
    }

    //3rd entry (index 2) should be "zlib-1.2.8/CMakeLists.txt"
    const auto & e2 = entries[2];
    if ((e2.name() != "zlib-1.2.8/CMakeLists.txt")
       || (e2.index() != 2) || (e2.size() != 8098)
       || (e2.isDirectory()))
    {
      std::cout << "Error: Third entry does not match expected values!" << std::endl;
      return 1;
    }
  } //try
  catch (std::exception& ex)
  {
    std::cout << "Error: An exception occurred while working with the ZIP file: "
              << ex.what() << std::endl;
    return 1;
  } //try-catch

  //All OK.
  std::cout << "Test for libthoro::zip::archive::entries() was successful." << std::endl;
  return 0;
}
