#ifndef CAMERAFPS_H
#define CAMERAFPS_H

//#include <bx/file.h>
#include "camera.h"

class CameraFps : public Camera
{
public:
    CameraFps(bx::Vec3 pos);

    void updateCameraFront();

protected:
    void mouseMove(float xoffset, float yoffset) override;

private:
    float m_yaw = 0.0f;
    float m_pitch = 0.0f;
//    float m_fov = 60.0f;
};

#endif // CAMERAFPS_H
