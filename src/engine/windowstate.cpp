#include "windowstate.h"

WindowState::WindowState(void *nwh, int width, int height)
    : m_nwh(nwh)
    , m_width(width)
    , m_height(height)
    , m_fbh(bgfx::createFrameBuffer(nwh, uint16_t(width), uint16_t(height)))
{

}
