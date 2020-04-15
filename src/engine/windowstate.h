#ifndef WINDOWSTATE_H
#define WINDOWSTATE_H


#include <bgfx/bgfx.h>
#include "program.h"

class WindowState {
public:
    WindowState(void * nwh, int width, int height);
//    ~WindowState();

    void clear();

//private:
    void* m_nwh = nullptr;
    int m_width;
    int m_height;

    bgfx::FrameBufferHandle m_fbh;
    float m_fps;
    size_t m_epoch = 10;

    Shading m_shading = Shading::RENDERED;
};

#endif // WINDOWSTATE_H
