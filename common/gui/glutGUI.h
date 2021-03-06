#ifndef RANDOM_THORO_GLUTGUI_H
#define RANDOM_THORO_GLUTGUI_H

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

#include "BasicGUI.h"

class glutGUI: public BasicGUI
{
  public:
    /* constructor */
    glutGUI() {}

    /* destructor */
    virtual ~glutGUI() {}

    /* returns a string identifying the GUI's type */
    virtual std::string getType() const;

    /* initializes the GUI class for later use and returns true in case of success */
    virtual bool init();

    /* sets the callback functions for GLUT */
    void setCallbacks();

    /* starts the GUI loop */
    virtual void start();

    /* terminates the GUI, i.e. cleans up all created resources */
    virtual void terminate();

    /* tries to create a new window and returns true in case of success

       parameters:
           width  - width of the window in pixels
           height - height of the window in pixels
           left   - distance from left screen border in pixels
           top    - distance from top screen border in pixels
           title  - title of the window
    */
    virtual bool createWindow(const int width, const int height, const int left, const int top, const std::string& title);

    /* gets the width of the current window in pixels*/
    virtual int getWindowWidth();

    /* gets the height of the current window in pixels*/
    virtual int getWindowHeight();

    /* sets the dimensions of the current window

       parameters:
           width  - requested window width
           height - requested window height
    */
    virtual void setWindowDimensions(const int width, const int height);

    /* gets the current window's distance from the left screen border in pixles */
    virtual int getWindowPosX();

    /* gets the current window's distance from the top screen border in pixles */
    virtual int getWindowPosY();

    /* sets the position of the current window

       parameters:
           x - requested distance from the left screen border in pixles
           y - requested distance from the top screen border in pixles
    */
    virtual void setWindowPosition(const int x, const int y);

    /* requests that the current window shall be made full screen */
    virtual void makeFullScreen();

    /* sets the title of the current window */
    virtual void setWindowTitle(const std::string& title);

    /* returns true, if the integer value in key represents the GUI's code for
       the left arrow key

       parameters:
           key - the key code
    */
    virtual bool isLeftKeyCode(const int key);

    /* returns true, if the integer value in key represents the GUI's code for
       the right arrow key

       parameters:
           key - the key code
    */
    virtual bool isRightKeyCode(const int key);

    /* returns true, if the integer value in key represents the GUI's code for
       the escape key

       parameters:
           key - the key code
    */
    virtual bool isESCKeyCode(const int key);

    /* returns true, if the integer value in key represents the GUI's code for
       the enter key

       parameters:
           key - the key code
    */
    virtual bool isEnterKeyCode(const int key);

    /* returns true, if the integer value in key represents the GUI's code for
       the backspace key

       parameters:
           key - the key code
    */
    virtual bool isBackspaceKeyCode(const int key);

    /* returns true, if the integer value in key represents the GUI's code for
       the delete key

       parameters:
           key - the key code
    */
    virtual bool isDeleteKeyCode(const int key);

    /* requests that the current window will be redisplayed */
    virtual void requestRedisplay();

    /* swap front and back buffers */
    virtual void swapBuffers();

  protected:
    /* writes out a text at the given location */
    static void protectedWriteText(const std::string& text, const float x, const float y, const float z);

  private:
    static void keyWrapper(unsigned char Key, int x, int y);

    static void drawWrapper(void);

    static void specialWrapper(int Key, int x, int y);

    static void idleWrapper(void);

    static glutGUI* wrap_ptr;
};//class

#endif // RANDOM_THORO_GLUTGUI_H
