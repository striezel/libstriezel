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

#ifndef RANDOM_THORO_BASICGUI_H
#define RANDOM_THORO_BASICGUI_H

#include <string>

//class BasicGUI: provides a common interface for GUI classes
class BasicGUI
{
  public:
    /* constructor */
    BasicGUI() {}

    /* destructor */
    virtual ~BasicGUI() {}

    /* initializes the GUI class for later use and returns true in case of success */
    virtual bool init() = 0;

    /* starts the GUI loop */
    virtual void start() = 0;

    /* terminates the GUI, i.e. cleans up all created resources */
    virtual void terminate() = 0;

    /* tries to create a new window and returns true in case of success

       parameters:
           width  - width of the window in pixels
           height - height of the window in pixels
           left   - distance from left screen border in pixels
           top    - distance from top screen border in pixels
           title  - title of the window
    */
    virtual bool createWindow(const int width, const int height, const int left, const int top, const std::string& title) = 0;

    /* gets the width of the current window in pixels */
    virtual int getWindowWidth() = 0;

    /* gets the width of the current window in pixels*/
    virtual int getWindowHeight() = 0;

    /* sets the dimensions of the current window

       parameters:
           width  - requested window width
           height - requested window height
    */
    virtual void setWindowDimensions(const int width, const int height) = 0;

    /* gets the current window's distance from the left screen border in pixles */
    virtual int getWindowPosX() = 0;

    /* gets the current window's distance from the top screen border in pixles */
    virtual int getWindowPosY() = 0;

    /* sets the position of the current window

       parameters:
           x - requested distance from the left screen border in pixles
           y - requested distance from the top screen border in pixles
    */
    virtual void setWindowPosition(const int x, const int y) = 0;

    /* requests that the current window shall be made full screen */
    virtual void makeFullScreen() = 0;

    /* sets the title of the current window */
    virtual void setWindowTitle(const std::string& title) = 0;

    /* function that will be called whenever a key is pressed

       parameters:
           key - the pressed key or generated character
    */
    virtual void keyPressed(int key) = 0;

    /* function that will be called whenever a special key (e.g. system key) is pressed

       parameters:
           key - the pressed key
    */
    //virtual void specialKeyPressed(int key) = 0;

    /* returns true, if the integer value in key represents the GUI's code for
       the left arrow key

       parameters:
           key - the key code
    */
    virtual bool isLeftKeyCode(const int key) = 0;

    /* returns true, if the integer value in key represents the GUI's code for
       the right arrow key

       parameters:
           key - the key code
    */
    virtual bool isRightKeyCode(const int key) = 0;

    /* returns true, if the integer value in key represents the GUI's code for
       the escape key

       parameters:
           key - the key code
    */
    virtual bool isESCKeyCode(const int key) = 0;

    /* returns true, if the integer value in key represents the GUI's code for
       the enter key

       parameters:
           key - the key code
    */
    virtual bool isEnterKeyCode(const int key) = 0;

    /* returns true, if the integer value in key represents the GUI's code for
       the backspace key

       parameters:
           key - the key code
    */
    virtual bool isBackspaceKeyCode(const int key) = 0;

    /* returns true, if the integer value in key represents the GUI's code for
       the delete key

       parameters:
           key - the key code
    */
    virtual bool isDeleteKeyCode(const int key) = 0;

    /* function that draws a frame */
    virtual void draw(void) = 0;

    /* function that will be called when the GUI/application idles */
    virtual void idle(void) = 0;

    /* requests that the current window will be redisplayed */
    virtual void requestRedisplay() = 0;

    /* swap front and back buffers */
    virtual void swapBuffers() = 0;
};//class

#endif // RANDOM_THORO_BASICGUI_H
