/*
 -----------------------------------------------------------------------------
    This file is part of a test suite for striezel's common code library.
    Copyright (C) 2016, 2025  Dirk Stolle

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

#include "../../locate_catch.hpp"
#include <iostream>
#include <tuple>
#include <vector>
#include "../../../filesystem/file.hpp"

TEST_CASE("filesystem::splitPathFileExtension")
{
  typedef std::tuple<std::string, char, std::string, std::string, std::string> tupT;

  const std::vector< tupT > cases
    = {
      { tupT(std::string(""), '/', std::string(""), std::string(""), std::string("")) },
      { tupT("/home/user/path/readme.txt", '/', "/home/user/path/", "readme", "txt") },
      { tupT("/home/user/path/file", '/', "/home/user/path/", "file", "") },
      { tupT("../path/to/some.thing", '/', "../path/to/", "some", "thing") },
      { tupT("relative/path/to/some.thing", '/', "relative/path/to/", "some", "thing") },
      { tupT("C:\\Program Files (x86)\\Bloatware\\leak_pointer.exe", '\\', "C:\\Program Files (x86)\\Bloatware\\", "leak_pointer", "exe") },
      { tupT("D:\\Director\\Y\\file.ext", '\\', "D:\\Director\\Y\\", "file", "ext") }
    };

  for (const auto & item : cases)
  {
    std::string path;
    std::string file;
    std::string ext;
    libstriezel::filesystem::splitPathFileExtension(std::get<0>(item), std::get<1>(item), path, file, ext);
    std::cout << "splitPathFileExtension(\"" << std::get<0>(item) << "\", '" << std::get<1>(item) << "', ...)" << std::endl
              << "\tpath: \"" << path << "\"" << std::endl
              << "\tfile: \"" << file << "\"" << std::endl
              << "\text:  \"" << ext << "\"" << std::endl;
    REQUIRE( std::get<2>(item) == path );
    REQUIRE( std::get<3>(item) == file );
    REQUIRE( std::get<4>(item) == ext );
  }
}
