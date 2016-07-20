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
#include "../../../archive/xz/archive.hpp"
#include "../../../filesystem/directory.hpp"
#include "../../../filesystem/file.hpp"
#include "../../../hash/sha256/sha256.hpp"
#include "../../../hash/sha256/FileSourceUtility.hpp"

void showEntry(const libthoro::archive::entry& e)
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

    //get list of all entries
    const auto entries = xzFile.entries();

    //try to extract every single entry to a temporary directory
    std::string tempDirName;
    if (!libthoro::filesystem::directory::createTemp(tempDirName))
    {
      std::cout << "Error: Could not create temporary directory for extraction!" << std::endl;
      return 1;
    }

    //try to extract every single entry one by one
    std::cout << "Extracting ..." << std::endl;
    int dotCount = 0;
    for (const auto& e : entries)
    {
      const std::string destFile = libthoro::filesystem::slashify(tempDirName) + e.basename();
      std::cout << ".";
      ++dotCount;
      if ((dotCount % 60) == 0)
        std::cout << std::endl;
      //extraction - but only for non-directory entries
      if (!e.isDirectory())
      {
        if (!xzFile.extractTo(destFile, e.name()))
        {
          std::cout << "Error: Could not extract file " << e.name()
                    << " from XZ archive!" << std::endl;
          libthoro::filesystem::directory::remove(tempDirName);
          return 1;
        }
        /* Size check is disabled, because size will always be -1.
        //check size
        if (libthoro::filesystem::file::getSize64(destFile) != e.size())
        {
          std::cout << "Error: File size of extracted file " << e.name()
                    << " does not match its size specified in the archive!" << std::endl;
          libthoro::filesystem::file::remove(destFile);
          libthoro::filesystem::directory::remove(tempDirName);
          return 1;
        }
        */
        //delete file
        libthoro::filesystem::file::remove(destFile);
      } //if entry is not a directory
    }
    std::cout << std::endl;

    //1st entry (index 0) should be "data.tar.gz"
    const auto & entry = entries[0];
    if ((entry.name() != "coreutils-7.1.tar")
       || (entry.isDirectory()))
    {
      std::cout << "Error: 1st entry does not match expected values!" << std::endl;
      libthoro::filesystem::directory::remove(tempDirName);
      return 1;
    }

    const std::string destFile = libthoro::filesystem::slashify(tempDirName) + entry.basename();
    //check one file in detail
    if (!xzFile.extractTo(destFile, entry.name()))
    {
      std::cout << "Error: Could not extract file " << entry.name() << " from XZ archive!"
                << std::endl;
      libthoro::filesystem::directory::remove(tempDirName);
      return 1;
    }

    //get SHA256 hash - easier than checking content byte by byte
    const std::string mdExpected = "b272f57fb88bb0bc7f37aaf4068aebb6bb277fa9f243ca1f53af5f1419c463a1";
    const SHA256::MessageDigest md = SHA256::computeFromFile(destFile);
    //delete file
    libthoro::filesystem::file::remove(destFile);
    //delete temporary directory
    libthoro::filesystem::directory::remove(tempDirName);
    //compare hashes
    if (mdExpected != md.toHexString())
    {
      std::cout << "Error: Hash of extracted file is wrong!" << std::endl
                << "Hash is:  " << md.toHexString() << std::endl
                << "Expected: " << mdExpected << std::endl;
      return 1;
    }
  } //try
  catch (std::exception& ex)
  {
    std::cout << "Error: An exception occurred while working with the XZ file: "
              << ex.what() << std::endl;
    return 1;
  } //try-catch

  //Everything is OK.
  std::cout << "Test for libthoro::xz::archive::extractTo() was successful." << std::endl;
  return 0;
}
