#include "camera.h"

#include <iostream>
#include <bgfx/bgfx.h>

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

void Camera::setViewTransform(float ratio, int viewId) const
{
    float view[16];
    // bx::mtxLookAt(view, eye, at);
    bx::mtxLookAt(view, m_pos, bx::add(m_pos, m_front), m_up);

    float proj[16];
    //    const float ratio = float(m_width) / m_height;
    bx::mtxProj(proj, m_fov, ratio, 0.01f, 500.0f,
        bgfx::getCaps()->homogeneousDepth);
    bgfx::setViewTransform(viewId, view, proj);

}
