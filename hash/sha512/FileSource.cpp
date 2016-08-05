/*
 -----------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2012, 2014, 2015  Dirk Stolle

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

#include "FileSource.hpp"
#include <cstring>
#include <stdexcept>

namespace SHA512
{

/* FileSource functions */
FileSource::FileSource()
: MessageSource()
{
}

FileSource::~FileSource()
{
  if (m_Stream.is_open())
  {
    m_Stream.close();
  }
}

bool FileSource::getNextMessageBlock(MessageBlock& mBlock)
{
  std::streamsize bytesRead = 0;
  switch (m_Status)
  {
    case psUnpadded:
         m_Stream.read(reinterpret_cast<char*> (&(mBlock.words[0])), 128);
         bytesRead = m_Stream.gcount();
         if (bytesRead==128)
         {
           m_BitsRead += 1024;
           mBlock.reverseBlock();
           return true;
         }
         //not full block read
         m_BitsRead += (bytesRead * 8);
         //close file, we are done here with reading from file anyway
         m_Stream.close();
         //add 1-bit (start of message padding)
         ((uint8_t*) &(mBlock.words[0]))[bytesRead] = 0x80;
         //zero out rest of message block
         memset(&(((uint8_t*) &(mBlock.words[0]))[bytesRead+1]), 0, 128 - (bytesRead+1));

         //pad size value in there, too, if possible
         if (bytesRead+1<=112)
         {
           #if BYTE_ORDER == LITTLE_ENDIAN
           reverse64(m_BitsRead, m_BitsRead);
           #endif
           mBlock.words[14] = 0;
           mBlock.words[15] = m_BitsRead;
           m_Status = psPaddedAndAllRead;
           mBlock.reverseBlock();
           return true;
         }
         m_Status = psPadded2048And1024Read;
         mBlock.reverseBlock();
         return true;
         break;
    case psPadded2048And1024Read:
         //fill all with zeroes (padding)
         memset(&(mBlock.words[0]), 0, 128);
         //pad size value in there, too
         #if BYTE_ORDER == LITTLE_ENDIAN
         reverse64(m_BitsRead, m_BitsRead);
         #endif
         mBlock.words[14] = 0;
         mBlock.words[15] = m_BitsRead;
         m_Status = psPaddedAndAllRead;
         mBlock.reverseBlock();
         return true;
         break;
    case psPaddedAndAllRead:
         return false;
         break;
    case psPadded1024:
    case psPadded2048:
         //We should never get to this point!
         throw std::logic_error("FileSource::getNextMessageBlock(): Code execution should never get to this point!");
         return false;
         break;
  }//swi
  //We should never get to this point either!
  throw std::logic_error("FileSource::getNextMessageBlock(): Code execution should never get to this point either!");
  return false;
}

bool FileSource::open(const std::string& fileName)
{
  if (m_Stream.is_open()) return false;
  m_Stream.open(fileName.c_str(), std::ios_base::in | std::ios_base::binary);
  if (m_Stream.good())
  {
    m_BitsRead = 0;
    m_Status = psUnpadded;
    return true;
  }
  return false;
}

} //namespace
