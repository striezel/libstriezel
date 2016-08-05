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

#ifndef LIBSTRIEZEL_BITS_HPP
#define LIBSTRIEZEL_BITS_HPP

#include <stdint.h>

// an "array" for up to 16 bits
struct SmallBitArray16
{
  public:
    /* constructor */
    SmallBitArray16();

    /* assignment operator */
    SmallBitArray16& operator=(const SmallBitArray16& other);

    /* member constructor */
    SmallBitArray16(const uint16_t theBits, const uint8_t numberOfBits);

    /* equality operator */
    bool operator==(const SmallBitArray16& other);

    /* returns the number of bits in the array*/
    uint8_t getNumberOfBits() const;

    /* returns the value of the index-th bit.

       parameters:
           index - the zero-based index of the queried bit
    */
    bool getBit(const uint8_t index) const;

    /* tries to set the bit at index-th position to newValue and returns true
       in case of success, or false in case of failure.

       parameters:
           index    - zero-based index of the bit
           newValue - new value of the bit
    */
    bool setBit(const uint8_t index, const bool newValue);

    /* tries to insert a bit at the front of the array and returns true in case
       of success

       parameters:
           newBit - value of the inserted bit
    */
    bool insertBitAtFront(const bool newBit);

    /* tries to append a bit at the end of the array and returns true in case
       of success

       parameters:
           newBit - value of the appended bit
    */
    bool appendBitAtBack(const bool newBit);

    /* tries to append a bit "array" at the end of the array and returns true in
       case of success

       parameters:
           other - the array that has to be appended
    */
    bool appendBitsAtBack(const SmallBitArray16& other);

    /* returns the internal representation of the "array" */
    uint16_t exposeBits() const;
  private:
    uint16_t m_Bits;
    uint8_t m_BitsPresent;
}; //struct

// an "array" for up to 65535 bits
struct LargeBitArray64k
{
  public:
    /* constructor */
    LargeBitArray64k();

    /* member constructor */
    LargeBitArray64k(const unsigned char* theBits, const uint16_t numberOfBits);

    /* assignment operator */
    LargeBitArray64k& operator=(const LargeBitArray64k& other);

    /* destructor */
    ~LargeBitArray64k();

    /* equality operator */
    bool operator==(const LargeBitArray64k& other) const;

    /* equality operator for small bits */
    bool operator==(const SmallBitArray16& right) const;

    /* returns the number of bits in the array*/
    uint16_t getNumberOfBits() const;

    /* returns the value of the index-th bit.

       parameters:
           index - the zero-based index of the queried bit
    */
    bool getBit(const uint16_t index) const;

    /* tries to set the bit at index-th position to newValue and returns true
       in case of success, or false in case of failure.

       parameters:
           index    - zero-based index of the bit
           newValue - new value of the bit
    */
    bool setBit(const uint16_t index, const bool newValue);

    /* tries to insert a bit at the front of the array and returns true in case
       of success

       parameters:
           newBit - value of the inserted bit
    */
    bool insertBitAtFront(const bool newBit);

    /* tries to append a bit at the end of the array and returns true in case
       of success

       parameters:
           newBit - value of the appended bit
    */
    bool appendBitAtBack(const bool newBit);

    /* tries to append a bit "array" at the end of the array and returns true in
       case of success

       parameters:
           other - the array that has to be appended
    */
    bool appendBitsAtBack(const LargeBitArray64k& other);

    static const uint16_t cMaximumBits = 65535;

    /* retrieves a small part (up to 16 bits) of the array as a small array

       parameters:
           startIndex - the index where the sequence starts
           length     - length of the sequence (must not be more than 16)
    */
    SmallBitArray16 getSmallBitSequence(const uint16_t startIndex, const uint8_t length) const;

    /* returns the internally set value of the byte at given index */
    uint8_t exposeByte(const uint16_t byte_index) const;

    /* tries to remove at most count bytes from the start of the array, which
       reduces the array's size for count*8 bits

       parameters:
           count - number of leading bytes to be removed
    */
    void removeLeadingBytes(const uint16_t count);
  private:
    /* returns true, if the currently allocated bytes would allow at least
       one more bit to be stored
    */
    bool hasEnoughSpaceForAnotherBit() const;

    /* returns true, if we could possibly allocate space for at least one more
       bit to be stored
    */
    bool canAllocateMoreSpace() const;

    /* tries to increase the size of the internal array and returns true in case
       of success, false on failure
    */
    bool increaseInternalArray();

    uint8_t * m_Bits;
    uint16_t m_CurrentBytesAllocated;
    uint16_t m_BitsPresent;
}; //struct

#endif // LIBSTRIEZEL_BITS_HPP
