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

#ifndef LIBTHORO_SHA512_FILESOURCE_HPP
#define LIBTHORO_SHA512_FILESOURCE_HPP

#include <fstream>
#include "MessageSource.hpp"

namespace SHA512
{

/** message source class for files */
class FileSource: public MessageSource
{
  public:
    /** \brief constructor */
    FileSource();


    /** \brief destructor */
    virtual ~FileSource();


    /** \brief puts the next message block from the source in mBlock
     *
     * \param mBlock  reference to the message blocked that should be filled
     * \return  Returns true, if there is at least one more message block.
     *          Returns false and leaves mBlock unchanged, if there are no
     *          more message blocks.
     */
    virtual bool getNextMessageBlock(MessageBlock& mBlock);


    /** \brief Tries to open the given file and prepares for reading from it.
     *
     * \param fileName  name (absolute or relative path) of the file to be opened
     * \return Returns true, if operation was successful and the class can be
     *         used to read from the file. Returns false on failure.
     */
    bool open(const std::string& fileName);
  private:
    std::ifstream m_Stream; /**< internal file stream */
}; //class

} //namespace

#endif // LIBTHORO_SHA512_FILESOURCE_HPP
