/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2011 thoronador

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

#include "readGIF.h"
#include <iostream>
#include "GIFStructures.h"

GLImageStructure readGIF(const std::string& FileName)
{
  #warning Not implemented yet!
  GLImageStructure result;
  GIF file_gif;
  if (!file_gif.readFromFile(FileName, true))
  {
    std::cout << "Error: could not read GIF file \""<<FileName<<"\".\n";
    return result;
  }

  #warning To be continued.
  #warning Not completely implemented yet!
  std::cout << "Error: Function readGIF() is incomplete. Wait for next commit to SVN. :p\n";
  return result;
}

bool isGIF(const unsigned char* header, const size_t length)
{
  if (length<6) return false;
  if ((header[0]=='G') and (header[1]=='I') and (header[2]=='F')
      and (header[3]=='8') and (header[5]=='a'))
  {
    //we are quite close here, just check the one character that can differ
    return ((header[4]=='7') or (header[4]=='9'));
  }
  return false;
}//function isGIF
