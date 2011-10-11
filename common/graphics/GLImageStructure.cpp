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

#include "GLImageStructure.h"
#include <cstdlib>

GLImageStructure::GLImageStructure()
{
  buffer = NULL;
  width = 0;
  height = 0;
  format = 0;
}

GLImageStructure::~GLImageStructure()
{
  //empty
}

bool GLImageStructure::isLoaded() const
{
  return ((width>0) and (height>0) and (buffer!=NULL) and (format!=0));
}

unsigned int GLImageStructure::getWidth() const
{
  return width;
}

unsigned int GLImageStructure::getHeight() const
{
  return height;
}

GLint GLImageStructure::getFormatGL() const
{
  return format;
}

const unsigned char* GLImageStructure::getBufferPointer() const
{
  return buffer;
}

void GLImageStructure::setWidth(const unsigned int w)
{
  width = w;
}

void GLImageStructure::setHeight(const unsigned int h)
{
  height = h;
}

void GLImageStructure::setFormat(const GLint f)
{
  format = f;
}

void GLImageStructure::setBuffer(unsigned char* b)
{
  buffer = b;
}

void GLImageStructure::freeBuffer()
{
  if (buffer!=NULL)
  {
    free(buffer);
    buffer = 0;
  }//if
}

bool GLImageStructure::resizeToHalf()
{
  if (!isLoaded()) return false;
  //calculate new dimensions
  const unsigned int halfWidth = width / 2;
  const unsigned int halfHeight = height / 2;
  //no zero sized images please
  if ((halfWidth==0) or (halfHeight==0)) return false;

  unsigned int components = 0;
  if ((getFormatGL()==GL_RGB) or (getFormatGL()==GL_BGR))
  {
    components = 3;
  }
  else if (getFormatGL()==GL_RGBA)
  {
    components = 4;
  }
  else
  {
    //unknown format, not supported
    return false;
  }
  unsigned char * newBuffer = static_cast<unsigned char*> (malloc(halfWidth*halfHeight*components));
  if (newBuffer==NULL)
  {
    return false;
  }
  //Fill new buffer - we just use every second pixel from every second line.
  // That's not the best quality, but it's faster.
  unsigned int i, j, k;
  for (j=0; j<halfHeight; ++j)
  {
    for (i=0; i<halfWidth; ++i)
    {
      for (k=0; k<components; ++k)
      {
        newBuffer[components*(i+halfWidth*j)+k] = buffer[components*(i*2+width*j*2)+k];
      }//for k
    }//for (inner)
  }//for (outer)
  unsigned char * temp = buffer;

  width = halfWidth;
  height = halfHeight;
  buffer = newBuffer;
  free(temp);
  return true;
}
