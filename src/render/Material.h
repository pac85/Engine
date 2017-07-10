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
#ifndef MATERIAL_H
#define MATERIAL_H

#include"../vk/vkMain.h"

class Material
{
    public:
        Material(vkMain &_vk_main, string ifile);
        Material(vkMain &_vk_main, vector<char> &code);
        virtual ~Material();

        inline void load_shader(vkMain &vk_main, string ifile)
        {
            //fragment_shader.load_shader(vk_main.get_device(), ifile);
        }

        inline void load_shader(vkMain &vk_main, vector<char> &code)
        {
            //fragment_shader.load_shader(vk_main.get_device(), code);
        }

        vkShaderModule fragment_shader;
        /*VDeleter<VkDescriptorSetLayout> descriptor_set_layout{vk_main.get_device().device, vkDestroyDescriptorSetLayout};
        VDeleter<VkBuffer> uniform_buffer;
        VDeleter<VkDeviceMemory> uniform_buffer_memory;*/


    protected:

    private:
        vkMain &vk_main;
};

#endif // MATERIAL_H
