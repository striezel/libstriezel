/*
 -----------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2015  Dirk Stolle

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

#ifndef LIBSTRIEZEL_SHA1_FILESOURCE_HPP
#define LIBSTRIEZEL_SHA1_FILESOURCE_HPP

#include "../sha256/FileSource.hpp"

namespace SHA1
{
  ///alias for type that is shared with SHA-256
  typedef SHA256::FileSource    FileSource;
} //namespace

#endif // LIBSTRIEZEL_SHA1_FILESOURCE_HPP
