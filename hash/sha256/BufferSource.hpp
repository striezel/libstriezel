/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's common code library.
    Copyright (C) 2012, 2015  Thoronador

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

#ifndef LIBTHORO_SHA256_BUFFERSOURCE_HPP
#define LIBTHORO_SHA256_BUFFERSOURCE_HPP

#include "MessageSource.hpp"

namespace SHA256
{

/* message source class for buffers */
class BufferSource: public MessageSource
{
  public:
    /* constructor */
    BufferSource(uint8_t* data, const uint64_t data_length_in_bits);

    /* destructor */
    virtual ~BufferSource();

    /* puts the next message block from the source in mBlock and returns true,
       if there is at least one more message block. Returns false and leaves
       mBlock unchanged, if there are no more message blocks.

       parameters:
           mBlock - reference to the message blocked that should be filled
    */
    virtual bool getNextMessageBlock(MessageBlock& mBlock);
  private:
    uint8_t * m_BufferPointer;
    uint64_t m_BufferSize;
}; //class

} //namespace

#endif // LIBTHORO_SHA256_BUFFERSOURCE_HPP
