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
#include "../../../archive/installshield/archive.hpp"

void showEntry(const libstriezel::archive::entry& e)
{
  std::cout << "name: " << e.name() << std::endl
            << "    size: " << e.size() << " byte(s), directory: "
            << (e.isDirectory() ? "yes" : "no") << std::endl;
}

/* Expected parameters: 1 - directory that contains the files */

int main(int argc, char** argv)
{
  std::string installShieldDirectory = "";
  if (argc > 1 && argv[1] != nullptr)
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

    //check number of entries via instShield::numEntries()
    const int64_t entryCountExpected = 319;
    const int64_t entryCountActual   = instShieldFile.numEntries();
    if (entryCountActual != entryCountExpected)
    {
      std::cout << "Error: Expected to find " << entryCountExpected
                << " entries in InstallShield file, but " << entryCountActual
                << " were found instead!" << std::endl;
      return 1;
    }

    //check number of entries via installshield::entries().size()
    const auto entries = instShieldFile.entries();
    if (entries.size() != entryCountExpected)
    {
      std::cout << "Error: Expected to find " << entryCountExpected
                << " entries in InstallShield file, but archive::entries() returned "
                 << entries.size() << " entries instead!" << std::endl;
      return 1;
    }

    //list all entries
    for (const auto& e : entries)
    {
      showEntry(e);
    }

    //zero-th entry should be "Essential Game Files/0011.VGA"
    const auto & entry = entries[0];
    if ((entry.name() != "Essential Game Files/0011.VGA")
       || (entry.size() != 824)
       || (entry.isDirectory()))
    {
      std::cout << "Error: First entry does not match expected values!" << std::endl;
      return 1;
    }

    //2nd entry (index 1) should be "244880  Essential Game Files\0012.VGA"
    const auto & e1 = entries[1];
    if ((e1.name() != "Essential Game Files/0012.VGA")
       || (e1.size() != 244880)
       || (e1.isDirectory()))
    {
      std::cout << "Error: Second entry does not match expected values!" << std::endl;
      return 1;
    }
  } //try
  catch (std::exception& ex)
  {
    std::cout << "Error: An exception occurred while working with the InstallShield file: "
              << ex.what() << std::endl;
    return 1;
  } //try-catch

  //All OK.
  std::cout << "Test for libstriezel::installshield::archive::entries() was successful." << std::endl;
  return 0;
}
