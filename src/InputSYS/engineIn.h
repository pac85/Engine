/**************************************************************************/
/*
  Copyright (C) 2017 Antonino Maniscalco (alias pac85)

  This file is part of Engine.

  Engine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Engine is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Engine.  If not, see http://www.gnu.org/licenses/

*/
/**************************************************************************/
#ifndef ENGINEIO_H
#define ENGINEIO_H

#include "../common.h"


class engineIn
{
    public:

        struct control
        {
            string control_name;
            int value;
            int trigger_action;
        };

        engineIn(GLFWwindow* window);
        virtual ~engineIn();

        void UpdateIn(float delta); /*should be called every time the input system
                                      has to read all the inputs(usually every frame)*/

        void add_action(string action_name, void (*call_back)());
        void add_control(string control_name, void (*call_back)(float,float));

    protected:

    private:

        GLFWwindow* InWindow;

        map<string, void (*)()> action_map;                 //maps a name to a function
        map<int, string> key_action_map;                    //maps a key to an action name
        map<string, void (*)(float, float)> control_map;    //maps a name to a function
        map<int, control> key_control_map;                  //maps a key to a control name

        void internal_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
};

#endif // ENGINEIO_H
