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
/**************************************************************************/#include "World.h"
#include <stack>

world::world()
{
    //ctor
}

world::~world()
{
    //dtor
}


void world::add_actor(BaseActor * actor_to_add)
{
    if(actor_to_add->bTick)
    {
        active_actor_list.push_back(WorldActor(actor_to_add));
        actor_index_map[actor_to_add] = WorldActorIndex(active_actor_list.size()-1, WorldActorIndex::ACTIVE);
    }
    else
    {
        inactive_actor_list.push_back(WorldActor(actor_to_add));
        actor_index_map[actor_to_add] = WorldActorIndex(inactive_actor_list.size()-1, WorldActorIndex::INACTIVE);
    }
}

void world::remove_actor(BaseActor* actor_to_remove)
{
    WorldActorIndex &actor_index = actor_index_map[actor_to_remove];
    switch(actor_index.actor_state)
    {
        case WorldActorIndex::ACTIVE:
            active_actor_list.erase(active_actor_list.begin()+actor_index.index);
            actor_index_map.erase(actor_to_remove);
            break;

        case WorldActorIndex::INACTIVE:
            inactive_actor_list.erase(inactive_actor_list.begin()+actor_index.index);
            actor_index_map.erase(actor_to_remove);
            break;

        case WorldActorIndex::ATTACHED:
            for(unsigned int i = 0;i < attachment_list.size();i++)
            {
                if(actor_index == attachment_list[i].parent_index || actor_index == attachment_list[i].attached_index)
                {
                    aa_deteach(attachment_list[i]);
                    actor_index = actor_index_map[actor_to_remove];
                }
            }
            actor_index_map.erase(actor_to_remove);
            break;
    }
}

void world::aa_attach(attachment _attachment)
{
    WorldActorIndex &parent_index = _attachment.parent_index,
                    &attached_index = _attachment.attached_index;

    if(parent_index.actor_state != WorldActorIndex::ATTACHED &&
       attached_index.actor_state != WorldActorIndex::ATTACHED)
    {
        vector<WorldActor> & parent_list = get_list(parent_index),      //the vector parent BaseActor belongs to
                           & attached_list = get_list(attached_index);  //the vector attached BaseActor belongs to

        //the BaseActors are pushed in the attached BaseActor list in the right order
        attached_actor_list.push_back(parent_list[parent_index.index]);
        actor_index_map[attached_actor_list[attached_actor_list.size()-1].actor_pointer] = WorldActorIndex(attached_actor_list.size()-1, WorldActorIndex::ATTACHED);
        _attachment.parent_index = WorldActorIndex(attached_actor_list.size()-1, WorldActorIndex::ATTACHED);

        attached_actor_list.push_back(attached_list[attached_index.index]);
        actor_index_map[attached_actor_list[attached_actor_list.size()-1].actor_pointer] = WorldActorIndex(attached_actor_list.size()-1, WorldActorIndex::ATTACHED);
        _attachment.attached_index = WorldActorIndex(attached_actor_list.size()-1, WorldActorIndex::ATTACHED);

        //and removed from the list they belonged to
        parent_list.erase(parent_list.begin()+parent_index.index);
        attached_list.erase(attached_list.begin()+attached_index.index);
    }
    else if(parent_index.actor_state == WorldActorIndex::ATTACHED )
    {
        vector<WorldActor> & attached_list = get_list(attached_index);  //the vector attached BaseActor belongs to
        attached_actor_list.insert(attached_actor_list.begin()+parent_index.index+1, attached_list[attached_index.index]);
        actor_index_map[attached_actor_list[parent_index.index+1].actor_pointer] = WorldActorIndex(parent_index.index+1, WorldActorIndex::ATTACHED);
        _attachment.attached_index = WorldActorIndex(parent_index.index, WorldActorIndex::ATTACHED);
        attached_list.erase(attached_list.begin()+attached_index.index);
    }
    else if(attached_index.actor_state == WorldActorIndex::ATTACHED )
    {
        vector<WorldActor> & parent_list = get_list(parent_index);  //the vector the attached BaseActor belongs to
        attached_actor_list.insert(attached_actor_list.begin()+attached_index.index, parent_list[parent_index.index]);
        actor_index_map[attached_actor_list[attached_index.index].actor_pointer] = WorldActorIndex(attached_index.index, WorldActorIndex::ATTACHED);
        _attachment.parent_index = WorldActorIndex(attached_index.index, WorldActorIndex::ATTACHED);
        parent_list.erase(parent_list.begin()+parent_index.index);
    }
    //otherwise both are already in the list and therefore already attached(that should never appen)

    //Updates the state of the attachment
    parent_index.actor_state = WorldActorIndex::ATTACHED;
    attached_index.actor_state = WorldActorIndex::ATTACHED;

    //and inserts it in the right place inside attachment_list
    for(unsigned int i = 0;;i++)
    {
        if(i >= attachment_list.size())
        {
            attachment_list.push_back(_attachment);
            break;
        }

        if(attachment_list[i].attached_index.index == parent_index.index)
        {
            attachment_list.insert(attachment_list.begin()+i+1, _attachment);
            break;
        }
    }
}

