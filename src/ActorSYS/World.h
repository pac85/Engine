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
    struct ComponentAttachment
    {
        enum Attach_type
        {
            relative,               //Uses the attachment offset has an offset to the position of the BaseActor to witch it is attached
            absolute                //Uses the attachment offset has an absolute position
        } attach_type;

        Transform offset;

        Component * attached_ptr;

        string target_socket;
    };

    struct WorldBaseActor
    {
        BaseActor * BaseActor_pointer;                          //The BaseActor pointer
        vector<ComponentAttachment> component_list;     //List of components attached to the BaseActor
        /*bool bAllowTick;                                //If false tick() will not be called even if bTick is true(set by the engine)
        bool skip_tick;                                 //If true tick won't be called but will be set to false again so the next frame it will*/

        inline WorldBaseActor(BaseActor * _BaseActor_pointer)
        {
            BaseActor_pointer = _BaseActor_pointer;
        }
    };

    struct WorldComponent
    {
        Component * component;

    };

    struct WorldBaseActorIndex
    {
        unsigned int index;

        enum state
        {
            ACTIVE,
            INACTIVE,
            ATTACHED
        }BaseActor_state;

        WorldBaseActorIndex()
        {

        }

        WorldBaseActorIndex(unsigned int _index, state _state)
        {
            index = _index, BaseActor_state = _state;
        }

        inline bool operator== (WorldBaseActorIndex &b)
        {
            return b.index == index;
        }

        inline bool operator!= (WorldBaseActorIndex &b)
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

        WorldBaseActorIndex parent_index,       //An index to the pointer to with the BaseActor is attached
                        attached_index;     //An index to the attached object

        string target_socket;

        bool updated;
        bool skip_update;           //if true an update will be skipped, it will the be set back to false

        inline bool operator == (attachment &b)
        {
            return b.parent_index == parent_index && b.attached_index == attached_index;
        }
    };

    public:
        world();
        virtual ~world();

        void add_actor(BaseActor * BaseActor_to_add);       //adds an BaseActor to the world
        void remove_actor(BaseActor* BaseActor_to_remove);  //removes BaseActor from world

        void aa_attachment(attachment _attachment); //adds an BaseActor-BaseActor attachment
        void aa_deteach(attachment _attachment);    //removes the attachment

        void load_level(string file_name);          //Loads a level from an xwl file

        void UpdateWorld(float delta);                           //Does all the update stuff


    protected:

    private:

        inline vector<WorldBaseActor>& get_list(WorldBaseActorIndex wai)
        {
            switch(wai.BaseActor_state)
            {
                case WorldBaseActorIndex::ACTIVE:
                    return active_BaseActor_list;
                    break;
                case WorldBaseActorIndex::INACTIVE:
                    return inactive_BaseActor_list;
                    break;
                case WorldBaseActorIndex::ATTACHED:
                    return attached_BaseActor_list;
                    break;
            }
        }

        //Lists of BaseActor, lights get special lists because the world class has to render their shadow maps
        vector<WorldBaseActor> attached_BaseActor_list;
        vector<attachment> attachment_list;
        //vector<attachment> attachments;
        vector<WorldBaseActor> active_BaseActor_list;
        vector<WorldBaseActor> inactive_BaseActor_list;
        map<BaseActor*, WorldBaseActorIndex> BaseActor_index_map;



        void tickAll(float delta_time);                          //Calls tick for every object

        void update_attachments(float delta_time);               //Updates all attachments in the correct order

        //void update_attachment(float delta_time, int index);     //Updates one attachment
};

#endif // WORLD_H
