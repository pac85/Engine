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
#ifndef MODELCOMPONENT_H
#define MODELCOMPONENT_H

#include"../ActorSYS/Component.h"
#include"StaticMesh.h"
#include"Material.h"

class ModelComponent : Component
{
    public:
        ModelComponent(const vkMain &_vk_main, BaseActor * _parent);
        //ModelComponent(ModelComponent &b);
        virtual ~ModelComponent();

        inline Transform get_transform()
        {
            return transform;
        }

        /*const vkMain & vk_main;
        BaseActor *parent;*/
        StaticMesh mesh;
        Material *material;

        VkCullModeFlagBits cull_mode;
        VkFrontFace  front_face;


    protected:

    private:

        Transform transform;
};

#endif // MODELCOMPONENT_H
