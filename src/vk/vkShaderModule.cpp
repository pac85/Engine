#include "vkShaderModule.h"

vkShaderModule::vkShaderModule(vkDevice &device, string ifile)
{
    vector<char> temp_code = bin_read_file(ifile);

    vkShaderModule(device, temp_code);
}

vkShaderModule::vkShaderModule(vkDevice &device, vector<char> &code) : shader_module{device.device, vkDestroyShaderModule}
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
