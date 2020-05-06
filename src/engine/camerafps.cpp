#include "camerafps.h"
#include <iostream>
//#include <bx/math.h>

CameraFps::CameraFps(bx::Vec3 pos, bx::Vec3 target)
    : Camera(pos)
    , m_spotLight(bx::Vec3{0.0, 1.0, 0.0}, pos) // random value for direction
{
    m_type = FPS;

    float view[16];
    bx::mtxLookAt(view, pos, target);
    bx::Vec3 dof = bx::Vec3{view[2], view[6], view[10]};
    m_yaw = bx::toDeg(bx::atan2(dof.z, dof.x));
    m_pitch = bx::toDeg(bx::asin(dof.y));
//    m_pitch = bx::toDeg(bx::asin(dof.z));


    updateCameraFront();
}

void CameraFps::mouseMove(float xoffset, float yoffset)
{
    m_yaw += xoffset;
    m_pitch += yoffset;

    // make sure that when g_pitch is out of bounds, screen doesn't get flipped
    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    if (m_pitch < -89.0f)
        m_pitch = -89.0f;
    //    bx::Vec3 front;
    //    front.x = bx::cos(bx::toRad(g_yaw)) * bx::cos(bx::toRad(g_pitch));
    //    front.y = bx::sin(bx::toRad(g_pitch));
    //    front.z = bx::sin(bx::toRad(g_yaw)) * bx::cos(bx::toRad(g_pitch));
    //    g_cameraFront = bx::normalize(front);
    updateCameraFront();
}

void CameraFps::move(Camera::Direction direction, float distance)
{
    Camera::move(direction, distance);

    m_spotLight.updatePos(m_pos);
}

void CameraFps::updateCameraFront()
{

    bx::Vec3 front;
    front.x = bx::cos(bx::toRad(m_yaw)) * bx::cos(bx::toRad(m_pitch));
    front.y = bx::sin(bx::toRad(m_pitch));
    front.z = bx::sin(bx::toRad(m_yaw)) * bx::cos(bx::toRad(m_pitch));
    m_front = bx::normalize(front);
    m_right = bx::normalize(bx::cross(m_up, m_front));

    m_spotLight.updateDirection(m_front);
}