void world::aa_deteach(attachment _attachment)
{
    WorldActorIndex parent_index = _attachment.parent_index,
                    attached_index = _attachment.attached_index;

    //if the BaseActors are not attached there is nothing to do
    if(parent_index.actor_state != WorldActorIndex::ATTACHED ||
       attached_index.actor_state != WorldActorIndex::ATTACHED)
    {
        return;
    }

    //searches and deletes the attachment
    for(unsigned int i = 0;;i++)
    {
        //it returns if it doesn't find it
        if(i >= attachment_list.size())
        {
            return;
        }
        if(attachment_list[i] == _attachment)
        {
            attachment_list.erase(attachment_list.begin()+i);
            break;
        }
    }

    vector<WorldActor> & parent_list = get_list(parent_index),      //the vector parent BaseActor belongs to
                       & attached_list = get_list(attached_index);  //the vector attached BaseActor belongs to

    //if there arent any other attached BaseActor to the parent it is moved to active list
    for(unsigned int i = 0;;i++)
    {
        if(i >= attachment_list.size())
        {
            active_actor_list.push_back(parent_list[parent_index.index]);
            actor_index_map[active_actor_list[active_actor_list.size()-1].actor_pointer] = WorldActorIndex(active_actor_list.size()-1, WorldActorIndex::ACTIVE);
            parent_list.erase(parent_list.begin()+parent_index.index);
            break;
        }

        if(parent_index.index == attachment_list[i].attached_index.index)
        {
            break;
        }
    }
    for(unsigned int i = 0;;i++)
    {
        if(i >= attachment_list.size())
        {
            active_actor_list.push_back(attached_list[attached_index.index]);
            actor_index_map[active_actor_list[active_actor_list.size()-1].actor_pointer] = WorldActorIndex(active_actor_list.size()-1, WorldActorIndex::ACTIVE);
            attached_list.erase(attached_list.begin()+attached_index.index);
            break;
        }

        if(attached_index.index == attachment_list[i].parent_index.index)
        {
            break;
        }
    }
}

void world::load_level(string file_name)
{

}


void world::tickAll(float delta_time)
{
    //Calls tick on every active BaseActor
    for(WorldActor &Actor:active_actor_list)
    {
        Actor.actor_pointer->tick(delta_time);
    }
}

void world::update_attachments(float delta_time)
{
    //avoids crash in case attachment list is empty, otherwise the next piece of code would try to acces the first element
    if(attachment_list.size()<=0)
        return;

    WorldActorIndex last_updated_attached = attachment_list[0].attached_index;
    attached_actor_list[attachment_list[0].parent_index.index].actor_pointer->tick(delta_time);

    for(unsigned int i = 1;i < attachment_list.size();i++)
    {
        if(last_updated_attached != attachment_list[i].parent_index)
            attached_actor_list[attachment_list[i].parent_index.index].actor_pointer->tick(delta_time);

        if(attachment_list[i].attach_type == attachment::ABSOLUTE)
        {
            //just sets the attached BaseActor's transform to be equal to offset
            attached_actor_list[attachment_list[i].attached_index.index].actor_pointer->set_transform(attachment_list[i].offset);
        }
        else if(attachment_list[i].attach_type == attachment::RELATIVE)
        {
            //calculates the sums the parent transform with offset
            Transform temp_transform;
            temp_transform = attached_actor_list[attachment_list[i].parent_index.index].actor_pointer->get_transform() + attachment_list[i].offset;
            attached_actor_list[attachment_list[i].attached_index.index].actor_pointer->set_transform(temp_transform);
        }

        attached_actor_list[attachment_list[i].attached_index.index].actor_pointer->tick(delta_time);

        last_updated_attached = attachment_list[i].attached_index;
    }
}

/*void world::update_attachment(float delta_time, int index)
{
    //calls update and tick
    attachments[index].attached_ptr->update();
    attachments[index].attached_ptr->tick(delta_time);
    //sets skip_tick to avoid that tick is called again
    BaseActor_list[actor_index_map[attachments[index].attached_ptr]].skip_tick = true;
    //Sets updated to true, this is important for the next piece of code to work
    attachments[index].updated = true;

    if(attachments[index].attach_type == attachment::absolute)
    {
        //just sets the attached BaseActor's transform to be equal to offset
        attachments[index].attached_ptr->set_transform(attachments[index].offset);
    }
    else if(attachments[index].attach_type == attachment::relative)
    {
        //calculates the sums the parent transform with offset
        Transform temp_transform;
        temp_transform = attachments[index].attached_ptr->get_transform() + attachments[index].offset;
        attachments[index].attached_ptr->set_transform(temp_transform);
    }
}

void world::update_attachments(float delta_time)
{
    //sets all updated bools to false since this is a new frame
    for(unsigned int i = 0;i < attachments.size(); i++)
    {
        attachments[i].updated = false;
    }

    stack<int> update_stack;
    for(unsigned int i = 0;i < attachments.size(); i++)
    {
        //skip cycle if already updated
        if(attachments[i].updated){continue;}

        update_stack.push(i);

        while(!update_stack.empty())
        {
            //gets the top index on the stack
            int& top_i = update_stack.top();

            if(attached_index_map.find(attachments[top_i].parent_ptr) == attached_index_map.end()) //if it doesn't have a parent(it's root)
            {
                //calls tick
                attachments[i].parent_ptr->tick(delta_time);
                //sets skip_tick to avoid that tick is called again in the tickall function
                BaseActor_list[actor_index_map[attachments[top_i].parent_ptr]].skip_tick = true;
                //updates attached BaseActor
                update_attachment(delta_time, top_i);
                update_stack.pop();
            }
                                 //gets parent index from the map
            else if(attachments[ attached_index_map[attachments[i].parent_ptr] ].updated )   //if the parent was updated
            {
                //updates attached BaseActor
                update_attachment(delta_time, top_i);
                update_stack.pop();

            }
            else
            {
                update_stack.push(attached_index_map[attachments[i].parent_ptr]); //pushes parent so it can be processed later
            }
        }


    }
}*/

void world::UpdateWorld(float delta)
{
    update_attachments(delta);
    tickAll(delta);
}
