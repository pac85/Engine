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
#include "engineIn.h"

engineIn::engineIn(GLFWwindow* window)
{
    InWindow = window;
    glfwSetKeyCallback(InWindow, (GLFWkeyfun)&internal_key_callback);   //internal_key_callback will be called when a key will be pressed
}

engineIn::~engineIn()
{
    //dtor
}

void engineIn::UpdateIn(float delta)
{
    map<int, control>::iterator bindings_it;    //iterates through every map's element
    for(bindings_it = key_control_map.begin();bindings_it != key_control_map.end();bindings_it++)
    {
        //if the action(wich can either be pressed or released) is equal to the control trigger action
        if(glfwGetKey( InWindow, bindings_it->first) == bindings_it->second.trigger_action)
        {
            //calls the corresponding function
            (*control_map[bindings_it->second.control_name])(delta, bindings_it->second.value);
        }
    }
}

void engineIn::add_action(string action_name, void (*call_back)())
{
    action_map[action_name] = call_back;
}

void engineIn::add_control(string control_name, void (*call_back)(float,float))
{
    control_map[control_name] = call_back;
}

void engineIn::internal_key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    string action_name = key_action_map[key];    //finds the corresponding action name
    (*action_map[action_name])();                //and uses it to call the corresponding function
}
