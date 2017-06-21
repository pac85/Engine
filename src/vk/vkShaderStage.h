#ifndef VKSHADERSTAGE_H
#define VKSHADERSTAGE_H

#include"vkShaderModule.h"

class vkShaderStage
{
    public:
        vkShaderStage(vkShaderModule shader_module, VkShaderStageFlagBits stage, string mf_name = "main");
        virtual ~vkShaderStage();

    protected:

    private:
};

#endif // VKSHADERSTAGE_H
