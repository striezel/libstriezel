/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2011, 2012, 2014  Dirk Stolle

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

#include "readPPM.hpp"
#include <GL/gl.h>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include "../IntegerUtils.hpp"
#include "GLfunctions.hpp"

bool isPPM(const unsigned char* header, const size_t length)
{
  if (length<2) return false;
  //P6 are the first bytes usually (we only care for binary format here)
  return ((header[0]=='P') && (header[1]=='6'));
}

GLImageStructure readPPM(const std::string& FileName)
{
  GLImageStructure result;
  result.setBuffer(NULL);
  result.setWidth(0);
  result.setHeight(0);
  result.setFormat(0);

  FILE * file_ppm = fopen(FileName.c_str(), "rb");
  if (!file_ppm)
  {
    std::cout << "readPPM(): File \""<<FileName<<"\" could not be opened for reading.\n";
    return result;
  }
  char header[2];
  memset(header, 0, 2);
  if (fread(header, 1, 2, file_ppm)!=2)
  {
    //file is not long enough to be a PPM file
    fclose(file_ppm);
    std::cout << "readPPM(): File \""<<FileName<<"\" is too short.\n";
    return result;
  }
  if ((header[0]!='P') || (header[1]!='6'))
  {
    fclose(file_ppm);
    std::cout << "readPPM(): File \""<<FileName<<"\" is not a valid PPM file.\n";
    return result;
  }

  //read end of line
  if (fread(header, 1, 1, file_ppm)!=1)
  {
    //file is not long enough to be a PPM file
    fclose(file_ppm);
    std::cout << "readPPM(): File \""<<FileName<<"\" is too short.\n";
    return result;
  }
  if (header[0]!='\n')
  {
    fclose(file_ppm);
    std::cout << "readPPM(): File \""<<FileName<<"\" has invalid header.\n";
    return result;
  }
  //read width
  unsigned int width = 0;
  header[0]=0;
  while (header[0]!=' ')
  {
    //read next character
    if (fread(header, 1, 1, file_ppm)!=1)
    {
      //file is not long enough to be a PPM file
      fclose(file_ppm);
      std::cout << "readPPM(): File \""<<FileName<<"\" is too short.\n";
      return result;
    }//if
    //check for digit
    if (((header[0]<'0') || (header[0]>'9')) && (header[0]!=' '))
    {
      fclose(file_ppm);
      std::cout << "readPPM(): File \""<<FileName<<"\" has invalid dimensions.\n";
      return result;
    }
    //process next digit
    if (header[0]!=' ')
    {
      width = width*10 + (header[0]-'0');
    }
  }//while

  //did we actually read something useful?
  if (width==0)
  {
    fclose(file_ppm);
    std::cout << "readPPM(): File \""<<FileName<<"\" has invalid dimension (width).\n";
    return result;
  }
  result.setWidth(width);

  //now read the height
  width = 0;
  header[0]=0;
  while (header[0]!=10)
  {
    //read next character
    if (fread(header, 1, 1, file_ppm)!=1)
    {
      //file is not long enough to be a PPM file
      fclose(file_ppm);
      std::cout << "readPPM(): File \""<<FileName<<"\" is too short.\n";
      return result;
    }//if
    //check for digit
    if (((header[0]<'0') || (header[0]>'9')) && (header[0]!=10))
    {
      fclose(file_ppm);
      std::cout << "readPPM(): File \""<<FileName<<"\" has invalid dimensions.\n";
      return result;
    }
    //process next digit
    if (header[0]!=10)
    {
      width = width*10 + (header[0]-'0');
    }
  }//while

  //did we actually read something useful?
  if (width==0)
  {
    fclose(file_ppm);
    std::cout << "readPPM(): File \""<<FileName<<"\" has invalid dimension (height).\n";
    return result;
  }
  result.setHeight(width);

  if (((!isPowerOfTwo(result.getHeight())) || (!isPowerOfTwo(result.getWidth())))
     && !hasNPOTSupport())
  {
    std::cout << "Width or height of \""<<FileName<<"\" is not a power of two "
              << "and NPOT textures are not supported by your OpenGL version.\n";
    fclose(file_ppm);
    return result;
  }

  //now the maximum
  width = 0;
  header[0]=0;
  while (header[0]!=10)
  {
    //read next character
    if (fread(header, 1, 1, file_ppm)!=1)
    {
      //file is not long enough to be a PPM file
      fclose(file_ppm);
      std::cout << "readPPM(): File \""<<FileName<<"\" is too short.\n";
      return result;
    }//if
    //check for digit
    if (((header[0]<'0') || (header[0]>'9')) && (header[0]!=10))
    {
      fclose(file_ppm);
      std::cout << "readPPM(): File \""<<FileName<<"\" has invalid dimensions.\n";
      return result;
    }
    //process next digit
    if (header[0]!=10)
    {
      width = width*10 + (header[0]-'0');
    }
  }//while

  //did we actually read something useful?
  if (width==0)
  {
    fclose(file_ppm);
    std::cout << "readPPM(): File \""<<FileName<<"\" has invalid max. component value.\n";
    return result;
  }

  const unsigned int max_value = width;
  if (max_value!=255)
  {
    fclose(file_ppm);
    std::cout << "readPPM(): File \""<<FileName<<"\" has max. component value "
              << max_value<<", which is not 255 and thus not supported yet!\n";
    return result;
  }

  //format is RGB, always
  result.setFormat(GL_RGB);

  //the rest of this stuff is the image data
  const unsigned int line_size_in_bytes = 3*result.getWidth();

  //allocate buffer for reading one line
  unsigned char* line_buffer = (unsigned char*) malloc(line_size_in_bytes);
  //allocate overall image buffer
  unsigned char* v_ptr = (unsigned char*) malloc(line_size_in_bytes*result.getHeight());
  memset(v_ptr, 0, line_size_in_bytes*result.getHeight());
  unsigned int offset = line_size_in_bytes*(result.getHeight()-1);

  unsigned int lines_read = 0;
  while (lines_read<result.getHeight())
  {
    if (fread(line_buffer, 1, line_size_in_bytes, file_ppm)!=line_size_in_bytes)
    {
      fclose(file_ppm);
      std::cout << "readPPM(): File \""<<FileName<<"\" is too short, could not "
                << "read all image lines!\n";
      free(v_ptr);
      free(line_buffer);
      return result;
    }//if
    //copy from line buffer to real buffer
    memcpy(&(v_ptr[offset]), line_buffer, line_size_in_bytes);
    offset -= line_size_in_bytes;
    ++lines_read;
  }//while
  fclose(file_ppm);
  //free the read buffer
  free(line_buffer);
  //set the GL image structure's pointer
  result.setBuffer(v_ptr);
  return result;
}//function readPPM

