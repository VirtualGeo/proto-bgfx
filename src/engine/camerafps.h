#ifndef CAMERAFPS_H
#define CAMERAFPS_H

#include "camera.h"
#include "spotlight.h"

class CameraFps : public Camera
{
public:
    CameraFps(bx::Vec3 pos, bx::Vec3 target = bx::Vec3(0.0));
    CameraFps(const CameraFps & camera) = delete;
    CameraFps(CameraFps && camera);
//    CameraFps(const CameraFps & cameraFps) = delete;
//    ~CameraFps();

    void updateCameraFront();
    void setTarget(bx::Vec3 target = bx::Vec3(0.0f));

//protected:
    void mouseMove(float xoffset, float yoffset) override;
    void move(Direction direction, float distance) override;
    void setPos(const bx::Vec3 &&pos) override;


public:
    SpotLight m_spotLight;
//    bool m_spotLightEnable = false;


private:
    float m_yaw = 0.0f;
    float m_pitch = 0.0f;
//    float m_fov = 60.0f;


};

#endif // CAMERAFPS_H
