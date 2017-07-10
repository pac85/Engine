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
#include "vkShaderModule.h"

vkShaderModule::vkShaderModule()
{

}

vkShaderModule::vkShaderModule(const vkDevice &device, string ifile)
{
    vector<char> temp_code = bin_read_file(ifile);

    vkShaderModule(device, temp_code);
}

vkShaderModule::vkShaderModule(const vkDevice &device, vector<char> &code) : shader_module{device.device, vkDestroyShaderModule}
{
    VkShaderModuleCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    create_info.codeSize = code.size();

    std::vector<uint32_t> code_aligned(code.size() / sizeof(uint32_t) + 1);
    memcpy(code_aligned.data(), code.data(), code.size());
    create_info.pCode = code_aligned.data();

    if (vkCreateShaderModule(device.device, &create_info, nullptr, shader_module.replace()) != VK_SUCCESS)
    {
        throw std::runtime_error("failed to create shader module!");
    }
}

vkShaderModule::~vkShaderModule()
{
    //dtor
}
