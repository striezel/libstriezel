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

#include "GLfunctions.h"
#include <string>
#if defined(__WIN32__)
  //under windows, some GL constants seem to be defined in another header
  #include <GL/glext.h>
#endif

NPOTSupportType getNPOTSupport()
{
  const std::string cExts = std::string((const char*)glGetString(GL_EXTENSIONS));
  if (cExts.find("GL_ARB_texture_non_power_of_two")!=std::string::npos)
    return npotNPOT;
  if ((cExts.find("GL_ARB_texture_rectangle")!=std::string::npos)
     or (cExts.find("GL_EXT_texture_rectangle")!=std::string::npos)
     or (cExts.find("GL_NV_texture_rectangle")!=std::string::npos))
    return npotRectangle;
  return npotNone;
}

void getNPOTTextureTargets(const NPOTSupportType n, GLenum& tex, GLenum& proxy)
{
  switch (n)
  {
    case npotRectangle:
         #ifndef GL_TEXTURE_RECTANGLE_ARB
         tex = GL_TEXTURE_RECTANGLE_NV;
         proxy = GL_PROXY_TEXTURE_RECTANGLE_NV;
         #else
         tex = GL_TEXTURE_RECTANGLE_ARB;
         proxy = GL_PROXY_TEXTURE_RECTANGLE_ARB;
         #endif
         return;
         break;
    case npotNPOT:
         tex = GL_TEXTURE_2D;
         proxy = GL_PROXY_TEXTURE_2D;
         return;
         break;
    default:
         //no support, no targets
         tex = GL_NONE;
         proxy = GL_NONE;
         return;
         break;
  }//swi
}
