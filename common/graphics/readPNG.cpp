/*
 -----------------------------------------------------------------------------
    This file is part of striezel's common code library.
    Copyright (C) 2011, 2012, 2025  Dirk Stolle

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

#include "readPNG.hpp"
#include <png.h>
#include <iostream>
#include <cstdlib>
#include <GL/gl.h>
#include "../IntegerUtils.hpp"
#include "GLfunctions.hpp"

// aux. function
void deallocateRowPointers(png_bytep* rp, const unsigned int height)
{
  for (unsigned int i = 0; i < height; ++i)
  {
    free(rp[i]);
    rp[i] = nullptr;
  }
}

// tries to read a png file and returns non-NULL buffer within structure on success

GLImageStructure readPNG(const std::string& FileName)
{
  GLImageStructure result;
  result.setBuffer(nullptr);
  result.setWidth(0);
  result.setHeight(0);
  result.setFormat(0);

  png_byte header[8]; // 8 is the maximum size that can be checked by libpng
  FILE *file_png = fopen(FileName.c_str(), "rb");
  if (!file_png)
  {
    std::cout << "File \"" << FileName << "\" could not be opened for reading.\n";
    return result;
  }
  // read signature
  const size_t itemsRead = fread(header, 1, 8, file_png);
  if (png_sig_cmp(header, 0, itemsRead) != 0)
  {
    std::cout << "File \"" << FileName << "\" is not a PNG file.\n";
    fclose(file_png);
    return result;
  }

  // initialize
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

  if (!png_ptr)
  {
    std::cout << "png_create_read_struct() failed with file \"" << FileName << "\".\n";
    fclose(file_png);
    return result;
  }

  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr)
  {
    std::cout << "png_create_info_struct() failed with file \"" << FileName << "\".\n";
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(file_png);
    return result;
  }

  png_init_io(png_ptr, file_png);
  png_set_sig_bytes(png_ptr, 8);
  png_read_info(png_ptr, info_ptr);

  // get colour type and bit depth
  png_byte colour_type = png_get_color_type(png_ptr, info_ptr);
  png_byte bit_depth = png_get_bit_depth(png_ptr, info_ptr);
  // expansions and transformations
  // expand paletted images to RGB
  if (colour_type == PNG_COLOR_TYPE_PALETTE)
  {
    png_set_expand(png_ptr);
  }
  // expand low bit greyscale images to 8 bit
  if ((colour_type == PNG_COLOR_TYPE_GRAY) && (bit_depth < 8))
  {
    png_set_expand(png_ptr);
  }
  // strip 16 bits per sample to 8 bits per sample
  if (bit_depth == 16)
  {
    png_set_strip_16(png_ptr);
  }
  // set greyscaled images to RGB(A) images
  if ((colour_type == PNG_COLOR_TYPE_GRAY) || (colour_type == PNG_COLOR_TYPE_GRAY_ALPHA))
  {
    png_set_gray_to_rgb(png_ptr);
  }

  //int number_of_passes = png_set_interlace_handling(png_ptr);
  png_read_update_info(png_ptr, info_ptr);

  // set width/height values
  result.setWidth(png_get_image_width(png_ptr, info_ptr));
  result.setHeight(png_get_image_height(png_ptr, info_ptr));

  if ((!isPowerOfTwo(result.getHeight()) || !isPowerOfTwo(result.getWidth()))
     && !hasNPOTSupport())
  {
    std::cout << "Width or height of \"" << FileName << "\" is not a power of two "
              << "and NPOT textures are not supported by your OpenGL version.\n";
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    fclose(file_png);
    return result;
  }

  // update colour type, it might have changed
  colour_type = png_get_color_type(png_ptr, info_ptr);

  switch (colour_type)
  {
    case PNG_COLOR_TYPE_RGB:
         result.setFormat(GL_RGB);
         break;
    case PNG_COLOR_TYPE_RGBA:
         result.setFormat(GL_RGBA);
         break;
    default:
         std::cout << "Colour type of file \"" << FileName << "\" is neither RGB nor RGBA.\n";
         png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
         fclose(file_png);
         return result;
  }

  // read file
  const unsigned int row_size_in_bytes = png_get_rowbytes(png_ptr,info_ptr);

  // prepare the array for the data
  unsigned char* v_ptr = (unsigned char*) malloc(row_size_in_bytes * result.getHeight() * sizeof(png_byte));
  if (v_ptr == nullptr)
  {
    std::cout << "Could not allocate memory for pixel data!\n";
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    return result;
  }

  // create row pointers
  png_bytepp row_pointers = (png_bytepp) malloc(sizeof(png_bytep) * result.getHeight());
  if (row_pointers == nullptr)
  {
    std::cout << "Could not allocate memory for PNG row pointers!\n";
    png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    free(v_ptr);
    v_ptr = nullptr;
    return result;
  }
  for (unsigned int y = 0; y < result.getHeight(); y++)
  {
    row_pointers[y] = v_ptr + (result.getHeight() - 1 - y) * row_size_in_bytes;
  }

  png_read_image(png_ptr, row_pointers);
  fclose(file_png);

  result.setBuffer(v_ptr);
  png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
  // free row pointers
  //deallocateRowPointers(row_pointers, result.getHeight());
  // free row pointer array
  free(row_pointers);
  row_pointers = nullptr;
  v_ptr = nullptr;
  return result;
} // function readPNG

bool isPNG(const png_bytep header, const size_t length)
{
  if (length < 8)
    return false;
  return png_sig_cmp(header, 0, 8) == 0;
} // function isPNG
