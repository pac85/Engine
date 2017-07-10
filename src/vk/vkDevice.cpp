/**************************************************************************/
/*
  Copyright (C) 2017 Antonino Maniscalco (alias pac85)

  This file is part of Engine.

  Engine is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Engine is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with Engine.  If not, see http://www.gnu.org/licenses/

*/
/**************************************************************************/
#include "vkDevice.h"

vkDevice::vkDevice(vkInstance &instance, vkPhysicalDevice &_physical_device, vector<const char*> * device_extensions)
{
    physical_device = &_physical_device;
    //Generates and fills a VkDeviceQueueCreateInfo for every queue family required
    vector<VkDeviceQueueCreateInfo> queue_create_infos;
    set<int> unique_queue_family{physical_device->queue_families.graphics.index,
                                 physical_device->queue_families.present.index};

    float queue_priority = 1;
    for(int queue_family : unique_queue_family)
    {
        VkDeviceQueueCreateInfo queue_create_info = {};
        queue_create_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        queue_create_info.queueFamilyIndex = queue_family;
        queue_create_info.queueCount = 1;
        queue_create_info.pQueuePriorities = &queue_priority;
        queue_create_infos.push_back(queue_create_info);
    }

    //Features
    VkPhysicalDeviceFeatures device_features = {};


    //Device
    VkDeviceCreateInfo create_info = {};

    create_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    //Queues
    create_info.pQueueCreateInfos = queue_create_infos.data();
    create_info.queueCreateInfoCount = (uint32_t) queue_create_infos.size();
    //features
    create_info.pEnabledFeatures = &device_features;
    //validation layers
    if(instance.using_validation_layers())
    {
        create_info.enabledLayerCount = instance.get_validation_layers()->size();
        create_info.ppEnabledLayerNames = instance.get_validation_layers()->data();
    }
    else
    {
        create_info.enabledLayerCount = 0;
    }
    //Extensions
    create_info.enabledExtensionCount = physical_device->device_extensions.size();
    create_info.ppEnabledExtensionNames = physical_device->device_extensions.data();

    if (vkCreateDevice(physical_device->physical_device, &create_info, nullptr, device.replace()) != VK_SUCCESS)
    {
        cout << "failed to create logical device!" << endl;
    }

    //Gets the queue's handle
    vkGetDeviceQueue(device, physical_device->queue_families.graphics.index, 0, &queues.graphics.queue);
    vkGetDeviceQueue(device, physical_device->queue_families.present.index, 0, &queues.present.queue);
    vkGetDeviceQueue(device, physical_device->queue_families.transfer.index, 0, &queues.transfer.queue);
}

vkDevice::~vkDevice()
{
    //dtor
}
