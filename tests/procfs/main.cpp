/*
 -----------------------------------------------------------------------------
    This file is part of a test suite for Thoronador's common code library.
    Copyright (C) 2016  Thoronador

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
#include "../../procfs/processors.hpp"

int main()
{
  const auto procCount = libthoro::procfs::getProcessorCount();
  if (procCount > 0)
  {
    std::cout << "Machine has " << procCount << " processor(s)." << std::endl
              << "Test successful." << std::endl;
    return 0;
  }
  else
  {
    std::cout << "Test FAILED!" << std::endl;
    return 1;
  }
}
