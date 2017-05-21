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

Transform::Transform()
{
    Transform(vec3(0.0f), vec3(0.0f));
}

Transform::Transform(vec3 _position, vec3 _rotation)
{
    set_position(_position);
    set_rotation(_rotation);
}

Transform Transform::operator+(Transform op)
{
    return Transform(   op.get_position() + position,
                        op.get_rotation() + rotation);
}

void Transform::UpDate_MTransform()
{
    QRotation = quat(rotation);
    mat4 M_rotation = toMat4(QRotation);
    MTransform      = translate(M_rotation, position);
}

void Transform::set_rotation(vec3 _rotation)
{
    rotation = _rotation;
    UpDate_MTransform();
}

void Transform::set_position(vec3 _position)
{
    position = _position;
    UpDate_MTransform();
}

vec3 Transform::get_rotation()
{
    return rotation;
}

quat Transform::get_rotation_quat()
{
    return QRotation;
}

mat4 Transform::get_rotation_matrix()
{
    return toMat4(QRotation);
}

vec3 Transform::get_position()
{
    return position;
}

mat4 Transform::get_transform()
{
    return MTransform;
}

/*void Actor::add_socket(Transform S_transform, string S_name)
{
    socket_offset[S_name] = S_transform;
}

Actor::Transform Actor::get_absolute_socket_transform(string S_name)
{
    return socket_offset[S_name] + ATransform;
}*/

void Actor::add_component(Component &_component)
{
    components.push_back(_component);
    component_index[&_component] = components.size()-1;
}
void remove_component(Component _component);

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

