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

#include "ImageLoader.hpp"
#include "readBMP.hpp"
#include "readGIF.hpp"
#include "readJPEG.hpp"
#include "readPNG.hpp"
#include "readPPM.hpp"

namespace ImageLoader
{

bool isSupportedImage(const ImageType it)
{
  return (it!=itUnknown);
}

ImageType getImageType(const std::string& FileName)
{
  FILE *file_image = fopen(FileName.c_str(), "rb");
  if (!file_image)
  {
    //file could not be opened for reading
    return itUnknown;
  }

  unsigned char header[8];
  memset(header, 0, 8);
  //read first eight bytes
  if (fread(header, 1, 8, file_image)!=8)
  {
    //file is not long enough to be a proper image file
    fclose(file_image);
    return itUnknown;
  }
  fclose(file_image);

  if (isJPEG(header, 8))
  {
    return itJPEG;
  }
  if (isPNG(header, 8))
  {
    return itPNG;
  }
  if (isGIF(header, 8))
  {
    return itGIF;
  }
  if (isPPM(header, 8))
  {
    return itPPM;
  }
  if (isBMP(header, 8))
  {
    return itBitmap;
  }
  return itUnknown;
}

GLImageStructure readImage(const std::string& FileName, ImageType type_hint)
{
  if (type_hint==itUnknown)
  {
    type_hint = getImageType(FileName);
  }
  switch (type_hint)
  {
    case itJPEG:
         return readJPEG(FileName);
    case itPNG:
         return readPNG(FileName);
    case itGIF:
         return readGIF(FileName);
    case itBitmap:
         return readBMP(FileName);
    case itPPM:
         return readPPM(FileName);
  }

  //no supported image here
  GLImageStructure temp_glis;
  temp_glis.setWidth(0);
  temp_glis.setHeight(0);
  temp_glis.setFormat(0);
  temp_glis.setBuffer(NULL);
  return temp_glis;
}

} //namespace
