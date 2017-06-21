#ifndef VKSHADERMODULE_H
#define VKSHADERMODULE_H

#include"../common.h"
#include"vkDevice.h"

class vkShaderModule
{
    public:
        vkShaderModule(vkDevice &device, string ifile);
        vkShaderModule(vkDevice &device, vector<char> &code);
        virtual ~vkShaderModule();

        inline VkShaderModule get_shader_module()
        {
            return shader_module;
        }

    protected:

    private:

        VDeleter<VkShaderModule> shader_module;
};

#endif // VKSHADERMODULE_H
