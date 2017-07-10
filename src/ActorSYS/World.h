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
#ifndef WORLD_H
#define WORLD_H
#include"../common.h"
#include"BaseActor.h"
#include"../render/Camera.h"
/*#include"PointLight.h"
#include"SpotLight.h"
#include"DirectionalLight.h"*/


class world
{
    /*struct ComponentAttachment
    {
        enum Attach_type
        {
            relative,               //Uses the attachment offset has an offset to the position of the BaseActor to witch it is attached
            absolute                //Uses the attachment offset has an absolute position
        } attach_type;

        Transform offset;

        Component * attached_ptr;

        string target_socket;
    };*/

    struct WorldActor
    {
        BaseActor * actor_pointer;                          //The BaseActor pointer
        /*bool bAllowTick;                                //If false tick() will not be called even if bTick is true(set by the engine)
        bool skip_tick;                                 //If true tick won't be called but will be set to false again so the next frame it will*/

        inline WorldActor(BaseActor * _actor_pointer)
        {
            actor_pointer = _actor_pointer;
        }
    };

    struct WorldActorIndex
    {
        unsigned int index;

        enum state
        {
            ACTIVE,
            INACTIVE,
            ATTACHED
        }actor_state;

        WorldActorIndex()
        {

        }

        WorldActorIndex(unsigned int _index, state _state)
        {
            index = _index, actor_state = _state;
        }

        inline bool operator== (WorldActorIndex &b)
        {
            return b.index == index;
        }

        inline bool operator!= (WorldActorIndex &b)
        {
            return b.index != index;
        }
    };

    struct attachment
    {
        enum e_attach_type
        {
            RELATIVE,               //Uses the attachment offset has an offset to the position of the BaseActor to witch it is attached
            ABSOLUTE                //Uses the attachment offset has an absolute position
        }attach_type;

        Transform offset;

        WorldActorIndex parent_index,       //An index to the pointer to with the BaseActor is attached
                        attached_index;     //An index to the attached object

        string target_socket;

        bool updated;
        bool skip_update;           //if true an update will be skipped, it will the be set back to false

        inline attachment(WorldActorIndex _parent_index, WorldActorIndex _attached_index)
        {
            parent_index = _parent_index, attached_index = _attached_index;
        }

        inline bool operator == (attachment &b)
        {
            return b.parent_index == parent_index && b.attached_index == attached_index;
        }
    };

    public:
        world();
        virtual ~world();

        void add_actor(BaseActor * actor_to_add);       //adds an BaseActor to the world
        void remove_actor(BaseActor* actor_to_remove);  //removes BaseActor from world


        inline attachment create_attachment(BaseActor* _parent_actor, BaseActor* _attached_actor)
        {
            if(actor_index_map.find(_parent_actor) == actor_index_map.end() || actor_index_map.find(_attached_actor) == actor_index_map.end())
                return ;

            return attachment(actor_index_map[_parent_actor], actor_index_map[_attached_actor]);
        }

        void aa_attach(attachment _attachment); //adds an BaseActor-BaseActor attachment
        void aa_deteach(attachment _attachment);    //removes the attachment

        void load_level(string file_name);          //Loads a level from an xwl file

        void UpdateWorld(float delta);                           //Does all the update stuff


    protected:

    private:

        inline vector<WorldActor>& get_list(WorldActorIndex wai)
        {
            switch(wai.actor_state)
            {
                case WorldActorIndex::ACTIVE:
                    return active_actor_list;
                    break;
                case WorldActorIndex::INACTIVE:
                    return inactive_actor_list;
                    break;
                case WorldActorIndex::ATTACHED:
                    return attached_actor_list;
                    break;
            }
        }

        //Lists of BaseActor, lights get special lists because the world class has to render their shadow maps
        vector<WorldActor> attached_actor_list;
        vector<attachment> attachment_list;
        //vector<attachment> attachments;
        vector<WorldActor> active_actor_list;
        vector<WorldActor> inactive_actor_list;
        map<BaseActor*, WorldActorIndex> actor_index_map;



        void tickAll(float delta_time);                          //Calls tick for every object

        void update_attachments(float delta_time);               //Updates all attachments in the correct order

        //void update_attachment(float delta_time, int index);     //Updates one attachment
};

#endif // WORLD_H
