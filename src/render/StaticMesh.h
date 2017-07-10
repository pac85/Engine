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
//note: by "StaticMesh" I mean a non-animated mesh , it can still be moved and retated around

#ifndef MODEL_H
#define MODEL_H

#include "../common.h"
#include "vk/vkUtils.h"
#include "vk/vkDeleter.hpp"
#include "vk/vkDevice.h"
#include "vk/vkPhysicalDevice.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags


class StaticMesh
{
    public:
        StaticMesh();
        virtual ~StaticMesh();

        /*//loads a model from the file specified in the first parameter, the second optional parameter specifies assimp flags
        void load_model(const string &file, unsigned int ai_flags = 0aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType);*/
        void load_model(aiMesh mesh);

        static VkCommandPool * command_pool;
        static vkDevice * device;

        VDeleter<VkBuffer> vertex_buffer{device->device, vkDestroyBuffer};
        VDeleter<VkDeviceMemory> vertex_buffer_memory{device->device, vkFreeMemory};

        VDeleter<VkBuffer> normal_buffer{device->device, vkDestroyBuffer};
        VDeleter<VkDeviceMemory> normal_buffer_memory{device->device, vkFreeMemory};

        vector<VDeleter<VkBuffer>> uv_buffers;
        vector<VDeleter<VkDeviceMemory>> uv_buffers_memory;

        VDeleter<VkBuffer> index_buffer{device->device, vkDestroyBuffer};
        VDeleter<VkDeviceMemory> index_buffer_memory{device->device, vkFreeMemory};

        /*enum attributes_modes
        {
            NO_NORM_UV_0    = 0,
            NO_NORM_UV_1    = 1,
            NO_NORM_UV_2    = 2,
            UV_0            = 3,
            UV_1            = 4,
            UV_2            = 5,

        }attributes_mode;

        enum instancing_modes
        {
            NON_INSTANCED_ONLY,
            INSTANCED_ONLY,
            INSTANCEABLE
        }instancing_mode;


        const VkVertexInputBindingDescription binding_description[2][6] =
        {
            {
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX),
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX),
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX),
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX),
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX),
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX)
            },

            {
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX),
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX),
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX),
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX),
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX),
                make_VkVertexInputBindingDescription(0, 0, VK_VERTEX_INPUT_RATE_VERTEX)
            }
        };*/

    protected:

    private:

};

#endif // MODEL_H
