#ifndef VKDEVICE_H
#define VKDEVICE_H

#include "../common.h"
#include "vkDeleter.hpp"
#include "vkPhysicalDevice.h"
#include "vkQueues.h"


class vkDevice
{
    public:
        vkDevice(vkInstance &instance, vkPhysicalDevice &_physical_device, vector<const char*> * device_extensions);
        virtual ~vkDevice();

        vkPhysicalDevice * physical_device;
        VDeleter<VkDevice> device{vkDestroyDevice};
        vkQueues queues;

        inline void wait_idle()
        {
            vkDeviceWaitIdle(device);
        }

    protected:

    private:
};

#endif // VKDEVICE_H
