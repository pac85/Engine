#ifndef VKPHYSICALDEVICE_H
#define VKPHYSICALDEVICE_H
#include "../common.h"
#include "vkDeleter.hpp"
#include "vkInstance.h"
#include "vkQueueFamilies.h"
#include "vkSurface.h"

class vkPhysicalDevice
{
    public:
        vkPhysicalDevice(vkInstance &instance, vkSurface &surface, vector<const char*> * requiered_device_extensions);
        virtual ~vkPhysicalDevice();

        VkPhysicalDevice physical_device = VK_NULL_HANDLE;
        vkQueueFamilies queue_families;
        vector<const char*> device_extensions; //a copy of the input required_device_extensions

    protected:

    private:

        bool select_device
        (
            std::vector<VkPhysicalDevice> &devices,
            VkPhysicalDevice &chosen_device,
            vkQueueFamilies * queue_families,
            vkSurface &surface,
            vector<const char*> * device_extensions
        );
};

#endif // VKPHYSICALDEVICE_H
