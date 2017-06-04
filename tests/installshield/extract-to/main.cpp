/*
 -----------------------------------------------------------------------------
    This file is part of the test suite for striezel's common code library.
    Copyright (C) 2017  Dirk Stolle

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
#include "../../../hash/sha256/sha256.hpp"
#include "../../../hash/sha256/FileSourceUtility.hpp"
#include "../../../archive/installshield/archive.hpp"

void showEntry(const libstriezel::archive::entry& e)
{
  std::cout << "name: " << e.name() << std::endl
            << "    size: " << e.size() << " byte(s), directory: "
            << (e.isDirectory() ? "yes" : "no") << std::endl;
}

/* Expected parameters: 1 - directory that contains the InstallShield files */

int main(int argc, char** argv)
{
  std::string installShieldDirectory = "";
  if (argc>1 && argv[1] != nullptr)
  {
    installShieldDirectory = libstriezel::filesystem::unslashify(std::string(argv[1]));
    if (!libstriezel::filesystem::directory::exists(installShieldDirectory))
    {
      std::cout << "Error: Directory " << installShieldDirectory << " does not exist!" << std::endl;
      return 1;
    }
  }
  else
  {
    std::cout << "Error: First argument (InstallShield directory) is missing!" << std::endl;
    return 1;
  }

  const std::string instShieldFileName = installShieldDirectory + libstriezel::filesystem::pathDelimiter + "data1.cab";

  try
  {
    libstriezel::installshield::archive instShieldFile(instShieldFileName);

    //get list of all entries
    const auto entries = instShieldFile.entries();

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
    int filesSoFar = 0;
    for (const auto& e : entries)
    {
      if (filesSoFar > 250)
        break;
      const std::string destFile = libstriezel::filesystem::slashify(tempDirName) + e.basename();
      std::cout << ".";
      ++dotCount;
      if ((dotCount % 60) == 0)
        std::cout << std::endl;
      //extraction
      if (!instShieldFile.extractTo(destFile, e.name()))
      {
        std::cout << "Error: Could not extract file " << e.name() << " from InstallShield cabinet!"
                  << std::endl;
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
      ++filesSoFar;
    } //for
    std::cout << std::endl;

    //1st entry (index 0) should be "Essential Game Files/0011.VGA"
    const auto & e1 = entries[0];
    if ((e1.name() != "Essential Game Files/0011.VGA")
       || (e1.size() != 824)
       || (e1.isDirectory()))
    {
      std::cout << "Error: First entry does not match expected values!" << std::endl;
      libstriezel::filesystem::directory::remove(tempDirName);
      return 1;
    }

    const std::string destFile = libstriezel::filesystem::slashify(tempDirName) + e1.basename();
    //check one file in detail
    if (!instShieldFile.extractTo(destFile, e1.name()))
    {
      std::cout << "Error: Could not extract file " << e1.name() << " from InstallShield cabinet!"
                << std::endl;
      libstriezel::filesystem::directory::remove(tempDirName);
      return 1;
    }

    //get SHA256 hash - easier than checking content byte by byte
    const std::string mdExpected = "98fe102a2561d2f2907a1585a558bda98d074b1b84f69e2cb069616df68e4da3";
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
    std::cout << "Error: An exception occurred while working with the InstallShield file: "
              << ex.what() << std::endl;
    return 1;
  } //try-catch

  //Everything is OK.
  std::cout << "Test for libstriezel::installshield::archive::extractTo() was successful." << std::endl;
  return 0;
}
