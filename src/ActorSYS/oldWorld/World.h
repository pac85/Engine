/**************************************************************************/

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
                                                                  
                                                                      
/**************************************************************************/#ifndef WORLD_H
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
        vector<ComponentAttachment> component_list;     //List of components attached to the actor
        bool bAllowTick;                                //If false tick() will not be called even if bTick is true(set by the engine)
        bool skip_tick;                                 //If true tick won't be called but will be set to false again so the next frame it will
    };

    struct WorldComponent
    {
        Component * component;

    };

    public:
        world();
        virtual ~world();

        void add_actor(Actor * actor_to_add);       //adds an actor to the world

        void remove_actor(Actor* actor_to_remove);  //removes actor from world

        Actor* get_actor_ptr(string instance_name); //returns a pointe to the actor, NULL if instance_name isn't in the map

        void load_level(string file_name);          //Loads a level from an xwl file

        void UpdateWorld(float delta);                           //Does all the update stuff


    protected:

    private:
        //Lists of actor, lights get special lists because the world class has to render their shadow maps
        vector<WorldActor> actor_list;
        map<Actor*, int>   actor_index_map;
        map<string, int>   Actor_instance_index_map;
        vector<attachment> attachments;                          //A list of attachments
        map<Actor*, int>   attached_index_map;                   //Maps attached actor to the bounding index



        void tickAll(float delta_time);                          //Calls tick for every object

        void update_attachments(float delta_time);               //Updates all attachments in the correct order

        void update_attachment(float delta_time, int index);     //Updates one attachment
};

#endif // WORLD_H
