/*
 -----------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2015, 2021, 2025  Dirk Stolle

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

#include "processors.hpp"
#if defined(_WIN32)
#include <windows.h>
#else
#include <fstream>
#include <cstring>
#endif

namespace libstriezel::procfs
{

int getProcessorCount()
{
  #if defined(_WIN32)
  DWORD procs = GetActiveProcessorCount(ALL_PROCESSOR_GROUPS);
  if (procs == 0)
  {
    return -1;
  }
  return procs;
  #else
  std::ifstream input;
  input.open("/proc/cpuinfo", std::ios_base::in | std::ios_base::binary);
  if (!input.good())
    return -1;

  int procs = 0;
  const unsigned int buffer_size = 4096;
  char buffer[buffer_size];
  memset(buffer, '\0', buffer_size);
  std::string line = "";
  while (input.getline(buffer, buffer_size - 1))
  {
    line = std::string(buffer);
    if (line.find("processor") == 0)
      ++procs;
    memset(buffer, '\0', buffer_size);
  }
  const bool reached_eof = input.eof();
  input.close();
  if (reached_eof)
    return procs;
  return -1;
  #endif
}

} // namespace
