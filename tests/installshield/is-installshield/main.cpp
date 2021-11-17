/*
 -----------------------------------------------------------------------------
    This file is part of the test suite for striezel's common code library.
    Copyright (C) 2017, 2021  Dirk Stolle

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

#include <iostream>
#include <utility>
#include "../../../filesystem/directory.hpp"
#include "../../../filesystem/file.hpp"
#include "../../../archive/installshield/archive.hpp"

/* List of test files in files/ directory:
     first = file name, second = whether it should be detected as InstallShield or not
*/
const std::vector<std::pair<std::string, bool> > testCases = {
  { "test_00.dat" , false},
  { "test_01.dat" , false},
  { "test_02.dat" , false},
  { "test_03.dat" , false},
  { "test_04.dat" , true},
  // data1.cab from https://www.dropbox.com/s/1ng0z9kfxc7eb1e/unshield-the-feeble-files-spanish.zip?dl=1
  { "data1.cab" , true}
};

/* Expected parameters: 1 - directory that contains the InstallShield files */

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
  // Iterate over test cases.
  for (const auto& item : testCases)
  {
    // construct file name
    const std::string fileName = installShieldDirectory + libstriezel::filesystem::pathDelimiter + item.first;
    // existence check
    if (!libstriezel::filesystem::file::exists(fileName))
    {
      std::cout << "Error: File " << fileName << " does not exist!" << std::endl;
      return 1;
    }
    // check, if it is a InstallShield cabinet
    const bool isInstShield = libstriezel::installshield::archive::isInstallShield(fileName);
    std::cout << "isInstShield(" << fileName << ") = "
              << (isInstShield ? "yes" : "no") << std::endl;
    // compare with expected values
    if (isInstShield != item.second)
    {
      if (item.second)
        std::cout << "Error: File " << fileName << " should be a InstallShield file, "
                  << "but it was NOT detected as such!" << std::endl;
      else
        std::cout << "Error: File " << fileName << " should NOT be a InstallShield file, "
                  << "but it WAS detected as such!" << std::endl;
      return 1;
    }
  }
  // All OK.
  std::cout << "Tests for libstriezel::installshield::archive::isInstallShield() were successful." << std::endl;
  return 0;
}
