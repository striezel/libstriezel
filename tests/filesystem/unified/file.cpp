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
#include <set>
#include "../../../filesystem/file.hpp"

TEST_CASE("filesystem::file")
{
  SECTION("createTemp")
  {
    SECTION("create 20 temporary files")
    {
      std::set<std::string> tempFileNames;

      for (int i = 0; i < 20; ++i)
      {
        std::string fileName = "";
        REQUIRE( libstriezel::filesystem::file::createTemp(fileName) );

        // make sure that names vary
        const bool not_in_set = tempFileNames.find(fileName) == tempFileNames.end();
        REQUIRE( not_in_set );
        tempFileNames.insert(fileName);

        // remove temporary file to keep file system clean
        REQUIRE( libstriezel::filesystem::file::remove(fileName) );
      }
    }

    SECTION("create several temp. files simultaneously")
    {
      std::set<std::string> tempFileNames;

      for (int i = 0; i < 50; ++i)
      {
        std::string fileName = "";
        REQUIRE( libstriezel::filesystem::file::createTemp(fileName) );

        // make sure that names vary
        const bool not_in_set = tempFileNames.find(fileName) == tempFileNames.end();
        REQUIRE( not_in_set );

        tempFileNames.insert(fileName);
      }

      // remove temporary files
      for(const auto & fn : tempFileNames)
      {
        REQUIRE( libstriezel::filesystem::file::remove(fn) );
      }
    }
  }

  SECTION("rename")
  {
    // test scenario: create temporary file and then rename/move it
    std::string originalFileName = "";
    REQUIRE( libstriezel::filesystem::file::createTemp(originalFileName) );

    const std::string newFileName = originalFileName + "_new";
    // new file shall not exist yet
    REQUIRE_FALSE( libstriezel::filesystem::file::exists(newFileName) );

    // move file
    REQUIRE( libstriezel::filesystem::file::rename(originalFileName, newFileName) );

    // old file should not exist anymore ...
    REQUIRE_FALSE( libstriezel::filesystem::file::exists(originalFileName) );

    // ... but "new" file has to exist
    REQUIRE( libstriezel::filesystem::file::exists(newFileName) );

    // remove file
    REQUIRE( libstriezel::filesystem::file::remove(newFileName) );
  }
}
