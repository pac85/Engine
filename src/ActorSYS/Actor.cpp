#include "Actor.h"

Actor::Actor()
{
    //add_socket(Transform(), "root");
}

Actor::~Actor()
{
    //dtor
}

string Actor::get_instance_name()
{
    return instance_name;
}

void Actor::set_instance_name(string _instance_name)
{
    instance_name = _instance_name;
}

Transform Actor::get_transform()
{
    return ATransform;
}

void Actor::set_transform(Transform _transform)
{
    ATransform = _transform;
}

/*void Actor::add_socket(Transform S_transform, string S_name)
{
    socket_offset[S_name] = S_transform;
}

Actor::Transform Actor::get_absolute_socket_transform(string S_name)
{
    return socket_offset[S_name] + ATransform;
}*/

Actor* ActorFactory::createInstance(string const& s, string instance_name)
{
    Actor_map_type::iterator it = getMap()->find(s);

    if(it == getMap()->end())                       //if this condition is true s is not an element of the map
            return 0;

    //sets instance name
    Actor * instancePtr = it->second();
    instancePtr->set_instance_name(instance_name);
    return instancePtr;                            //returns the corresponding Actor derived class
}

ActorFactory::Actor_map_type * ActorFactory::actor_name_map = NULL;


ActorFactory::Actor_map_type * ActorFactory::getMap()
{
    if(!actor_name_map)                         //if map is not created yet
    {
         actor_name_map = new Actor_map_type;   //create it
    }
    return actor_name_map;                      //return the pointer to map
}

