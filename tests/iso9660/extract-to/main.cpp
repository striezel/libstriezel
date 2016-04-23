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
#include "../../../archive/iso9660/archive.hpp"
#include "../../../filesystem/directory.hpp"
#include "../../../filesystem/file.hpp"
#include "../../../hash/sha256/sha256.hpp"
#include "../../../hash/sha256/FileSourceUtility.hpp"

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
    std::cout << "Error: First argument (.iso directory) is missing!" << std::endl;
    return 1;
  }

  const std::string isoFileName = isoDirectory + libthoro::filesystem::pathDelimiter + "dsl-4.11.rc2.iso";

  try
  {
    libthoro::archive::iso9660::archive isoFile(isoFileName);

    //get list of all entries
    const auto entries = isoFile.entries();

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
      showEntry(e);
      //extraction - but only for non-directory entries
      if (!e.isDirectory())
      {
        if (!isoFile.extractTo(destFile, e.name()))
        {
          std::cout << "Error: Could not extract file " << e.name()
                    << " from ISO image!" << std::endl;
          libthoro::filesystem::directory::remove(tempDirName);
          return 1;
        }
        //check size
        if (libthoro::filesystem::file::getSize64(destFile) != e.size())
        {
          std::cout << "Error: File size of extracted file " << e.name()
                    << " does not match its size specified in the archive!" << std::endl;
          libthoro::filesystem::file::remove(destFile);
          libthoro::filesystem::directory::remove(tempDirName);
          return 1;
        }
        //delete file
        libthoro::filesystem::file::remove(destFile);
      } //if entry is not a directory
    }
    std::cout << std::endl;

    //6th entry (index 5) should be "boot/isolinux/isolinux.bin"
    const auto & e6 = entries[5];
    if ((e6.name() != "boot/isolinux/isolinux.bin")
       || (e6.isDirectory()))
    {
      std::cout << "Error: 6th entry does not match expected values!" << std::endl;
      libthoro::filesystem::directory::remove(tempDirName);
      return 1;
    }

    const std::string destFile = libthoro::filesystem::slashify(tempDirName) + e6.basename();
    //check one file in detail
    if (!isoFile.extractTo(destFile, e6.name()))
    {
      std::cout << "Error: Could not extract file " << e6.name() << " from ISO image!"
                << std::endl;
      libthoro::filesystem::directory::remove(tempDirName);
      return 1;
    }

    //get SHA256 hash - easier than checking content byte by byte
    const std::string mdExpected = "1fb16576ba6beb28991e165397ee6cd505e8dcf452881ee24c5e14172db1ca6d";
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
    std::cout << "Error: An exception occurred while working with the ISO file: "
              << ex.what() << std::endl;
    return 1;
  } //try-catch

  //Everything is OK.
  std::cout << "Test for libthoro::iso9660::archive::extractTo() was successful." << std::endl;
  return 0;
}
