#include "windowstate.h"
#include <QDebug>

WindowState::WindowState(void *nwh, int width, int height)
    : m_nwh(nwh)
    , m_width(width)
    , m_height(height)
//    , m_fbh(bgfx::createFrameBuffer(nwh, uint16_t(width), uint16_t(height)))
{
    m_fbh.idx = bgfx::kInvalidHandle;
    Q_ASSERT(! bgfx::isValid(m_fbh));

}

//WindowState::~WindowState()
//{
////    bgfx::destroy(m_fbh);

//}

void WindowState::clear()
{
    bgfx::destroy(m_fbh);

}
