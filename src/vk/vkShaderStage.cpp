#include "vkShaderStage.h"

vkShaderStage::vkShaderStage(vkShaderModule vertex_shader, vkShaderModule fragment_shader)
{
    VkPipelineShaderStageCreateInfo vertex_shader_index = {};

    vertex_shader_index.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertex_shader_index.stage = VK_SHADER_STAGE_VERTEX_BIT;

}

vkShaderStage::~vkShaderStage()
{
    //dtor
}
