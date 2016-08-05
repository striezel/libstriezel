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

#ifndef LIBSTRIEZEL_GIFELEMENTBASE_HPP
#define LIBSTRIEZEL_GIFELEMENTBASE_HPP

struct GIFElementBase
{
  public:
    /* constructor */
    GIFElementBase();

    /* destructor */
    virtual ~GIFElementBase();

    virtual bool isExtension() const = 0;
    virtual bool isTableBasedImage() const = 0;
};//struct

#endif // LIBSTRIEZEL_GIFELEMENTBASE_HPP
