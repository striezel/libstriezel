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

    /* initializes the GUI class for later use and returns true in case of success */
    virtual bool init();

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

  private:
    static void keyWrapper(unsigned char Key, int x, int y);

    static void drawWrapper(void);

    static void specialWrapper(int Key, int x, int y);

    static void idleWrapper(void);

    static glutGUI* wrap_ptr;
};//class

#endif // RANDOM_THORO_GLUTGUI_H
