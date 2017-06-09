#include "World.h"
#include <stack>

world::world()
{
    //ctor
}

world::~world()
{
    //dtor
}


void world::add_actor(BaseActor * BaseActor_to_add)
{
    if(BaseActor_to_add->bTick)
    {
        active_BaseActor_list.push_back(WorldBaseActor(BaseActor_to_add));
        BaseActor_index_map[BaseActor_to_add] = WorldBaseActorIndex(active_BaseActor_list.size()-1, WorldBaseActorIndex::ACTIVE);
    }
    else
    {
        inactive_BaseActor_list.push_back(WorldBaseActor(BaseActor_to_add));
        BaseActor_index_map[BaseActor_to_add] = WorldBaseActorIndex(inactive_BaseActor_list.size()-1, WorldBaseActorIndex::INACTIVE);
    }
}

void world::remove_actor(BaseActor* BaseActor_to_remove)
{
    WorldBaseActorIndex &BaseActor_index = BaseActor_index_map[BaseActor_to_remove];
    switch(BaseActor_index.BaseActor_state)
    {
        case WorldBaseActorIndex::ACTIVE:
            active_BaseActor_list.erase(active_BaseActor_list.begin()+BaseActor_index.index);
            BaseActor_index_map.erase(BaseActor_to_remove);
            break;

        case WorldBaseActorIndex::INACTIVE:
            inactive_BaseActor_list.erase(inactive_BaseActor_list.begin()+BaseActor_index.index);
            BaseActor_index_map.erase(BaseActor_to_remove);
            break;

        case WorldBaseActorIndex::ATTACHED:
            for(unsigned int i = 0;i < attachment_list.size();i++)
            {
                if(BaseActor_index == attachment_list[i].parent_index || BaseActor_index == attachment_list[i].attached_index)
                {
                    aa_deteach(attachment_list[i]);
                    BaseActor_index = BaseActor_index_map[BaseActor_to_remove];
                }
            }
            BaseActor_index_map.erase(BaseActor_to_remove);
            break;
    }
}

