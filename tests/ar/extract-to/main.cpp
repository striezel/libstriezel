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
#include "../../../archive/ar/archive.hpp"
#include "../../../filesystem/directory.hpp"
#include "../../../filesystem/file.hpp"
#include "../../../hash/sha256/sha256.hpp"
#include "../../../hash/sha256/FileSourceUtility.hpp"

void showEntry(const libstriezel::ar::entry& e)
{
  std::cout << "name: " << e.name() << std::endl
            << "    size: " << e.size() << " byte(s), directory: "
            << (e.isDirectory() ? "yes" : "no") << std::endl;
}

/* Expected parameters: 1 - directory that contains the .deb file */

int main(int argc, char** argv)
{
  std::string arDirectory = "";
  if (argc>1 && argv[1] != nullptr)
  {
    arDirectory = libstriezel::filesystem::unslashify(std::string(argv[1]));
    if (!libstriezel::filesystem::directory::exists(arDirectory))
    {
      std::cout << "Error: Directory " << arDirectory << " does not exist!" << std::endl;
      return 1;
    }
  }
  else
  {
    std::cout << "Error: First argument (Ar directory) is missing!" << std::endl;
    return 1;
  }

  const std::string arFileName = arDirectory + libstriezel::filesystem::pathDelimiter + "2vcard_0.5-3_all.deb";

  try
  {
    libstriezel::ar::archive arFile(arFileName);

    //get list of all entries
    const auto entries = arFile.entries();

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
        if (!arFile.extractTo(destFile, e.name()))
        {
          std::cout << "Error: Could not extract file " << e.name()
                    << " from Ar archive!" << std::endl;
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

    //3rd entry (index 2) should be "data.tar.gz"
    const auto & e2 = entries[2];
    if ((e2.name() != "data.tar.gz")
       || (e2.isDirectory()))
    {
      std::cout << "Error: 3rd entry does not match expected values!" << std::endl;
      libstriezel::filesystem::directory::remove(tempDirName);
      return 1;
    }

    const std::string destFile = libstriezel::filesystem::slashify(tempDirName) + e2.basename();
    //check one file in detail
    if (!arFile.extractTo(destFile, e2.name()))
    {
      std::cout << "Error: Could not extract file " << e2.name() << " from Ar archive!"
                << std::endl;
      libstriezel::filesystem::directory::remove(tempDirName);
      return 1;
    }

    //get SHA256 hash - easier than checking content byte by byte
    const std::string mdExpected = "81d801099e95d35a4aa7f999882acafec25eaa4d7c0ec352acea9b91c8d5f3eb";
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
    std::cout << "Error: An exception occurred while working with the Ar file: "
              << ex.what() << std::endl;
    return 1;
  } //try-catch

  //Everything is OK.
  std::cout << "Test for libstriezel::ar::archive::extractTo() was successful." << std::endl;
  return 0;
}
