/*
 -----------------------------------------------------------------------------
    This file is part of a test suite for striezel's common code library.
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
#include <vector>
#include "../../../../filesystem/directory.hpp"
#include "../../../../filesystem/file.hpp"

int main(int argc, char** argv)
{
  if ((argc!=2) || (argv==nullptr) || (argv[1]==nullptr))
  {
    std::cout << "Error: You have to pass the path to the file directory as first and only parameter!" << std::endl;
    return 1;
  }

  //base directory where all the files are located
  const std::string baseDir = libstriezel::filesystem::slashify(std::string(argv[1]));

  //var to save file content
  std::string content = "This is not the content you are looking for.";

  //file_zero_bytes.dat: file with size of zero bytes
  if (!libstriezel::filesystem::file::readIntoString(baseDir+"file_zero_bytes.dat", content))
  {
    std::cout << "Error: Could not read data from file file_zero_bytes.dat!" << std::endl;
    return 1;
  }
  if (!content.empty())
  {
    std::cout << "Error: Content from file file_zero_bytes.dat should be empty string!"
              << " Current content is \"" << content << "\" instead." << std::endl;
    return 1;
  }

  //file_16_nul_bytes.dat: 16 NUL bytes
  content = "This is not the content you are looking for."; //reset content
  if (!libstriezel::filesystem::file::readIntoString(baseDir+"file_16_nul_bytes.dat", content))
  {
    std::cout << "Error: Could not read data from file file_16_nul_bytes.dat!" << std::endl;
    return 1;
  }
  if (content != std::string(16, '\0') || (content.size() != 16))
  {
    std::cout << "Error: Content from file file_16_nul_bytes.dat should be 16 NUL bytes!"
              << " Current content is " << content.size() << " characters instead." << std::endl;
    return 1;
  }


  //file_256x2.dat: two times all bytes from 0 to 255
  content = "This is not the content you are looking for."; //reset content
  if (!libstriezel::filesystem::file::readIntoString(baseDir+"file_256x2.dat", content))
  {
    std::cout << "Error: Could not read data from file file_256x2.dat!" << std::endl;
    return 1;
  }
  if (content.size() != 512)
  {
    std::cout << "Error: Content from file file_256x2.dat should be 512 characters!"
              << " Current content is " << content.size() << " characters instead." << std::endl;
    return 1;
  }
  int i;
  for (i=0; i<512; ++i)
  {
    if (content[i] != static_cast<char>(i % 256))
    {
      std::cout << "Error: Content from file file_256x2.dat is not as expected!"
                << "Character at index " << i << " is code " << static_cast<int>(content[i])
                << ", but is should be code " << (i % 256) << " instead!" << std::endl;
      return 1;
    }
  } //for

  //Everything is OK.
  std::cout << "Test for file::readIntoString() passed!" << std::endl;
  return 0;
}
