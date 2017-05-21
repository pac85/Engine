#ifndef VKMAIN_H
#define VKMAIN_H

#include "vkInstance.h"
#include "vkSurface.h"
#include "vkPhysicalDevice.h"
#include "vkDevice.h"
#include "vkSwapChain.h"
#include "vkShaderModule.h"

#include "../common.h"


class vkMain
{
    public:
        vkMain( VkApplicationInfo app_info,
                GLFWwindow * window,
                vector<const char*> device_extensions,
                VkSurfaceFormatKHR surface_format,
                VkPresentModeKHR present_mode,
                VkExtent2D extent);


        virtual ~vkMain();

        inline vkInstance get_instance()
        {
            return *instance;
        }

        inline vkSurface get_surface()
        {
            return *surface;
        }

        inline vkPhysicalDevice get_physical_device()
        {
            return *physical_device;
        }

        inline vkDevice get_device()
        {
            return *device;
        }

        inline vkSwapChain get_swap_chain()
        {
            return *swap_chain;
        }

        void recreate_swap_chain(VkSurfaceFormatKHR surface_format, VkPresentModeKHR present_mode, VkExtent2D extent);

    protected:

    private:

        vkInstance * instance;
        vkSurface * surface;
        vkPhysicalDevice * physical_device;
        vkDevice * device;
        vkSwapChain * swap_chain;
};

#endif // VKMAIN_H
