#ifndef CAMERA_H
#define CAMERA_H

#include <bx/file.h>
#include <vector>
#include <memory>
#include <entry/windowstate.h>

class Camera
{
public:
    enum Type {
        NONE = 0,
        FPS,
    };
    enum Direction {
        FRONT,
        RIGHT,
        UP
    };


//public:
    Camera(bx::Vec3 pos);
//    Camera() = delete;
//    virtual ~Camera() = 0;
//    ~Camera() = delete;
    Camera(const Camera & camera) = delete;
//    Camera(Camera && camera);
//    Camera(Camera && camera);

    virtual void move(Direction direction, float distance);
    virtual void mouseMove(float xoffset, float yoffset) = 0;
    void zoom(float offset);
    void setViewTransform(float ratio, int viewId) const;
    void setViewTransform(const View &view) const;

//private:
//protected:
    bx::Vec3 m_pos;
//    float m_fov = 60.0f;
    float m_fov = 80.0f;
    bx::Vec3 m_front;
    bx::Vec3 m_right;
    bx::Vec3 m_up = {0.0f, 1.0f, 0.0f}; // Euler camera

    Type m_type;

//public:
protected:
    virtual void setPos(const bx::Vec3 &&pos);
};

//using Cameras = std::vector<std::unique_ptr<Camera>>; // question : Camera* ?

#endif // CAMERA_H
