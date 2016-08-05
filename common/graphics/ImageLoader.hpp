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

#ifndef LIBSTRIEZEL_IMAGELOADER_HPP
#define LIBSTRIEZEL_IMAGELOADER_HPP

#include <string>
#include "GLImageStructure.hpp"

namespace ImageLoader
{


/* enumeration type to indicate image type */
enum ImageType {itUnknown, itBitmap, itGIF, itJPEG, itPNG, itPPM};

/* returns true, if the given file seems to be a supported image file according
   to its first bytes

   parameters:
       it - type the suspected image file, as given by getImageType()

   remarks:
       Currently the following formats are supported: Bitmap, PNG, JPEG, PPM
       (binary PPM only).
*/
bool isSupportedImage(const ImageType it);

/* returns the image type of the given file according to its first bytes

   parameters:
       FileName - name of the suspected image file

   remarks:
       Currently the following formats are recognised: Bitmap, PNG, JPEG, PPM
       (binary PPM only).
*/
ImageType getImageType(const std::string& FileName);

/* returns a structure containing the image data of the given image file

   parameters:
       FileName  - name of the image file
       type_hint - indicator of the images type. If this is wrong, reading the
                   image will fail. If you don't know the type, set it to
                   itUnknown  and the function will guess itself.
*/
GLImageStructure readImage(const std::string& FileName, ImageType type_hint);

}//namespace

#endif // LIBSTRIEZEL_IMAGELOADER_HPP
