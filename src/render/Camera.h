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
