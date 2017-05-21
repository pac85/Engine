#ifndef VKSURFACE_H
#define VKSURFACE_H
#include "../common.h"
#include "vkDeleter.hpp"
#include "vkInstance.h"

class vkSurface
{
    public:
        vkSurface(GLFWwindow* window, vkInstance &instance);
        virtual ~vkSurface();

        VDeleter<VkSurfaceKHR> surface;

    protected:

    private:
};

#endif // VKSURFACE_H
