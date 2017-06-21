#include "Material.h"

Material::Material(vkDevice &device, string ifile) : fragment_shader(device, ifile)
{
    //ctor
}

Material::Material(vkDevice &device, vector<char> &code) : fragment_shader(device, code)
{

}

Material::~Material()
{
    //dtor
}
