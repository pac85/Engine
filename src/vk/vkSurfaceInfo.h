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
#ifndef VKSWAPCHAINSUPPORTDETAILS_H
#define VKSWAPCHAINSUPPORTDETAILS_H

#include "../common.h"
#include "vkDeleter.hpp"
#include "vkSurface.h"


class vkSurfaceInfo
{
    public:
        vkSurfaceInfo(VkPhysicalDevice device, vkSurface &surface)
        {
            vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface.surface, &capabilities);

            uint32_t formatCount;
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface.surface, &formatCount, nullptr);

            if (formatCount != 0)
            {
                formats.resize(formatCount);
                vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface.surface, &formatCount, formats.data());
            }

            uint32_t presentModeCount;
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface.surface, &presentModeCount, nullptr);

            if (presentModeCount != 0)
            {
                present_modes.resize(presentModeCount);
                vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface.surface, &presentModeCount, present_modes.data());
            }
        }
        virtual ~vkSurfaceInfo() {}

        VkSurfaceCapabilitiesKHR capabilities;
        vector<VkSurfaceFormatKHR> formats;
        vector<VkPresentModeKHR> present_modes;

    protected:

    private:
};

#endif // VKSWAPCHAINSUPPORTDETAILS_H
