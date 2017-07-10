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
#include "BaseActor.h"

BaseActor::BaseActor()
{
    //add_socket(Transform(), "root");
}

BaseActor::~BaseActor()
{
    //dtor
}

string BaseActor::get_instance_name()
{
    return string("test");
    return instance_name;
}

void BaseActor::set_instance_name(string _instance_name)
{
    instance_name = _instance_name;
}

Transform BaseActor::get_transform()
{
    return ATransform;
}

void BaseActor::set_transform(Transform _transform)
{
    ATransform = _transform;
}

/*void BaseActor::add_socket(Transform S_transform, string S_name)
{
    socket_offset[S_name] = S_transform;
}

BaseActor::Transform BaseActor::get_absolute_socket_transform(string S_name)
{
    return socket_offset[S_name] + ATransform;
}*/

BaseActor* BaseActorFBaseActory::createInstance(string const& s, string instance_name)
{
    BaseActor_map_type::iterator it = getMap()->find(s);

    if(it == getMap()->end())                       //if this condition is true s is not an element of the map
            return 0;

    //sets instance name
    BaseActor * instancePtr = it->second();
    instancePtr->set_instance_name(instance_name);
    return instancePtr;                            //returns the corresponding BaseActor derived class
}

BaseActorFBaseActory::BaseActor_map_type * BaseActorFBaseActory::BaseActor_name_map = NULL;


BaseActorFBaseActory::BaseActor_map_type * BaseActorFBaseActory::getMap()
{
    if(!BaseActor_name_map)                         //if map is not created yet
    {
         BaseActor_name_map = new BaseActor_map_type;   //create it
    }
    return BaseActor_name_map;                      //return the pointer to map
}

