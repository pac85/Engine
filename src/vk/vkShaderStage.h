#ifndef VKSHADERSTAGE_H
#define VKSHADERSTAGE_H

#include"vkShaderModule.h"

class vkShaderStage
{
    public:
        vkShaderStage(vkShaderModule vertex_shader, vkShaderModule fragment_shader);
        virtual ~vkShaderStage();

    protected:

    private:
};

#endif // VKSHADERSTAGE_H
