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
  int argcount = 0;
  glutInit(&argcount, NULL);
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

void glutGUI::start()
{
  glutMainLoop();
}

void glutGUI::terminate()
{
  /* glutLeaveMainLoop() would be a nicer, cleaner way, but this function is
     only part of freeglut(?) and not part of the original GLUT implementation.
     So we do an ordinary exit(0) instead. */
  #ifdef APP_USING_FREEGLUT
  glutLeaveMainLoop();
  #else
  exit(0);
  #endif
}

bool glutGUI::createWindow(const int width, const int height, const int left, const int top, const std::string& title)
{
  glutInitWindowPosition(left, top);
  glutInitWindowSize(width, height);
  return (glutCreateWindow(title.c_str())>0);
}

int glutGUI::getWindowWidth()
{
  return glutGet(GLUT_WINDOW_WIDTH);
}

int glutGUI::getWindowHeight()
{
  return glutGet(GLUT_WINDOW_HEIGHT);
}

void glutGUI::setWindowDimensions(const int width, const int height)
{
  glutReshapeWindow(width, height);
}

int glutGUI::getWindowPosX()
{
  return glutGet(GLUT_WINDOW_X);
}

int glutGUI::getWindowPosY()
{
  return glutGet(GLUT_WINDOW_Y);
}

void glutGUI::setWindowPosition(const int x, const int y)
{
  glutPositionWindow(x, y);
}

void glutGUI::makeFullScreen()
{
  glutFullScreen();
}

void glutGUI::setWindowTitle(const std::string& title)
{
  glutSetWindowTitle(title.c_str());
}

void glutGUI::keyWrapper(unsigned char Key, int x, int y)
{
  if (NULL!=wrap_ptr)
    wrap_ptr->keyPressed(Key);
}

void glutGUI::drawWrapper(void)
{
  if (NULL!=wrap_ptr)
    wrap_ptr->draw();
}

void glutGUI::specialWrapper(int Key, int x, int y)
{
  if (NULL!=wrap_ptr)
    wrap_ptr->specialKeyPressed(Key);
}

void glutGUI::idleWrapper(void)
{
  if (NULL!=wrap_ptr)
    wrap_ptr->idle();
}

void glutGUI::requestRedisplay()
{
  glutPostRedisplay();
}

void glutGUI::swapBuffers()
{
  glutSwapBuffers();
}

void glutGUI::protectedWriteText(const std::string& text, const float x, const float y, const float z)
{
  glRasterPos3f(x, y, z);
  const std::string::size_type len = text.length();
  unsigned int i;
  for (i=0; i<len; ++i)
  {
    if (text[i]>=32) /* and (text[i] <=127)*/
      glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text[i]);
  }
}
