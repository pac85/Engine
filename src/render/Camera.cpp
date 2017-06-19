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
