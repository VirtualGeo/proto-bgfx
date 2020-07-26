#include "camera.h"

#include <bgfx/bgfx.h>
#include <entry/windowstate.h>
#include <iostream>
#include <cassert>

static const float g_maxFov = 120.0f;
static const float g_minFov = 1.0f;

Camera::Camera(bx::Vec3 pos)
    : m_pos(std::move(pos))
{
}

//Camera::Camera(Camera &&camera)
//{
//#ifdef DEBUG
//    std::cout << "\033[34m";
//    std::cout << "[Camera] " << this << "  right moving from " << &camera << std::endl;
//    std::cout << "\033[0m";
//#endif
//    assert(false);

//}

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

//    const float side = 230.0f;
//    bx::mtxOrtho(proj, -side, side, -side, side, 0.01f, 500.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);

    bgfx::setViewTransform(viewId, view, proj);

}

void Camera::setViewTransform(const View& view) const
{
    float viewMtx[16];
    // bx::mtxLookAt(view, eye, at);
    bx::mtxLookAt(viewMtx, m_pos, bx::add(m_pos, m_front), m_up);

    float projMtx[16];
    //    const float ratio = float(m_width) / m_height;

    if (view.projection == Projection::PERSPECTIVE) {
        bx::mtxProj(projMtx, m_fov, view.ratio, 0.01f, 500.0f,
            bgfx::getCaps()->homogeneousDepth);

    } else if (view.projection == Projection::ORTHOGRAPHIC) {
        const float side = 230.0f;
        bx::mtxOrtho(projMtx, -side, side, -side, side, 0.01f, 500.0f, 0.0f, bgfx::getCaps()->homogeneousDepth);
    }
    else {
        assert(false);
    }

    bgfx::setViewTransform(view.id, viewMtx, projMtx);
}

void Camera::setPos(const bx::Vec3 &&pos)
{
    m_pos = std::move(pos);
//    m_pos = pos;
}
