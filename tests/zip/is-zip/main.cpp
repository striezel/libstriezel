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

#include <iostream>
#include <utility>
#include "../../../filesystem/directory.hpp"
#include "../../../filesystem/file.hpp"
#include "../../../archive/zip/archive.hpp"

/* List of test files in ZIP directory:
     first = file name, second = whether it should be detected as ZIP or not
*/
const std::vector<std::pair<std::string, bool> > testCases = {
  { "test_01.dat" , false},
  { "test_02.dat" , false},
  { "test_03.dat" , false},
  { "test_04.dat" , true},
  // https://archive.apache.org/dist/logging/log4j/1.2.17/log4j-1.2.17.zip
  { "log4j-1.2.17.zip" , true},
  //Java Archive from file above
  { "apache-log4j-1.2.17/log4j-1.2.17.jar" , true},
  // http://calibre-ebook.com/downloads/demos/demo.docx
  { "demo.docx" , true}
};

/* Expected parameters: 1 - directory that contains the zip files */

int main(int argc, char** argv)
{
  std::string zipDirectory = "";
  if (argc>1 && argv[1] != nullptr)
  {
    zipDirectory = libstriezel::filesystem::unslashify(std::string(argv[1]));
    if (!libstriezel::filesystem::directory::exists(zipDirectory))
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
  //Iterate over test cases.
  for (const auto item : testCases)
  {
    //construct file name
    const std::string fileName = zipDirectory + libstriezel::filesystem::pathDelimiter + item.first;
    //existence check
    if (!libstriezel::filesystem::file::exists(fileName))
    {
      std::cout << "Error: File " << fileName << " does not exist!" << std::endl;
      return 1;
    }
    //check, if it is a ZIP
    const bool isZip = libstriezel::zip::archive::isZip(fileName);
    std::cout << "isZip(" << fileName << ") = "
              << (isZip ? "yes" : "no") << std::endl;
    //compare with expected values
    if (isZip != item.second)
    {
      if (item.second)
        std::cout << "Error: File " << fileName << " should be a ZIP file, "
                  << "but it was NOT detected as ZIP file!" << std::endl;
      else
        std::cout << "Error: File " << fileName << " should NOT be a ZIP file, "
                  << "but it WAS detected as ZIP file!" << std::endl;
      return 1;
    }
  } //for
  //All OK.
  std::cout << "Tests for libstriezel::zip::archive::isZip() were successful." << std::endl;
  return 0;
}
