/*
 -----------------------------------------------------------------------------
    This file is part of the striezel's common code library.
    Copyright (C) 2011, 2015  Dirk Stolle

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

#ifndef GIFCONSTANTS_H
#define GIFCONSTANTS_H

#include <cstdint>

// constants
const uint8_t cGIFImageSeparator = 0x2C;
const uint8_t cGIFExtensionIntroducer = 0x21;
// ---- graphic control extension
const uint8_t cGIFGraphicControlLabel = 0xF9;
// ---- comment extension
const uint8_t cGIFCommentLabel = 0xFE;
// ---- plain text extension
const uint8_t cGIFPlainTextLabel = 0x01;
// ---- application extension
const uint8_t cGIFApplicationExtensionLabel = 0xFF;
const uint8_t cGIFTrailer = 0x3B;

//note: most of them should possibly go into the Extensions.h/.cpp

#endif // GIFCONSTANTS_H
