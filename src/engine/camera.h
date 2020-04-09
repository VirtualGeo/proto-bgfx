#ifndef CAMERA_H
#define CAMERA_H

#include <bx/file.h>

class Camera
{
public:
    enum Direction {
        FRONT,
        RIGHT,
        UP
    };

//public:
    Camera(bx::Vec3 pos);

    void move(Direction direction, float distance);
//private:
    bx::Vec3 m_pos;
    float m_fov = 60.0f;
    bx::Vec3 m_front;
    bx::Vec3 m_right;
    bx::Vec3 m_up = {0.0f, 1.0f, 0.0f};
};

#endif // CAMERA_H
