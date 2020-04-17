#include "camera.h"

#include <iostream>

static const float g_maxFov = 120.0f;
static const float g_minFov = 1.0f;

Camera::Camera(bx::Vec3 pos)
    : m_pos(std::move(pos))
{
}

void Camera::move(Camera::Direction direction, float distance)
{
    switch (direction) {
    case FRONT:
        m_pos = bx::add(m_pos, bx::mul(m_front, distance));
        break;

    case UP:
        m_pos = bx::add(m_pos, bx::mul(m_up, distance));
        break;

    case RIGHT:
        m_pos = bx::add(m_pos, bx::mul(m_right, distance));
        break;
    }
}

void Camera::zoom(float offset)
{
    if (m_fov >= 1.0f && m_fov <= g_maxFov)
        m_fov -= offset;
    if (m_fov <= g_minFov)
        m_fov = g_minFov;
    if (m_fov >= g_maxFov)
        m_fov = g_maxFov;
}
