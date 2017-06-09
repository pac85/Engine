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

