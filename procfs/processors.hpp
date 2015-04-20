/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's common code library.
    Copyright (C) 2015  Thoronador

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

#ifndef LIBTHORO_PROCESSORS_HPP
#define LIBTHORO_PROCESSORS_HPP

namespace libthoro
{

namespace procfs
{

/** \brief determines the number of processors on the current machine
 *
 * \return returns the number of processors, if successful. Returns -1 on failure.
 */
int getProcessorCount();

} //namespace procfs

} //namespace libthoro

#endif // LIBTHORO_PROCESSORS_HPP
