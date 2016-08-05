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

#ifndef LIBSTRIEZEL_READBMP_HPP
#define LIBSTRIEZEL_READBMP_HPP

#include <string>
#include <GL/gl.h>
#if defined(__WIN32__)
  //under windows, some GL constants seem to be defined in another header
  #include <GL/glext.h>
#endif

#include "GLImageStructure.hpp"

/* returns a structure containing the image data of the given bitmap file

   parameters:
       FileName - name of the bitmap
*/
GLImageStructure readBMP(const std::string& FileName);

/* returns true, if the given file seems to be a bitmap file according to its
   first bytes

   parameters:
       header - pointer to an array that contains at least the first two bytes
                from the file
       length - number of elements in the array - has to be at least two
*/
bool isBMP(const unsigned char* header, const size_t length);

#endif //LIBSTRIEZEL_READBMP_HPP
