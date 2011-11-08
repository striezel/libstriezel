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

#include "readGIF.h"
#include <iostream>
#include <cstdio>
#include <cmath>
#include <cstring>

GLImageStructure readGIF(const std::string& FileName)
{
  #warning Not implemented yet!
  GLImageStructure result;
  FILE *file_gif = fopen(FileName.c_str(), "rb");
  if (!file_gif)
  {
    //file could not be opened for reading
    return result;
  }
  //general reading buffer
  char buffer[255];
  memset(buffer, 0, 255);
  //read signature and version (3 bytes each)
  if (fread(buffer, 1, 6, file_gif)!=6)
  {
    //file is not long enough to be a GIF file
    fclose(file_gif);
    return result;
  }
  //check file signature
  if ((buffer[0]!='G') or (buffer[1]!='I') or (buffer[2]!='F'))
  {
    std::cout << "Error: File \""<<FileName<<"\" does not have a GIF signature.\n";
    fclose(file_gif);
    return result;
  }
  //check version
  if ((buffer[3]!='8') or (buffer[5]!='a') or ((buffer[4]!='7') and (buffer[4]!='9')))
  {
    std::cout << "Error:  File \""<<FileName<<"\" has invalid or unknown GIF version \""
              << buffer[3] << buffer[4] << buffer[5] << "\", only 87a and 89a"
              << " are allowed.\n";
    fclose(file_gif);
    return result;
  }
  /* set GIF version (not sure if we need this, because we plan to be able to
     read both versions) */
  const int GIF_version = (buffer[4]=='7') ? 87 : 89;
  /*read Logical Screen Descriptor (7 bytes)

    contents:
        Logical Screen Width    - unsigned (16 bits)
        Logical Screen Height   - unsigned (16 bits)
        Packed Fields           - byte
        Background Colour Index - byte
        Pixel Aspect Ration     - byte
  */

  uint16_t screenWidth, screenHeight;
  //-- read screen width
  if (fread(&screenWidth, 2, 1, file_gif)!=1)
  {
    std::cout << "Error: Could not read logical screen width value from file!\n";
    fclose(file_gif);
    return result;
  }
  //-- read screen width
  if (fread(&screenHeight, 2, 1, file_gif)!=1)
  {
    std::cout << "Error: Could not read logical screen height value from file!\n";
    fclose(file_gif);
    return result;
  }
  uint8_t packedFields, backgroundColourIndex, PixelAspectRatio;
  //-- read packed fields
  if (fread(&packedFields, 1, 1, file_gif)!=1)
  {
    std::cout << "Error: Could not read packed fields value from file!\n";
    fclose(file_gif);
    return result;
  }
  //-- read bg colour index
  if (fread(&backgroundColourIndex, 1, 1, file_gif)!=1)
  {
    std::cout << "Error: Could not read background colour index from file!\n";
    fclose(file_gif);
    return result;
  }
  //-- read pixel aspect ratio
  if (fread(&PixelAspectRatio, 1, 1, file_gif)!=1)
  {
    std::cout << "Error: Could not read pixel aspect ratio from file!\n";
    fclose(file_gif);
    return result;
  }
  //split packed fields
  const bool globalColourTableFlag = (packedFields & (1<<7) != 0);
  const uint8_t colourResolution = ((packedFields & ((1<<6) | (1<<5) | (1<<4))) >> 4);
  const bool sortFlag = (packedFields & (1<<3) != 0);
  const uint8_t sizeOfGlobalColourTable = (packedFields & (1 | (1<<1) | (1<<2)));

  char globalColourTable[255*3];
  memset(globalColourTable, 0, 255*3);

  //check for global colour table and read it, if present
  if (globalColourTableFlag)
  {
    //global table is present
    //the table follows immediately after the logical screen descriptor, so read it
    const unsigned int actualColourTableSize =3* pow(2, sizeOfGlobalColourTable+1);
    //read it at once
    if (fread(globalColourTable, 1, actualColourTableSize, file_gif)!=actualColourTableSize)
    {
      std::cout << "Error: Unable to read global colour table of file \""<<FileName<<"\"!\n";
      fclose(file_gif);
      return result;
    }
  }//if global colour table is present

  /*read image descriptor (required)

    structure:
        Image Separator     - byte
        Image Left Position - unsigned (16 bit)
        Image Top Position  - unsigned (16 bit)
        Image Width         - unsigned (16 bit)
        Image Height        - unsigned (16 bit)
        Packed Fields       - byte
  */

  uint8_t imageSep = 0;
  //-- read image separator
  if (fread(&imageSep, 1, 1, file_gif)!=1)
  {
    std::cout << "Error: Could not read image separator from file!\n";
    fclose(file_gif);
    return result;
  }
  //check for proper value
  if (imageSep!=0x2C)
  {
    std::cout << "Error: Image separator has incorrect value!\n";
    fclose(file_gif);
    return result;
  }

  uint16_t imageLeftPos, imageTopPos;
  //-- read image left position
  if (fread(&imageLeftPos, 2, 1, file_gif)!=1)
  {
    std::cout << "Error: Could not read image left position from file!\n";
    fclose(file_gif);
    return result;
  }
  //-- read image top position
  if (fread(&imageTopPos, 2, 1, file_gif)!=1)
  {
    std::cout << "Error: Could not read image top position from file!\n";
    fclose(file_gif);
    return result;
  }

  uint16_t imageWidth, imageHeight;
  //-- read image width
  if (fread(&imageWidth, 2, 1, file_gif)!=1)
  {
    std::cout << "Error: Could not read image width from file!\n";
    fclose(file_gif);
    return result;
  }
  //-- read image height
  if (fread(&imageHeight, 2, 1, file_gif)!=1)
  {
    std::cout << "Error: Could not read image height from file!\n";
    fclose(file_gif);
    return result;
  }
  //--read packed fields
  if (fread(&packedFields, 1, 1, file_gif)!=1)
  {
    std::cout << "Error: Could not read packed field in image descriptor from file!\n";
    fclose(file_gif);
    return result;
  }
  // split packed fields
  const bool localColourTableFlag = ((packedFields >> 7)!=0);
  const bool interlaceFlag = ((packedFields & (1<<6))!=0);
  const bool second_sortFlag = ((packedFields & (1<<5))!=0);
  const uint16_t sizeOfLocalColourTable = (packedFields & (1 | (1<<1) | (1<<2)));

  //local colour table may follow
  if ((!localColourTableFlag) and (!globalColourTableFlag))
  {
    std::cout << "Error: File contains neither global nor local colour table!\n";
    fclose(file_gif);
    return result;
  }//if

  unsigned char localColourTable[255*3];
  memset(localColourTable, 0, 255*3);
  //check for local colour table
  if (localColourTable)
  {
    //read local colour table
    const unsigned int actualColourTableSize =3* pow(2, sizeOfLocalColourTable+1);
    //read it at once
    if (fread(localColourTable, 1, actualColourTableSize, file_gif)!=actualColourTableSize)
    {
      std::cout << "Error: Unable to read local colour table of file \""<<FileName<<"\"!\n";
      fclose(file_gif);
      return result;
    }


  }//if local colour table

  #warning To be continued.
  #warning Not completely implemented yet!
  std::cout << "Error: Function readGIF() is incomplete. Wait for next commit to SVN. :p\n";
  return result;
}

bool isGIF(const std::string& FileName)
{
  FILE *file_gif = fopen(FileName.c_str(), "rb");
  if (!file_gif)
  {
    //file could not be opened for reading
    return false;
  }

  char header[6];
  memset(header, 0, 6);
  //read first sixt bytes
  if (fread(header, 1, 6, file_gif)!=6)
  {
    //file is not long enough to be a GIF file
    fclose(file_gif);
    return false;
  }
  fclose(file_gif);
  if ((header[0]=='G') and (header[1]=='I') and (header[2]=='F')
      and (header[3]=='8') and (header[5]=='a'))
  {
    //we are quite close here, just check the one character that can differ
    return ((header[4]=='7') or (header[4]=='9'));
  }
  return false;
}//function isGIF
