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
#ifndef CAMERA_H
#define CAMERA_H

#include"../common.h"
#include <glm/gtc/matrix_transform.hpp>
#include"../ActorSYS/BaseActor.h"

using namespace glm;


class Camera
{
    public:
        Camera(float _fov, float _n_plane, float _f_plane);
        virtual ~Camera();

        inline void set_fov(float f)
        {
            fov = f;
        }

        inline void set_transform(Transform t)
        {
            camera_transform = t;
        }

        mat4 get_camera_matrix(int width, int height);



    protected:

    private:

        Transform camera_transform;
        vec3 up = vec3(0.0f, 1.0f, 0.0f);
        float fov, n_plane, f_plane;
};

#endif // CAMERA_H
