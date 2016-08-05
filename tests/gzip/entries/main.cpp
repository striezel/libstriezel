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
#include "../../../archive/gzip/archive.hpp"

void showEntry(const libstriezel::gzip::entry& e)
{
  std::cout << "name: " << e.name() << std::endl
            << "    size: " << e.size() << " byte(s), directory: "
            << (e.isDirectory() ? "yes" : "no") << std::endl;
}

/* Expected parameters: 1 - directory that contains the .gz file */

int main(int argc, char** argv)
{
  std::string gzDirectory = "";
  if (argc>1 && argv[1] != nullptr)
  {
    gzDirectory = libstriezel::filesystem::unslashify(std::string(argv[1]));
    if (!libstriezel::filesystem::directory::exists(gzDirectory))
    {
      std::cout << "Error: Directory " << gzDirectory << " does not exist!" << std::endl;
      return 1;
    }
  }
  else
  {
    std::cout << "Error: First argument (gzip directory) is missing!" << std::endl;
    return 1;
  }

  const std::string gzFileName = gzDirectory + libstriezel::filesystem::pathDelimiter + "zlib.tar.gz";

  try
  {
    libstriezel::gzip::archive gzipFile(gzFileName);

    //list all entries
    const auto entries = gzipFile.entries();
    for (const auto& e : entries)
    {
      showEntry(e);
    }

    //check number of entries via gzip::entries().size()
    const int64_t entryCountExpected = 1;
    if (entries.size() != entryCountExpected)
    {
      std::cout << "Error: Expected to find " << entryCountExpected
                << " entries in .gz file, but gzip::archive::entries() returned "
                << entries.size() << " entries instead!" << std::endl;
      return 1;
    }

    //zero-th entry should be "zlib.tar"
    const auto & entry = entries[0];
    if ((entry.name() != "zlib.tar")
       || (entry.size() != 2795520)
       || (entry.isDirectory()))
    {
      std::cout << "Error: First entry does not match expected values!" << std::endl;
      return 1;
    }
  } //try
  catch (std::exception& ex)
  {
    std::cout << "Error: An exception occurred while working with the .gz file: "
              << ex.what() << std::endl;
    return 1;
  } //try-catch

  //All OK.
  std::cout << "Test for libstriezel::gzip::archive::entries() was successful." << std::endl;
  return 0;
}
