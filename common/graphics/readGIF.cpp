/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2011, 2012  thoronador

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
#include "gif/GIFStructures.h"
#include "gif/Decoder.h"

GLImageStructure readGIF(const std::string& FileName)
{
  #warning Not implemented yet!
  GLImageStructure result;
  #warning Premature return!
  return result; //remove this line later, as soon as function is completely implemented

  GIF file_gif;
  if (!file_gif.readFromFile(FileName, true))
  {
    std::cout << "Error: could not read GIF file \""<<FileName<<"\".\n";
    return result;
  }
  //GIF is read, now check the elements
  if (file_gif.getElements().empty())
  {
    std::cout << "Error: GIF file \""<<FileName<<"\" does not contain any image data!\n";
    return result;
  }
  GIFElementBase * elem = NULL;
  elem = file_gif.getElements().back();
  if (!elem->isTableBasedImage())
  {
    std::cout << "Error: the last read element in \""<<FileName<<"\" is not a table based image.\n";
    return result;
  }
  GIFTableBasedImage * tableBasedImage = static_cast<GIFTableBasedImage*>(elem);
  const GIFImageDescriptor& idesc = tableBasedImage->getImageDescriptor();
  std::cout << "Image descriptor of first image:\n"
            << "  left:  "<<idesc.getLeftPosition()<<"\n"
            << "  top:   " << idesc.getTopPosition()<<"\n"
            << "  width: "<<idesc.getWidth()<<"\n"
            << "  height: "<<idesc.getHeight()<<"\n"
            << "  has local table: "<<idesc.getLocalColourTableFlag()<<"\n"
            << "     size: "<<(int) idesc.getSizeOfLocalColourTable()<<"\n"
            << "  has glob. table: "<<file_gif.getLogicalScreenDescriptor().getColourTableFlag()<<"\n"
            << "     size: "<<(int) file_gif.getLogicalScreenDescriptor().getSizeOfGlobalColourTable()<<"\n"
            << "  interlaced: "<<idesc.getInterlaceFlag()<<"\n"
            << "  min. code size: "<<(int)tableBasedImage->getImageData().getMinCodeSize()<<"\n"
            << "  data sub-blocks: "<<tableBasedImage->getImageData().getNumberOfSubBlocks()<<"\n";
  if (!(idesc.getLocalColourTableFlag() or file_gif.getLogicalScreenDescriptor().getColourTableFlag()))
  {
    std::cout << "Error: First image in file \""<<FileName<<"\" does not have a"
              << " local colour table, and there is no global table either.\n";
    return result;
  }

  const GIFColourTable& colourTable = idesc.getLocalColourTableFlag() ?
          tableBasedImage->getLocalColourTable() : file_gif.getGlobalColourTable();

  decode(colourTable, *tableBasedImage);

  #warning To be continued.
  #warning Not completely implemented yet!
  std::cout << "Error: Function readGIF() is incomplete. Wait for next commit to git repository. :p\n";
  return result;
}

bool isGIF(const unsigned char* header, const size_t length)
{
  if (length<6) return false;
  if ((header[0]=='G') and (header[1]=='I') and (header[2]=='F')
      and (header[3]=='8') and (header[5]=='a'))
  {
    //we are quite close here, just check the one character that can differ
    return ((header[4]=='7') or (header[4]=='9'));
  }
  return false;
}//function isGIF
