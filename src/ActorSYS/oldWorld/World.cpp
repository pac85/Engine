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


void world::add_actor(Actor * actor_to_add)
{
    //Otherwise is another kind of actor
    WorldActor temp;
    temp.actor_pointer = actor_to_add;
    //Allows ticking by default
    temp.bAllowTick = true;
    actor_list.push_back(temp);
    Actor_instance_index_map[actor_to_add->get_instance_name()] =
    actor_index_map[actor_to_add] = actor_list.size() - 1;
}

void world::remove_actor(Actor* actor_to_remove)
{
    //erases the actor from actor_list and from actor_index_map
    int actor_index = actor_index_map[actor_to_remove];
    actor_index_map.erase(actor_to_remove);
    actor_list.erase(actor_list.begin()+actor_index);

    //rebuilds the actor_index_map(the actor_list vector has changed so it's necessary to rebuild it)
    actor_index_map.clear();
    Actor_instance_index_map.clear();
    for(unsigned int i = 0;i < actor_list.size();i++)
    {
        Actor_instance_index_map[actor_list[i].actor_pointer->get_instance_name()] =
        actor_index_map[actor_list[i].actor_pointer] = i;
    }
}

Actor* world::get_actor_ptr(string instance_name)
{
    if(Actor_instance_index_map.find(instance_name) != Actor_instance_index_map.end())
    {
        return actor_list[Actor_instance_index_map[instance_name]].actor_pointer;
    }
    else
    {
        return NULL;
    }
}

void world::load_level(string file_name)
{

}


void world::tickAll(float delta_time)
{
    //Calls update on every actor which is allowed to tick and which has the bTick bool set to true
    for(unsigned int i = 0;i < actor_list.size();i++)
    {
        if (actor_list[i].actor_pointer->bTick && actor_list[i].bAllowTick )
        {
            if(!actor_list[i].skip_tick)
            {
                actor_list[i].actor_pointer->update();
            }
        }
    }
    //Calls tick on every actor which is allowed to tick and which has the bTick bool set to true
    for(unsigned int i = 0;i < actor_list.size();i++)
    {
        if (actor_list[i].actor_pointer->bTick && actor_list[i].bAllowTick )
        {
            if(!actor_list[i].skip_tick)
            {
                actor_list[i].actor_pointer->tick(delta_time);
            }
            else
            {
                actor_list[i].skip_tick = false;
            }
        }

        //Updates all of the attached components
        for(auto &component : actor_list[i].component_list)
        {
            component.attached_ptr->update(delta_time);
        }
    }
}

void world::update_attachment(float delta_time, int index)
{
    //calls update and tick
    attachments[index].attached_ptr->update();
    attachments[index].attached_ptr->tick(delta_time);
    //sets skip_tick to avoid that tick is called again
    actor_list[actor_index_map[attachments[index].attached_ptr]].skip_tick = true;
    //Sets updated to true, this is important for the next piece of code to work
    attachments[index].updated = true;

    if(attachments[index].attach_type == attachment::absolute)
    {
        //just sets the attached actor's transform to be equal to offset
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
                actor_list[actor_index_map[attachments[top_i].parent_ptr]].skip_tick = true;
                //updates attached actor
                update_attachment(delta_time, top_i);
                update_stack.pop();
            }
                                 /********gets parent index from the map********/
            else if(attachments[ attached_index_map[attachments[i].parent_ptr] ].updated )   //if the parent was updated
            {
                //updates attached actor
                update_attachment(delta_time, top_i);
                update_stack.pop();

            }
            else
            {
                update_stack.push(attached_index_map[attachments[i].parent_ptr]); //pushes parent so it can be processed later
            }
        }


    }
}

void world::UpdateWorld(float delta)
{
    update_attachments(delta);
    tickAll(delta);
}
