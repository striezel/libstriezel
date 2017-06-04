/*
 -----------------------------------------------------------------------------
    This file is part of the test suite for striezel's common code library.
    Copyright (C) 2016, 2017  Dirk Stolle

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
#include "../../../archive/rar/archive.hpp"
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

/* Expected parameters: 1 - directory that contains the .rar file */

int main(int argc, char** argv)
{
  std::string rarDirectory = "";
  if (argc>1 && argv[1] != nullptr)
  {
    rarDirectory = libstriezel::filesystem::unslashify(std::string(argv[1]));
    if (!libstriezel::filesystem::directory::exists(rarDirectory))
    {
      std::cout << "Error: Directory " << rarDirectory << " does not exist!" << std::endl;
      return 1;
    }
  }
  else
  {
    std::cout << "Error: First argument (rar directory) is missing!" << std::endl;
    return 1;
  }

  const std::string rarFileName = rarDirectory + libstriezel::filesystem::pathDelimiter + "BEER_v112-04_Update.rar";

  try
  {
    libstriezel::rar::archive rarFile(rarFileName);

    //get list of all entries
    const auto entries = rarFile.entries();

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
    unsigned int successfulExtractions = 0;
    unsigned int failedExtractions = 0;
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
        if (!rarFile.extractTo(destFile, e.name()))
        {
          std::cout << "Error: Could not extract file " << e.name()
                    << " from rar archive to " << destFile << "!" << std::endl;
          libstriezel::filesystem::directory::remove(tempDirName);
          std::cout << "However, since libarchive only has limited support for"
                    << " the RAR format due to some proprietary restrictions, "
                    << "we do not treat this as error and just go on here."
                    << std::endl;
          ++failedExtractions;
          continue;
        }
        ++successfulExtractions;
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
    } //for
    std::cout << std::endl;
    const auto totalExtractions = successfulExtractions + failedExtractions;
    const double successRate = successfulExtractions * 100 / static_cast<double>(totalExtractions);
    const double failureRate = failedExtractions * 100 / static_cast<double>(totalExtractions);
    std::cout << "Successfully extracted files: " << successfulExtractions
              << " (" << successRate << " %)" << std::endl
              << "Failed extractions: " << failedExtractions << " ("
              << failureRate << " %)" << std::endl;

    //1st entry (index 0) should be "Icons/NoM/MCA_cyro_rum.tga", a file
    const auto & e = entries[0];
    if ((e.name() != "Icons/NoM/MCA_cyro_rum.tga")
       || (e.size() != 1814)
       || (e.isDirectory()))
    {
      std::cout << "Error: First entry does not match expected values!" << std::endl;
      libstriezel::filesystem::directory::remove(tempDirName);
      return 1;
    }

    const std::string destFile = libstriezel::filesystem::slashify(tempDirName) + e.basename();
    //check one file in detail
    if (!rarFile.extractTo(destFile, e.name()))
    {
      std::cout << "Error: Could not extract file " << e.name() << " from rar archive!"
                << std::endl;
      libstriezel::filesystem::directory::remove(tempDirName);
      return 1;
    }

    //get SHA256 hash - easier than checking content byte by byte
    const std::string mdExpected = "a625d60db613119b6e0e94213c2b84ae633226e61a6722d51991d93b6fbc9d45";
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
    std::cout << "Error: An exception occurred while working with the rar file: "
              << ex.what() << std::endl;
    return 1;
  } //try-catch

  //Everything is OK.
  std::cout << "Test for libstriezel::rar::archive::extractTo() was successful." << std::endl;
  return 0;
}
