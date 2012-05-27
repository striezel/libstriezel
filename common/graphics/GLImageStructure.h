/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2011, 2012 thoronador

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

#ifndef GLIMAGESTRUCTURE_H
#define GLIMAGESTRUCTURE_H

#include <GL/gl.h>
#if defined(__WIN32__) || defined(_WIN32_) || defined(_WIN32)
  //under windows, some GL constants seem to be defined in another header
  #include <GL/glext.h>
#endif

struct GLImageStructure
{
  private:
    unsigned char * buffer;
    unsigned int width;
    unsigned int height;
    GLint format;
  public:
    // constructor
    GLImageStructure();

    // copy constructor
    GLImageStructure(const GLImageStructure& op);

    // assignment operator
    GLImageStructure& operator=(const GLImageStructure& op);

    // destructor
    ~GLImageStructure();

    // returns true, if the structure seems to contain valid data
    bool isLoaded() const;

    // returns the width value
    unsigned int getWidth() const;

    // returns the height value
    unsigned int getHeight() const;

    //returns true, if the loading process of that texture needs NPOT handling
    bool needsNPOTExtension() const;

    // returns the format
    GLint getFormatGL() const;

    /* returns the number of components, judging from the internal GL format.
       If this function returns zero, the internal format is either zero/not set
       or an unknown format.
    */
    unsigned int getNumberOfComponents() const;

    // returns a constant pointer to the buffer
    const unsigned char* getBufferPointer() const;

    //setter functions
    void setWidth(const unsigned int w);
    void setHeight(const unsigned int h);
    void setFormat(const GLint f);
    void setBuffer(unsigned char* b);

    //frees the buffer - this should usually be called before destruction
    void freeBuffer();

    /*resizes the image to half width and height (i.e. one quarter of original)
      and returns true in case of success */
    bool resizeToHalf();
};//struct

#endif // GLIMAGESTRUCTURE_H
