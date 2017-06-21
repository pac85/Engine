#ifndef MATERIAL_H
#define MATERIAL_H

#include"../vk/vkShaderModule.h"

class Material
{
    public:
        Material(vkDevice &device, string ifile);
        Material(vkDevice &device, vector<char> &code);
        virtual ~Material();

        vkShaderModule fragment_shader;

    protected:

    private:
};

#endif // MATERIAL_H
