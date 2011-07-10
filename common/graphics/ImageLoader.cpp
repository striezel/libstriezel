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

#include "ImageLoader.h"
#include "readBMP.h"
#include "readJPEG.h"
#include "readPNG.h"
#include "readPPM.h"

namespace ImageLoader
{

bool isSupportedImage(const std::string& FileName)
{
  return (isJPEG(FileName) or isPNG(FileName) or isPPM(FileName) or isBMP(FileName));
}

GLImageStructure readImage(const std::string& FileName)
{
  if (isJPEG(FileName))
  {
    return readJPEG(FileName);
  }
  if (isBMP(FileName))
  {
    return readBMP(FileName);
  }
  if (isPNG(FileName))
  {
    return readPNG(FileName);
  }
  if (isPPM(FileName))
  {
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
