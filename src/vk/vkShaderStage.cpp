#include "vkShaderStage.h"

vkShaderStage::vkShaderStage(vkShaderModule shader_module, VkShaderStageFlagBits stage, string mf_name = "main")
{
    VkPipelineShaderStageCreateInfo vertex_shader_index = {};

    vertex_shader_index.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertex_shader_index.stage = stage;
    vertex_shader_index.module = shader_module.get_shader_module();
    vertex_shader_index.pName = mf_name.c_str();

}

vkShaderStage::~vkShaderStage()
{
    //dtor
}
