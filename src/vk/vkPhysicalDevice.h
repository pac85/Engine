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
