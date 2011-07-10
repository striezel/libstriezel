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

#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <string>
#include "GLImageStructure.h"

namespace ImageLoader
{

/* returns true, if the given file seems to be a supported image file according
   to its first bytes

   parameters:
       FileName - name of the suspected image file

   remarks:
       Currently the following formats are supported: Bitmap, PNG, JPEG, PPM
       (binary PPM only).
*/
bool isSupportedImage(const std::string& FileName);

/* returns a structure containing the image data of the given image file

   parameters:
       FileName - name of the image file
*/
GLImageStructure readImage(const std::string& FileName);

}//namespace

#endif // IMAGELOADER_H
