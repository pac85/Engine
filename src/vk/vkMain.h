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
