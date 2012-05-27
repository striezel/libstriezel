/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2008, 2009, 2011, 2012 thoronador

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

 The code was mostly copied from vespucci's BitmapReader.pas and translated into
 C/C++. You can see the original Pascal stuff at
 http://vespucci.svn.sourceforge.net/viewvc/vespucci/trunk/engine/BitmapReader.pas?view=markup
*/

#include "readBMP.h"
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include "../IntegerUtils.h"
#include "GLfunctions.h"

struct TBitmapFileHeader
{
  uint16_t bfType;
  uint32_t bfSize;
  uint32_t bfReserved;
  uint32_t bfOffBits;
};//struct

struct TBitmapInfoHeader
{
  uint32_t biSize;
  int32_t  biWidth;
  int32_t  biHeight;
  uint16_t biPlanes;
  uint16_t biBitCount;
  uint32_t biCompression;
  uint32_t biSizeImage;
  int32_t  biXPixelsPerMeter;
  int32_t  biYPixelsPerMeter;
  uint32_t biClrUsed;
  uint32_t biClrImportant;
};//struct


//tries to read a BMP file and returns non-NULL buffer on success

GLImageStructure readBMP(const std::string& FileName)
{
  GLImageStructure result;
  result.setBuffer(NULL);
  result.setWidth(0);
  result.setHeight(0);
  result.setFormat(0);

  FILE *file_bmp = fopen(FileName.c_str(), "rb");
  if (!file_bmp)
  {
    std::cout << "File \""<<FileName<<"\" could not be opened for reading.\n";
    return result;
  }

  TBitmapFileHeader bfh;
  //read bitmap file header
  if (fread(&bfh, 1, 14, file_bmp)!=14)
  {
    std::cout << "File \""<<FileName<<"\" is not long enough to be a BMP file.\n";
    fclose(file_bmp);
    return result;
  }

  TBitmapInfoHeader bih;
  if (fread(&bih, 1, 40, file_bmp)!=40)
  {
    std::cout << "File \""<<FileName<<"\" is too short to read BitmapInfoHeader.\n";
    fclose(file_bmp);
    return result;
  }//if

  //check header info
  if (bfh.bfType!=19778) //i.e. <> "BM"
  {
    std::cout << "Wrong bfType ("<<bfh.bfType<<") in file \"" << FileName
              << "\". Should be 19778.\n";
    fclose(file_bmp);
    return result;
  }//if

  if (bih.biSize!=40)
  {
    std::cout << "Wrong size of BitmapInfoHeader ("<<bih.biSize<<") in file \""
              << FileName<<"\", should be 40.\n";
    fclose(file_bmp);
    return result;
  }//if

  //check width
  if (bih.biWidth<1)
  {
    std::cout << "Width is not at least one px. (Width: "<<bih.biWidth <<")\n";
    fclose(file_bmp);
    return result;
  }//if
  result.setWidth(bih.biWidth);

  //check height
  if (abs(bih.biHeight) <1)
  {
    std::cout << "Height is not at least one px. (Height: "<<abs(bih.biHeight)<<")\n";
    fclose(file_bmp);
    return result;
  }//if
  result.setHeight(abs(bih.biHeight));

  if (((!isPowerOfTwo(result.getHeight())) or (!isPowerOfTwo(result.getWidth())))
     and !hasNPOTSupport())
  {
    std::cout << "Width or height of \""<<FileName<<"\" is not a power of two "
              << "and NPOT textures are not supported by your OpenGL version.\n";
    fclose(file_bmp);
    return result;
  }

  if (bih.biPlanes!=1)
  {
    std::cout << "Invalid number of planes in bitmap file \""<<FileName<<"\".\n";
    fclose(file_bmp);
    return result;
  }//if

  if (bih.biBitCount != 24) //we only want 24bit images
  {
    std::cout << "Bits per pixel is different from 24 in bitmap file \""<<FileName<<"\"!\n";
    fclose(file_bmp);
    return result;
  }//if
  result.setFormat(GL_BGR);

  if (bih.biCompression != 0) //only uncompressed bitmaps wanted
  {
    std::cout << "Bitmap in file \""<<FileName<<"\" uses unsupported, compressed format!\n";
    fclose(file_bmp);
    return result;
  }//if

  //bih.biSizeImage shows size of data in bytes
  if (bih.biSizeImage!=(result.getWidth()*result.getHeight()*3))
  //there aren't three bytes for every px
  {
    std::cout << "Bitmap data in file \""<<FileName<<"\" has invalid size.\n";
    fclose(file_bmp);
    return result;
  }//if

  //now go for the data
  //order for 24bpp is blue, green, red

  //check for Offset and seek it, if neccessary
  const long int filePos = ftell(file_bmp);
  if (filePos==EOF)
  {
    std::cout << "Could not get file position in file \""<<FileName<<"\".\n";
    fclose(file_bmp);
    return result;
  }
  if (bfh.bfOffBits!=filePos)
  {
    const int fseek_return = fseek(file_bmp, bfh.bfOffBits, SEEK_SET);
    if (fseek_return!=0)
    {
      std::cout << "Could not seek data start position for file \"" << FileName
               << "\".\nFile position was " << filePos << ", data offset is "
               << bfh.bfOffBits <<"\n";
      fclose(file_bmp);
      return result;
    }
  }
  //allocate space for pixel data
  unsigned char * v_ptr = (unsigned char*) malloc(result.getWidth()*result.getHeight()*3);
  //now we can start the reading
  if (fread(v_ptr, 1, bih.biSizeImage, file_bmp)!= bih.biSizeImage)
  {
    std::cout << "Could not read all data from file \""<<FileName<<"\"!\n";
    fclose(file_bmp);
    free(v_ptr);
    return result;
  }//if
  //we are lucky...so far
  fclose(file_bmp);
  result.setBuffer(v_ptr);
  return result;
}//function readBMP

bool isBMP(const unsigned char* header, const size_t length)
{
  if (length<2) return false;
  return ((header[0]=='B') and (header[1]=='M'));
}//function isBMP (second version)
