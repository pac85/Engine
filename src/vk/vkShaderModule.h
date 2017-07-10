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
#ifndef VKSHADERMODULE_H
#define VKSHADERMODULE_H

#include"../common.h"
#include"vkDevice.h"

class vkShaderModule
{
    public:
        vkShaderModule();
        vkShaderModule(const vkDevice &device, string ifile);
        vkShaderModule(const vkDevice &device, vector<char> &code);
        virtual ~vkShaderModule();

        inline void load_shader(const vkDevice &device, string ifile)
        {
            vkShaderModule(device, ifile);
        }

        inline void load_shader(const vkDevice &device, vector<char> &code)
        {
            vkShaderModule(device, code);
        }

        inline VkShaderModule get_shader_module()
        {
            return shader_module;
        }

    protected:

    private:

        VDeleter<VkShaderModule> shader_module;
};

#endif // VKSHADERMODULE_H
