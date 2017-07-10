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
#include"Transform.h"

Transform::Transform()
{
    Transform(vec3(0.0f), vec3(0.0f));
}

Transform::Transform(vec3 _position, vec3 _rotation)
{
    set_position(_position);
    set_rotation(_rotation);
}

Transform Transform::operator+(Transform op)
{
    return Transform(   op.get_position() + position,
                        op.get_rotation() + rotation);
}

void Transform::UpDate_MTransform()
{
    QRotation = quat(rotation);
    mat4 M_rotation = toMat4(QRotation);
    MTransform      = translate(M_rotation, position);
}

void Transform::set_rotation(vec3 _rotation)
{
    rotation = _rotation;
    UpDate_MTransform();
}

void Transform::set_position(vec3 _position)
{
    position = _position;
    UpDate_MTransform();
}

vec3 Transform::get_rotation()
{
    return rotation;
}

quat Transform::get_rotation_quat()
{
    return QRotation;
}

mat4 Transform::get_rotation_matrix()
{
    return toMat4(QRotation);
}

vec3 Transform::get_position()
{
    return position;
}

mat4 Transform::get_transform()
{
    return MTransform;
}
