#ifndef ACTOR_H
#define ACTOR_H
#include "../common.h"
#include "Component.h"
#include "Transform.h"

using namespace glm;


#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#define REGISTER_DEC_TYPE(NAME) \
    static ActorRegister<NAME> reg

#define REGISTER_DEF_TYPE(NAME) \
    ActorRegister<NAME> NAME::reg(#NAME)

class Actor
{
    public:

        Actor();
        virtual ~Actor();

        string get_instance_name();
        void set_instance_name(string _instance_name);

        Transform get_transform();
        void set_transform(Transform _transform);

        /*//socket management
        void add_socket(Transform, string);      //adds a socket to the actor
        //returns the absolute transform of the socket
        Transform get_absolute_socket_transform(string);*/

        //events
        virtual void update() = 0;                   //called every frame, before tick
        virtual void tick(float delta) = 0;      //called every frame, after update
        virtual void GameStart() = 0;            //called when the game starts

        bool bTick;                              //if true tick() will be called every frame

    protected:
        Transform ATransform;

    private:
        //map<string, Transform>socket_offset;     //An array of sockets' relative transforms
        string instance_name;
};

/*
 *The following code allows a actor derived class instance to be created from an assigned name
 *Some of the code was taken from here:
 *http://stackoverflow.com/questions/582331/is-there-a-way-to-instantiate-objects-from-a-string-holding-their-class-name
*/

template<typename T> Actor * createT()
{
    return new T;
}

struct ActorFactory
{
    typedef map<std::string, Actor*(*)()> Actor_map_type;

    public:
        static Actor* createInstance(string const& s,    //Creates an instance of the actor the witch name is s
                                      string instance_name);
                                                        //returns 0 if there is no actor named s.

    protected:

        static Actor_map_type * getMap();               //returns the map witch associates names with actors

    private:
        static Actor_map_type * actor_name_map;         //the map witch associates names with actors
};

template<typename T>
struct ActorRegister : ActorFactory                     //Every actor derived class must have a static member of
{                                                       //this type and the constructor's argument is the name
    ActorRegister(string const& s)                      //that will be used  when the class is referenced by string.
    {
        getMap()->insert(make_pair(s, &createT<T>));
    }
};




#endif // ACTOR_H
