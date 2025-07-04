/*
 -----------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2012, 2015, 2025  Dirk Stolle

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

#ifndef LIBSTRIEZEL_SHA256_MESSAGESOURCE_HPP
#define LIBSTRIEZEL_SHA256_MESSAGESOURCE_HPP

#include <cstdint>
#include <sys/types.h>

namespace SHA256
{

#if BYTE_ORDER == LITTLE_ENDIAN
/** Reverses little endian to big endian.
 *
 * \param w   little endian value
 * \param x   variable to store the converted value
 */
void reverse32(const uint32_t w, uint32_t& x);


/** Reverses little endian to big endian.
 *
 * \param w   little endian value
 * \param x   var. to store the converted value
 */
void reverse64(const uint64_t w, uint64_t& x);
#endif

/* MessageBlock structure */
struct MessageBlock
{
  uint32_t words[16];

  void reverseBlock();
}; // struct

/* basic message source class for hashing messages */
class MessageSource
{
  public:
    MessageSource();

    virtual ~MessageSource();

    /** \brief Puts the next message block from the source in mBlock.
     *
     * \param mBlock   reference to the message blocked that should be filled
     * \return Returns true, if there is at least one more message block.
     * Returns false and leaves mBlock unchanged, if there are no more
     * message blocks.
     */
    virtual bool getNextMessageBlock(MessageBlock& mBlock) = 0;
  protected:
    uint64_t m_BitsRead;
    uint8_t * m_PaddingBuffer;
    enum PaddingStatus {psUnpadded, psPadded512, psPadded1024, psPadded1024And512Read, psPaddedAndAllRead};
    PaddingStatus m_Status;
}; // class

} // namespace SHA256

#endif // LIBSTRIEZEL_SHA256_MESSAGESOURCE_HPP
