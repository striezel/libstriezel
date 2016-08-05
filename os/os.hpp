/*
 -------------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2014, 2015  Dirk Stolle

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
 -------------------------------------------------------------------------------
*/

#ifndef LIBSTRIEZEL_OS_HPP
#define LIBSTRIEZEL_OS_HPP

#include <string>

namespace libstriezel
{
  /// class that handles OS-related stuff
  class os
  {
      #if defined(__linux__) || defined(__linux)
      /** \brief tries to find hints which Linux distribution is running
       *
       * \return Returns the name of the Linux distro or just "Linux",
       *         if no hints could be found.
       *
       */
      static std::string detectLinuxDistro();
      #endif //Linux
    public:
      /** \brief tries to detect the operating system
       *
       * \return Returns a string that identifies the OS.
       * \remarks The return value may only be a rough indication of the OS.
       *
       */
      static std::string detect();
  }; //class
} //namespace

#endif // LIBSTRIEZEL_OS_HPP