void world::aa_attachment(attachment _attachment)
{
    WorldBaseActorIndex &parent_index = _attachment.parent_index,
                    &attached_index = _attachment.attached_index;

    if(parent_index.BaseActor_state != WorldBaseActorIndex::ATTACHED &&
       attached_index.BaseActor_state != WorldBaseActorIndex::ATTACHED)
    {
        vector<WorldBaseActor> & parent_list = get_list(parent_index),      //the vector parent BaseActor belongs to
                           & attached_list = get_list(attached_index);  //the vector attached BaseActor belongs to

        //the BaseActors are pushed in the attached BaseActor list in the right order
        attached_BaseActor_list.push_back(parent_list[parent_index.index]);
        BaseActor_index_map[attached_BaseActor_list[attached_BaseActor_list.size()-1].BaseActor_pointer] = WorldBaseActorIndex(attached_BaseActor_list.size()-1, WorldBaseActorIndex::ATTACHED);

        attached_BaseActor_list.push_back(attached_list[attached_index.index]);
        BaseActor_index_map[attached_BaseActor_list[attached_BaseActor_list.size()-1].BaseActor_pointer] = WorldBaseActorIndex(attached_BaseActor_list.size()-1, WorldBaseActorIndex::ATTACHED);

        //and removed from the list they belonged to
        parent_list.erase(parent_list.begin()+parent_index.index);
        attached_list.erase(attached_list.begin()+attached_index.index);
    }
    else if(parent_index.BaseActor_state == WorldBaseActorIndex::ATTACHED )
    {
        vector<WorldBaseActor> & attached_list = get_list(attached_index);  //the vector attached BaseActor belongs to
        attached_BaseActor_list.insert(attached_BaseActor_list.begin()+parent_index.index+1, attached_list[attached_index.index]);
        BaseActor_index_map[attached_BaseActor_list[parent_index.index+1].BaseActor_pointer] = WorldBaseActorIndex(parent_index.index+1, WorldBaseActorIndex::ATTACHED);
        attached_list.erase(attached_list.begin()+attached_index.index);
    }
    else if(attached_index.BaseActor_state == WorldBaseActorIndex::ATTACHED )
    {
        vector<WorldBaseActor> & parent_list = get_list(parent_index);  //the vector the attached BaseActor belongs to
        attached_BaseActor_list.insert(attached_BaseActor_list.begin()+attached_index.index, parent_list[parent_index.index]);
        BaseActor_index_map[attached_BaseActor_list[attached_index.index].BaseActor_pointer] = WorldBaseActorIndex(attached_index.index, WorldBaseActorIndex::ATTACHED);
        parent_list.erase(parent_list.begin()+parent_index.index);
    }
    //otherwise both are already in the list and therefore already attached(that should never appen)

    //Updates the state of the attachment
    parent_index.BaseActor_state = WorldBaseActorIndex::ATTACHED;
    attached_index.BaseActor_state = WorldBaseActorIndex::ATTACHED;

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
    WorldBaseActorIndex parent_index = _attachment.parent_index,
                    attached_index = _attachment.attached_index;

    //if the BaseActors are not attached there is nothing to do
    if(parent_index.BaseActor_state != WorldBaseActorIndex::ATTACHED ||
       attached_index.BaseActor_state != WorldBaseActorIndex::ATTACHED)
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

    vector<WorldBaseActor> & parent_list = get_list(parent_index),      //the vector parent BaseActor belongs to
                       & attached_list = get_list(attached_index);  //the vector attached BaseActor belongs to

    //if there arent any other attached BaseActor to the parent it is moved to active list
    for(unsigned int i = 0;;i++)
    {
        if(i >= attachment_list.size())
        {
            active_BaseActor_list.push_back(parent_list[parent_index.index]);
            BaseActor_index_map[active_BaseActor_list[active_BaseActor_list.size()-1].BaseActor_pointer] = WorldBaseActorIndex(active_BaseActor_list.size()-1, WorldBaseActorIndex::ACTIVE);
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
            active_BaseActor_list.push_back(attached_list[attached_index.index]);
            BaseActor_index_map[active_BaseActor_list[active_BaseActor_list.size()-1].BaseActor_pointer] = WorldBaseActorIndex(active_BaseActor_list.size()-1, WorldBaseActorIndex::ACTIVE);
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
    for(WorldBaseActor &BaseActor:active_BaseActor_list)
    {
        BaseActor.BaseActor_pointer->tick(delta_time);

        //Updates all of the attached components
        for(auto &component : BaseActor.component_list)
        {
            component.attached_ptr->update(delta_time);
        }
    }
}

void world::update_attachments(float delta_time)
{
    //avoids crash in case attachment list is empty, otherwise the next piece of code would try to acces the first element
    if(attachment_list.size()<=0)
        return;

    WorldBaseActorIndex last_updated_attached = attachment_list[0].attached_index;
    attached_BaseActor_list[attachment_list[0].parent_index.index].BaseActor_pointer->tick(delta_time);

    for(unsigned int i = 1;attachment_list.size();i++)
    {
        if(last_updated_attached != attachment_list[i].parent_index)
            attached_BaseActor_list[attachment_list[i].parent_index.index].BaseActor_pointer->tick(delta_time);

        if(attachment_list[i].attach_type == attachment::ABSOLUTE)
        {
            //just sets the attached BaseActor's transform to be equal to offset
            attached_BaseActor_list[attachment_list[i].attached_index.index].BaseActor_pointer->set_transform(attachment_list[i].offset);
        }
        else if(attachment_list[i].attach_type == attachment::RELATIVE)
        {
            //calculates the sums the parent transform with offset
            Transform temp_transform;
            temp_transform = attached_BaseActor_list[attachment_list[i].parent_index.index].BaseActor_pointer->get_transform() + attachment_list[i].offset;
            attached_BaseActor_list[attachment_list[i].attached_index.index].BaseActor_pointer->set_transform(temp_transform);
        }

        attached_BaseActor_list[attachment_list[i].attached_index.index].BaseActor_pointer->tick(delta_time);

        last_updated_attached = attachment_list[i].attached_index;
    }
}

/*void world::update_attachment(float delta_time, int index)
{
    //calls update and tick
    attachments[index].attached_ptr->update();
    attachments[index].attached_ptr->tick(delta_time);
    //sets skip_tick to avoid that tick is called again
    BaseActor_list[BaseActor_index_map[attachments[index].attached_ptr]].skip_tick = true;
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
                BaseActor_list[BaseActor_index_map[attachments[top_i].parent_ptr]].skip_tick = true;
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
