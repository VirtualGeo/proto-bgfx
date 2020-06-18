#ifndef CAMERAFPS_H
#define CAMERAFPS_H

#include "camera.h"
#include "spotlight.h"

class CameraFps : public Camera
{
public:
    CameraFps(bx::Vec3 pos, bx::Vec3 target = bx::Vec3(0.0));
//    CameraFps(const CameraFps & camera) = delete;
//    CameraFps(CameraFps && camera) noexcept = default;

    void updateCameraFront();

//protected:
    void mouseMove(float xoffset, float yoffset) override;
    void move(Direction direction, float distance) override;

public:
    SpotLight m_spotLight;
    bool m_spotLightEnable = true;


private:
    float m_yaw = 0.0f;
    float m_pitch = 0.0f;
//    float m_fov = 60.0f;


};

#endif // CAMERAFPS_H
