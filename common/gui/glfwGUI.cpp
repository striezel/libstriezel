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

#include "glfwGUI.h"
#include <GL/glfw.h>

//static member
glfwGUI* glfwGUI::wrap_ptr = NULL;

glfwGUI::glfwGUI()
{
  m_WindowPosX = 0;
  m_WindowPosY = 0;
  m_NeedsRedisplay = true;
  m_Started = false;
  m_ExitLoop = false;
}

std::string glfwGUI::getType() const
{
  return "GLFW";
}

/* initializes the GUI class for later use and returns true in case of success */
bool glfwGUI::init()
{
  if (wrap_ptr==NULL) wrap_ptr = this;

  return (GL_TRUE==glfwInit());
}

void glfwGUI::setCallbacks()
{
  //assign callback functions
  #warning Not implemented yet!
  /*glutDisplayFunc(drawWrapper);
  glutKeyboardFunc(keyWrapper);
  glutSpecialFunc(specialWrapper);
  glutIdleFunc(idleWrapper);*/
  glfwSetKeyCallback(keyWrapper);
}

void glfwGUI::start()
{
  if (m_Started) return;
  m_Started = true;
  while (!m_ExitLoop)
  {
    glfwPollEvents();
    if (m_NeedsRedisplay)
    {
      m_NeedsRedisplay = false;
      draw();
    }
    else idle();
  }//while
  glfwCloseWindow();
  glfwTerminate();
}

void glfwGUI::terminate()
{
  m_ExitLoop = true;
}

bool glfwGUI::createWindow(const int width, const int height, const int left, const int top, const std::string& title)
{
  if (glfwOpenWindow(width, height, 8, 8, 8, 8, /*RGBA bits*/
                 8, /*8 bit depth buffer*/
                 0, /*stencil buffer bits*/
                 GLFW_WINDOW)==GL_TRUE)
  {
    glfwSetWindowPos(left, top);
    m_WindowPosX = left;
    m_WindowPosY = top;
    glfwSetWindowTitle(title.c_str());
    m_NeedsRedisplay = true;
    return true;
  }
  return false;
}

int glfwGUI::getWindowWidth()
{
  int width = 0;
  glfwGetWindowSize(&width, NULL);
  return width;
}

int glfwGUI::getWindowHeight()
{
  int h = 0;
  glfwGetWindowSize(NULL, &h);
  return h;
}

void glfwGUI::setWindowDimensions(const int width, const int height)
{
  glfwSetWindowSize(width, height);
  m_NeedsRedisplay = true;
}

int glfwGUI::getWindowPosX()
{
  return m_WindowPosX;
}

int glfwGUI::getWindowPosY()
{
  return m_WindowPosY;
}

void glfwGUI::setWindowPosition(const int x, const int y)
{
  glfwSetWindowPos(x, y);
  m_WindowPosX = x;
  m_WindowPosY = y;
}

void glfwGUI::makeFullScreen()
{
  #warning Not implemented yet!
  //glutFullScreen();
}

void glfwGUI::setWindowTitle(const std::string& title)
{
  glfwSetWindowTitle(title.c_str());
}

bool glfwGUI::isLeftKeyCode(const int key)
{
  return (key==GLFW_KEY_LEFT);
}

bool glfwGUI::isRightKeyCode(const int key)
{
  return (key==GLFW_KEY_RIGHT);
}

bool glfwGUI::isESCKeyCode(const int key)
{
  return (key==GLFW_KEY_ESC);
}

bool glfwGUI::isEnterKeyCode(const int key)
{
  return (key==GLFW_KEY_ENTER);
}

bool glfwGUI::isBackspaceKeyCode(const int key)
{
  return (key==GLFW_KEY_BACKSPACE);
}

bool glfwGUI::isDeleteKeyCode(const int key)
{
  return (key==GLFW_KEY_DEL);
}

void glfwGUI::requestRedisplay()
{
  m_NeedsRedisplay = true;
}

void glfwGUI::swapBuffers()
{
  glfwSwapBuffers();
}

void glfwGUI::protectedWriteText(const std::string& text, const float x, const float y, const float z)
{
  #warning Not implemented yet!
}

void glfwGUI::keyWrapper(int Key, int action)
{
  if (action==GLFW_PRESS)
  {
    if (wrap_ptr!=NULL)
    {
      //if ((0<=Key) && (255>=Key))
      //{
        wrap_ptr->keyPressed(Key);
      //}
      //else
      //{
      //  wrap_ptr->specialKeyPressed(Key);
      //}
    }
  }
}
