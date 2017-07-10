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
#include "StaticMesh.h"

using namespace Assimp;

StaticMesh::StaticMesh()
{
    if(command_pool == nullptr)
        throw runtime_error("StaticMesh::command_pool is not set");

    if(device == nullptr)
        throw runtime_error("StaticMesh::device is not set");
}

StaticMesh::~StaticMesh()
{
    //dtor
}

VkCommandPool * StaticMesh::command_pool = nullptr;
vkDevice * StaticMesh::device = nullptr;


/*void load_model(const string &file, unsigned int ai_flags = 0aiProcess_CalcTangentSpace | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType)
{
    Importer importer;

    const ai_scene* scene = importer.ReadFile(file, ai_flags);

    if(!ai_scene)
    {
        throw std::runtime_error("failed to load file " + file);
        return;
    }



}*/

void StaticMesh::load_model(aiMesh mesh)
{
    if(mesh.mNormals == nullptr)
    {
        cout << "mesh with no normals" << endl;
        return;
    }

    //Gets the space in bytes that the vertex data occupates, also used for normals
    VkDeviceSize buffer_size =  mesh.mNumVertices * sizeof(aiVector3D);

    //creates the vertex buffer
    create_filled_buffer_staged( device,
                                 *command_pool,
                                 buffer_size,
                                 mesh.mVertices,
                                 VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                 vertex_buffer,
                                 vertex_buffer_memory);
    //creates the normal buffer
    create_filled_buffer_staged( device,
                                 *command_pool,
                                 buffer_size,
                                 mesh.mNormals,
                                 VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                 normal_buffer,
                                 normal_buffer_memory);

    //creates all the UV buffers
    uv_buffers.clear();
    uv_buffers_memory.clear();

    vector<glm::vec2> uv_coords(mesh.mNumVertices);
    buffer_size = mesh.mNumVertices * sizeof(glm::vec2);

    for(int i = 0;mesh.mTextureCoords[i];i++) //iterates through uv maps until it finds a null one
    {
        for(unsigned int j = 0;j < mesh.mNumVertices; j++)
        {
            uv_coords[i] = glm::vec2(mesh.mTextureCoords[i][j].x, mesh.mTextureCoords[i][j].y);
        }

        uv_buffers.push_back(VDeleter<VkBuffer>{device->device, vkDestroyBuffer});
        uv_buffers_memory.push_back(VDeleter<VkDeviceMemory>{device->device, vkFreeMemory});

        create_filled_buffer_staged( device,
                                     *command_pool,
                                     buffer_size,
                                     uv_coords.data(),
                                     VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                     uv_buffers.back(),
                                     uv_buffers_memory.back());
    }

    uv_coords.clear();

    //creates the index buffer
    vector<unsigned int> indices;
    for(unsigned int i = 0; i < mesh.mNumFaces;i++)
    {
        if(mesh.mFaces[i].mNumIndices == 3) //non triangular faces are discarded
        {
            indices.push_back(mesh.mFaces[i].mIndices[0]);
            indices.push_back(mesh.mFaces[i].mIndices[1]);
            indices.push_back(mesh.mFaces[i].mIndices[2]);
        }
    }

    buffer_size = indices.size() * sizeof(unsigned int);

    create_filled_buffer_staged( device,
                                 *command_pool,
                                 buffer_size,
                                 indices.data(),
                                 VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                                 index_buffer,
                                 index_buffer_memory);
}
