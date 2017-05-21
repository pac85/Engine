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
