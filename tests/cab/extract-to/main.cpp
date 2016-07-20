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
#include "../../../archive/cab/archive.hpp"
#include "../../../filesystem/directory.hpp"
#include "../../../filesystem/file.hpp"
#include "../../../hash/sha256/sha256.hpp"
#include "../../../hash/sha256/FileSourceUtility.hpp"

void showEntry(const libthoro::cab::entry& e)
{
  std::cout << "name: " << e.name() << std::endl
            << "    size: " << e.size() << " byte(s), directory: "
            << (e.isDirectory() ? "yes" : "no") << std::endl;
}

/* Expected parameters: 1 - directory that contains the .msu file */

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

    //get list of all entries
    const auto entries = cabFile.entries();

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
        if (!cabFile.extractTo(destFile, e.name()))
        {
          std::cout << "Error: Could not extract file " << e.name()
                    << " from CAB archive!" << std::endl;
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

    //2nd entry (index 1) should be "Windows8-RT-KB2999226-x64.cab"
    const auto & e2 = entries[1];
    if ((e2.name() != "Windows8-RT-KB2999226-x64.cab")
       || (e2.isDirectory()))
    {
      std::cout << "Error: 2nd entry does not match expected values!" << std::endl;
      libthoro::filesystem::directory::remove(tempDirName);
      return 1;
    }

    const std::string destFile = libthoro::filesystem::slashify(tempDirName) + e2.basename();
    //check one file in detail
    if (!cabFile.extractTo(destFile, e2.name()))
    {
      std::cout << "Error: Could not extract file " << e2.name() << " from CAB archive!"
                << std::endl;
      libthoro::filesystem::directory::remove(tempDirName);
      return 1;
    }

    //get SHA256 hash - easier than checking content byte by byte
    const std::string mdExpected = "8162c6ea0fcc75a7a9b97bb758c705123f824c72aacf6d31e09096e475b1214c";
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
    std::cout << "Error: An exception occurred while working with the CAB file: "
              << ex.what() << std::endl;
    return 1;
  } //try-catch

  //Everything is OK.
  std::cout << "Test for libthoro::cab::archive::extractTo() was successful." << std::endl;
  return 0;
}
