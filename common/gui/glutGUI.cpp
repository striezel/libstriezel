/*
 -----------------------------------------------------------------------------
    This file is part of the Thoronador's random stuff.
    Copyright (C) 2012  thoronador

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

#include "glutGUI.h"
#include "IncludeGLUT.h"

//static member
glutGUI* glutGUI::wrap_ptr = NULL;

/* initializes the GUI class for later use and returns true in case of success */
bool glutGUI::init()
{
  if (wrap_ptr==NULL) wrap_ptr = this;
  glutInit(NULL, NULL);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
  #ifdef APP_USING_FREEGLUT
  //If we use freeglut API instead of the usual one, we like to return from the
  //main loop instead of exiting without prior notice.
  glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
  #endif
  //assign callback functions
  glutDisplayFunc(drawWrapper);
  glutKeyboardFunc(keyWrapper);
  glutSpecialFunc(specialWrapper);
  glutIdleFunc(idleWrapper);
  return true;
}

void glutGUI::terminate()
{
  #ifdef APP_USING_FREEGLUT
  glutLeaveMainLoop();
  #endif
}

bool glutGUI::createWindow(const int width, const int height, const int left, const int top, const std::string& title)
{
  glutInitWindowPosition(left, top);
  glutInitWindowSize(width, height);
  return (glutCreateWindow(title.c_str())>0);
}

void glutGUI::keyWrapper(unsigned char Key, int x, int y)
{
  wrap_ptr->keyPressed(Key);
}

void glutGUI::drawWrapper(void)
{
  wrap_ptr->draw();
}

void glutGUI::specialWrapper(int Key, int x, int y)
{
  wrap_ptr->specialKeyPressed(Key);
}

void glutGUI::idleWrapper(void)
{
  wrap_ptr->idle();
}
