#ifndef WORLD_H
#define WORLD_H
#include"../common.h"
#include"Actor.h"
#include"../render/Camera.h"
/*#include"PointLight.h"
#include"SpotLight.h"
#include"DirectionalLight.h"*/


class world
{

    struct attachment
    {
        enum Attach_type
        {
            relative,               //Uses the attachment offset has an offset to the position of the actor to witch it is attached
            absolute                //Uses the attachment offset has an absolute position
        } attach_type;

        Transform offset;

        Actor * parent_ptr;         //A pointer to the pointer to with the actor is attached
        Actor * attached_ptr;       //A pointer to the attached object

        string target_socket;

        bool updated;
        bool skip_update;           //if true an update will be skipped, it will the be set back to false
    };

    struct ComponentAttachment
    {
        enum Attach_type
        {
            relative,               //Uses the attachment offset has an offset to the position of the actor to witch it is attached
            absolute                //Uses the attachment offset has an absolute position
        } attach_type;

        Transform offset;

        Component * attached_ptr;

        string target_socket;
    };

    struct WorldActor
    {
        Actor * actor_pointer;                          //The Actor pointer
        vector<ComponentAttachment> component_list;    //List of components attached to the actor
        bool bAllowTick;                                //If false tick() will not be called even if bTick is true(set by the engine)
        bool skip_tick;                                 //If true tick won't be called but will be set to false again so the next frame it will
    };

    struct WorldComponent
    {
        Component * component;

    };

    enum Light_type
    {
        Point,
        Directional,
        Spot
    };

    struct WorldLight
    {
        //Pointers to the various types of lights(only one of these will be used but all kinds of pointers are needed)
        union
        {
            /*PointLight* PLight_pointer;
            SpotLight*  SLight_pointer;
            DirectionalLight* DLight_pointer;*/
        };
        //also stores an actor type pointer
        Actor* ALight_pointer;
        //Stores the light type (can be either Point, Directional or Spot)
        Light_type light_type;
        //Set by the engine based on whether the light can be seen
        bool bDraw;
        //Set by the engine
        bool bDrawShadowMap;
    };



    public:
        world();
        virtual ~world();

        void add_actor(Actor * actor_to_add);       //adds an actor to the world

        void remove_actor(Actor* actor_to_remove);  //removes actor from world

        Actor* get_actor_ptr(string instance_name); //returns a pointe to the actor, NULL if instance_name isn't in the map

        void load_level(string file_name);          //Loads a level from an xwl file



    protected:

    private:
        //Lists of actor, lights get special lists because the world class has to render their shadow maps
        vector<WorldActor> actor_list;
        vector<WorldLight> light_list;
        map<Actor*, int>   actor_index_map;
        map<string, int>   Actor_instance_index_map;
        map<Actor*, int>   light_index_map;
        map<string, int>   Light_instance_index_map;

        vector<attachment> attachments;                          //A list of attachments
        map<Actor*, int>   attached_index_map;                   //Maps attached actor to the bounding index



        void tickAll(float delta_time);                          //Calls tick for every object

        void update_attachments(float delta_time);               //Updates all attachments in the correct order

        void update_attachment(float delta_time, int index);     //Updates one attachment

        void UpdateWorld(float delta);                           //the function of the main rendering thread

};

#endif // WORLD_H
