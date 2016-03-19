/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
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

#ifndef LIBTHORO_GLFUNCTIONS_HPP
#define LIBTHORO_GLFUNCTIONS_HPP

#include <GL/gl.h>

enum NPOTSupportType {npotNone, npotNPOT, npotRectangle};

/* returns the support type of non-power of two textures in OpenGL */
NPOTSupportType getNPOTSupport();

/* returns true, if the GL implementation supports a form of NPOT texture sizes */
inline bool hasNPOTSupport()
{
  return (getNPOTSupport()!=npotNone);
}

/* returns the appropriate texture target for the given NPOT type

   parameters:
       n     - type of support for NPOT-sized textures
       tex   - reference to a GLenum that shall be set to the appropriate texture
               target
       proxy - reference to a GLenum that shall be set to the appropriate proxy
               texture target
*/
void getNPOTTextureTargets(const NPOTSupportType n, GLenum& tex, GLenum& proxy);

#endif // LIBTHORO_GLFUNCTIONS_HPP
