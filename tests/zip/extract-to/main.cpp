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
#include "../../../filesystem/directory.hpp"
#include "../../../filesystem/file.hpp"
#include "../../../hash/sha256/sha256.hpp"
#include "../../../hash/sha256/FileSourceUtility.hpp"
#include "../../../archive/zip/archive.hpp"

void showEntry(const libthoro::zip::entry& e)
{
  std::cout << "idx.: " << e.index() << ", name: " << e.name() << std::endl
            << "    size: " << e.sizeUncompressed() << " byte(s), directory: "
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

    //get list of all entries
    const auto entries = zipFile.entries();

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
      //extraction
      if (!zipFile.extractTo(destFile, e.index()))
      {
        std::cout << "Error: Could not extract file " << e.name() << " from ZIP!"
                  << std::endl;
        libthoro::filesystem::directory::remove(tempDirName);
        return 1;
      }
      //check size
      if (libthoro::filesystem::file::getSize64(destFile) != e.sizeUncompressed())
      {
        std::cout << "Error: File size of extracted file " << e.name()
                  << " does not match its size specified in the archive!" << std::endl;
        libthoro::filesystem::file::remove(destFile);
        libthoro::filesystem::directory::remove(tempDirName);
        return 1;
      }
      //delete file
      libthoro::filesystem::file::remove(destFile);
    }
    std::cout << std::endl;

    //3rd entry (index 2) should be "zlib-1.2.8/CMakeLists.txt"
    const auto & e2 = entries[2];
    if ((e2.name() != "zlib-1.2.8/CMakeLists.txt")
       || (e2.index() != 2) || (e2.sizeUncompressed() != 8098)
       || (e2.isDirectory()))
    {
      std::cout << "Error: Third entry does not match expected values!" << std::endl;
      libthoro::filesystem::directory::remove(tempDirName);
      return 1;
    }

    const std::string destFile = libthoro::filesystem::slashify(tempDirName) + e2.basename();
    //check one file in detail
    if (!zipFile.extractTo(destFile, e2.index()))
    {
      std::cout << "Error: Could not extract file " << e2.name() << " from ZIP!"
                << std::endl;
      libthoro::filesystem::directory::remove(tempDirName);
      return 1;
    }

    //get SHA256 hash - easier than checking content byte by byte
    const std::string mdExpected = "ff988cbc9cbb5692400b37df2b80eb25afa6d6bf0d840a6dff5673ee7c190718";
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
    std::cout << "Error: An exception occurred while working with the ZIP file: "
              << ex.what() << std::endl;
    return 1;
  } //try-catch

  //Everything is OK.
  std::cout << "Test for libthoro::zip::archive::extractTo() was successful." << std::endl;
  return 0;
}
