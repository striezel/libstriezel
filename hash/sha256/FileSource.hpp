/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's common code library.
    Copyright (C) 2012 thoronador

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

#ifndef LIBTHORO_SHA_256_SOURCES_HPP
#define LIBTHORO_SHA_256_SOURCES_HPP

#include <fstream>
#include "MessageSource.hpp"

namespace SHA256
{

/* message source class for files */
class FileSource: public MessageSource
{
  public:
    /* constructor */
    FileSource();

    /* destructor */
    virtual ~FileSource();

    /* puts the next message block from the source in mBlock and returns true,
       if there is at least one more message block. Returns false and leaves
       mBlock unchanged, if there are no more message blocks.

       parameters:
           mBlock - reference to the message blocked that should be filled
    */
    virtual bool getNextMessageBlock(MessageBlock& mBlock);

    /* tries to open the given file and prepares for reading from it. Returns
       true, if operation was successful and the class can be used to read from
       the file. Returns false on failure.

       parameters:
           fileName - name (absolute or relative path) of the file to be opened
    */
    bool open(const std::string& fileName);
  private:
    std::ifstream m_Stream;
}; //class

} //namespace

#endif // LIBTHORO_SHA_256_SOURCES_HPP
