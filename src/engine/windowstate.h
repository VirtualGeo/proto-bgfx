#ifndef WINDOWSTATE_H
#define WINDOWSTATE_H


#include <bgfx/bgfx.h>
#include "program.h"

class WindowState {
public:
    WindowState(void * nwh, int width, int height);

//private:
    int m_width;
    int m_height;

    bgfx::FrameBufferHandle m_fbh;
    void* m_nwh = nullptr;
    float m_fps;
    Shading m_shading = Shading::RENDERED;
};

#endif // WINDOWSTATE_H
