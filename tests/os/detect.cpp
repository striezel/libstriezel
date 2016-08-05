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
#include "../../os/os.hpp"

int main()
{
  const std::string os = libstriezel::os::detect();
  std::cout << "Detected operating system: " << os << std::endl;
  #if defined(_WIN64) || defined(__WIN64__) || defined(_WIN32) || defined(__WIN32__) || defined(__WINDOWS__)
  if (os.find("Windows") == std::string::npos)
  {
    std::cout << "Failure: We are on a Windows system, but the name of the "
              << "detected OS does not contain \"Windows\"!" << std::endl;
    return 1;
  }
  #elif defined(__linux__) || defined(__linux)
  //Could be pretty much any distribution, so we just check that we get a
  // non-empty result.
  if (os.empty())
  {
    std::cout << "Failure: Could not detect the operating system!" << std::endl;
    return 1;
  }
  #else
    #error Unknown operating system!
    /* Feel free to extend that code for Mac OS or whatever you are using, and
       don't forget to send a pull request. */
  #endif // defined Windows
  std::cout << "Test of libstriezel::os::detect() was successful."<< std::endl;
  return 0;
}
