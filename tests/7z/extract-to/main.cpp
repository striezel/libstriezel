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
#include "../../../archive/7z/archive.hpp"
#include "../../../filesystem/directory.hpp"
#include "../../../filesystem/file.hpp"
#include "../../../hash/sha256/sha256.hpp"
#include "../../../hash/sha256/FileSourceUtility.hpp"

void showEntry(const libstriezel::archive::entry& e)
{
  std::cout << "name: " << e.name() << std::endl
            << "    size: " << e.size() << " byte(s), directory: "
            << (e.isDirectory() ? "yes" : "no") << std::endl;
}

/* Expected parameters: 1 - directory that contains the .7z file */

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
    std::cout << "Error: First argument (.7z directory) is missing!" << std::endl;
    return 1;
  }

  const std::string sevenZipFileName = sevenZipDirectory + libstriezel::filesystem::pathDelimiter + "7z920_extra.7z";

  try
  {
    libstriezel::_7z::archive sevenZipFile(sevenZipFileName);

    //get list of all entries
    const auto entries = sevenZipFile.entries();

    //try to extract every single entry to a temporary directory
    std::string tempDirName;
    if (!libstriezel::filesystem::directory::createTemp(tempDirName))
    {
      std::cout << "Error: Could not create temporary directory for extraction!" << std::endl;
      return 1;
    }

    //try to extract every single entry one by one
    std::cout << "Extracting ..." << std::endl;
    int dotCount = 0;
    for (const auto& e : entries)
    {
      const std::string destFile = libstriezel::filesystem::slashify(tempDirName) + e.basename();
      std::cout << ".";
      ++dotCount;
      if ((dotCount % 60) == 0)
        std::cout << std::endl;
      //extraction - but only for non-directory entries
      if (!e.isDirectory())
      {
        if (!sevenZipFile.extractTo(destFile, e.name()))
        {
          std::cout << "Error: Could not extract file " << e.name()
                    << " from 7z archive!" << std::endl;
          libstriezel::filesystem::directory::remove(tempDirName);
          return 1;
        }
        //check size
        if (libstriezel::filesystem::file::getSize64(destFile) != e.size())
        {
          std::cout << "Error: File size of extracted file " << e.name()
                    << " does not match its size specified in the archive!" << std::endl;
          libstriezel::filesystem::file::remove(destFile);
          libstriezel::filesystem::directory::remove(tempDirName);
          return 1;
        }
        //delete file
        libstriezel::filesystem::file::remove(destFile);
      } //if entry is not a directory
    }
    std::cout << std::endl;

    //4th entry (index 3) should be "history.txt", a file
    const auto & e3 = entries[3];
    if ((e3.name() != "history.txt")
       || (e3.size() != 2093)
       || (e3.isDirectory()))
    {
      std::cout << "Error: Fourth entry does not match expected values!" << std::endl;
      libstriezel::filesystem::directory::remove(tempDirName);
      return 1;
    }

    const std::string destFile = libstriezel::filesystem::slashify(tempDirName) + e3.basename();
    //check one file in detail
    if (!sevenZipFile.extractTo(destFile, e3.name()))
    {
      std::cout << "Error: Could not extract file " << e3.name() << " from 7z archive!"
                << std::endl;
      libstriezel::filesystem::directory::remove(tempDirName);
      return 1;
    }

    //get SHA256 hash - easier than checking content byte by byte
    const std::string mdExpected = "31526ab62225e18172a4897f1aefbcc22d827fd5ba29d3b70248d4a3bff23be2";
    const SHA256::MessageDigest md = SHA256::computeFromFile(destFile);
    //delete file
    libstriezel::filesystem::file::remove(destFile);
    //delete temporary directory
    libstriezel::filesystem::directory::remove(tempDirName);
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
    std::cout << "Error: An exception occurred while working with the 7z file: "
              << ex.what() << std::endl;
    return 1;
  } //try-catch

  //Everything is OK.
  std::cout << "Test for libstriezel::7z::archive::extractTo() was successful." << std::endl;
  return 0;
}
