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

#include "readPNG.h"
#include <png.h>
#include <iostream>
#include <cstdlib>
#include <GL/gl.h>

//aux. function
void deallocateRowPointers(png_bytep* rp, const unsigned int height)
{
  unsigned int i;
  for (i=0; i<height; ++i)
  {
    free(rp[i]);
    rp[i] = NULL;
  }
}

//tries to read a png file and returns non-NULL buffer within structure on success

GLImageStructure readPNG(const std::string& FileName)
{
  GLImageStructure result;
  result.setBuffer(NULL);
  result.setWidth(0);
  result.setHeight(0);
  result.setFormat(0);

  png_byte header[8];// 8 is the maximum size that can be checked by libpng
  FILE *file_png = fopen(FileName.c_str(), "rb");
  if (!file_png)
  {
    std::cout << "File \""<<FileName<<"\" could not be opened for reading.\n";
    return result;
  }
  //read signature
  fread(header, 1, 8, file_png);
  if (png_sig_cmp(header, 0, 8))
  {
    std::cout << "File \""<<FileName<<"\" is not a PNG file.\n";
    fclose(file_png);
    return result;
  }

  // initialize
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr)
  {
    std::cout << "png_create_read_struct() failed with file \""<<FileName<<"\".\n";
    fclose(file_png);
    return result;
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
  {
    std::cout << "png_create_info_struct() failed with file \""<<FileName<<"\".\n";
    fclose(file_png);
    return result;
  }

  png_init_io(png_ptr, file_png);
  png_set_sig_bytes(png_ptr, 8);
  png_read_info(png_ptr, info_ptr);
  //set width/height values
  result.setWidth(png_get_image_width(png_ptr, info_ptr));
  result.setHeight(png_get_image_height(png_ptr, info_ptr));
  //get colour type
  png_byte colour_type = png_get_color_type(png_ptr, info_ptr);
  switch (colour_type)
  {
    case PNG_COLOR_TYPE_RGB:
         result.setFormat(GL_RGB);
         break;
    case PNG_COLOR_TYPE_RGBA:
         result.setFormat(GL_RGBA);
         break;
    default:
         std::cout << "Colour type of file \""<<FileName<<"\" is neither RGB nor RGBA.\n";
         return result;
  }//swi

  //png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);

  int number_of_passes = png_set_interlace_handling(png_ptr);
  png_read_update_info(png_ptr, info_ptr);

  // read file
  png_bytep* row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * result.getHeight());
  const unsigned int row_size_in_bytes = png_get_rowbytes(png_ptr,info_ptr);

  unsigned int y;
  for (y=0; y<result.getHeight(); y++)
  {
    row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));
  }//if

  png_read_image(png_ptr, row_pointers);
  fclose(file_png);

  //all is read, now get it into the void array
  char* v_ptr = (char*) malloc(png_get_rowbytes(png_ptr,info_ptr)*result.getHeight()*sizeof(png_bytep));
  if (v_ptr==NULL)
  {
    std::cout << "Could not allocate memory for pixel data!\n";
    deallocateRowPointers(row_pointers, result.getHeight());
    free(row_pointers);
    return result;
  }
  //copy it
  unsigned int offset = 0;
  for (y=0; y<result.getHeight(); ++y)
  {
    //since OpenGL starts pixel data in lower left corner and libpng in upper
    // left, we have to copy it the other way round
    memcpy(&(v_ptr[offset]), row_pointers[result.getHeight()-1-y], row_size_in_bytes);
    offset += row_size_in_bytes;
  }
  result.setBuffer(v_ptr);
  //free row pointers
  deallocateRowPointers(row_pointers, result.getHeight());
  //free row pointer array
  free(row_pointers);
  row_pointers = NULL;
  v_ptr = NULL;
  return result;
}//function readPNG

bool isPNG(const std::string& FileName)
{
  FILE *file_png = fopen(FileName.c_str(), "rb");
  if (!file_png)
  {
    //file could not be opened for reading
    return false;
  }

  png_byte header[8];// 8 is the maximum size that can be checked by libpng
  memset(header, 0, 8);
  //read first eight bytes
  if (fread(header, 1, 8, file_png)!=8)
  {
    //file is not long enough to be a PNG file
    fclose(file_png);
    return false;
  }
  fclose(file_png);
  return (png_sig_cmp(header, 0, 8)==0);
}//function isPNG
