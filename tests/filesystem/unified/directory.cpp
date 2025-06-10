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
#include "../../../filesystem/directory.hpp"

TEST_CASE("filesystem::directory")
{
  SECTION("createTemp")
  {
    SECTION("create 20 temporary directories")
    {
      std::set<std::string> tempDirNames;

      for (int i = 0; i < 20; ++i)
      {
        std::string dirName = "";
        REQUIRE( libstriezel::filesystem::directory::createTemp(dirName) );

        // make sure that names vary
        const bool not_in_set = tempDirNames.find(dirName) == tempDirNames.end();
        REQUIRE( not_in_set );
        tempDirNames.insert(dirName);

        // remove temporary directory to keep file system clean
        REQUIRE( libstriezel::filesystem::directory::remove(dirName) );
      }
    }

    SECTION("create several temp. directories simultaneously")
    {
      std::set<std::string> tempDirNames;

      for (int i = 0; i < 50; ++i)
      {
        std::string dirName = "";
        REQUIRE( libstriezel::filesystem::directory::createTemp(dirName) );

        // make sure that names vary
        const bool not_in_set = tempDirNames.find(dirName) == tempDirNames.end();
        REQUIRE( not_in_set );
        tempDirNames.insert(dirName);
      }

      // remove temporary directories
      for(const auto & dn : tempDirNames)
      {
        REQUIRE( libstriezel::filesystem::directory::remove(dn) );
      }
    }
  }

  SECTION("getHome")
  {
    std::string home;
    REQUIRE( libstriezel::filesystem::directory::getHome(home) );
    REQUIRE_FALSE( home.empty() );
    // Check some common locations.
    const bool startsWithCUsers = (home.find("C:\\Users\\") == 0)
                               || (home.find("c:\\Users\\") == 0)
                               || (home.find("c:\\users\\") == 0);
    const bool windowsSystemProfile = (home.find("C:\\Windows\\System32\\config\\systemprofile") == 0);
    const bool startsWithHome = (home.find("/home/") == 0);
    const bool startsWithRoot = (home.find("/root") == 0);
    const bool isCommon =  startsWithCUsers || windowsSystemProfile || startsWithHome || startsWithRoot;
    REQUIRE( isCommon );
  }
}
