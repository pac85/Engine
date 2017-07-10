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
#include "Camera.h"

Camera::Camera(float _fov, float _n_plane, float _f_plane)
{
    fov = _fov, n_plane = _n_plane, f_plane = _f_plane;
}

Camera::~Camera()
{
    //dtor
}

mat4 Camera::get_camera_matrix(int width, int height)
{
    return perspective(glm::radians(fov/2.0f), (float) width / (float)height, n_plane, f_plane) *
    lookAt(camera_transform.get_position(), camera_transform.get_rotation(), up);
}
