#include "camera.h"

#include <iostream>

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
