#ifndef TRANSFORM_H_INCLUDED
#define TRANSFORM_H_INCLUDED
#include "../common.h"


#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
using namespace glm;

class Transform
{
    public:
        Transform();
        Transform(vec3, vec3);

        Transform operator+(Transform);

        void set_rotation(vec3 _rotation);
        void set_position(vec3 _position);

        vec3 get_rotation();
        quat get_rotation_quat();
        mat4 get_rotation_matrix();
        vec3 get_position();
        mat4 get_transform();

    private:
        //rotation is in euler angles
        vec3 rotation, position;
        mat4 MTransform;
        quat QRotation;
        //needs to be called every time rotation or position is chamged to keep the matrix in sync
        void UpDate_MTransform();
};

#endif // TRANSFORM_H_INCLUDED
