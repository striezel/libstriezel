/*
 -----------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2012, 2015  Dirk Stolle

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

#ifndef LIBSTRIEZEL_SHA256_BUFFERSOURCE_HPP
#define LIBSTRIEZEL_SHA256_BUFFERSOURCE_HPP

#include "MessageSource.hpp"

namespace SHA256
{

/** message source class for buffers */
class BufferSource: public MessageSource
{
  public:
    /** \brief constructor
     *
     * \param data   pointer to the buffer (must not be null);
     *  The buffer pointer must not be freed or changed during the lifetime of
     *  the BufferSource object, because it uses the data directly instead of
     *  copying it for internal use.
     * \param data_length_in_bits  length of the buffer in bits(!);
     *  will be rounded up to the next multiple of eight, because the
     *  implementation is byte-oriented.
     */
    BufferSource(uint8_t* data, const uint64_t data_length_in_bits);


    /** \brief destructor */
    virtual ~BufferSource();


    /** \brief puts the next message block from the source in mBlock
     *
     * \param mBlock  reference to the message blocked that should be filled
     * \return Returns true, if there is at least one more message block.
     *  Returns false and leaves mBlock unchanged, if there are no more
     *  message blocks.
     */
    virtual bool getNextMessageBlock(MessageBlock& mBlock);
  private:
    uint8_t * m_BufferPointer; /**< pointer to the buffer */
    uint64_t m_BufferSize; /**< size of the buffer in bytes */
}; //class

} //namespace

#endif // LIBSTRIEZEL_SHA256_BUFFERSOURCE_HPP
