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

const char* GLImageStructure::getBufferPointer() const
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

void GLImageStructure::setBuffer(char* b)
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
