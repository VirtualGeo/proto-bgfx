#include "entry.h"


//std::vector<WindowState> entry::s_windows;
std::vector<std::unique_ptr<Camera>> entry::s_cameras; // question : Camera* ?
Scene entry::s_scene;

bool entry::g_showStats = false;
float entry::g_mtx[16];

std::string entry::g_renderer;
std::string entry::g_vendorID;

bool entry::g_vsyncEnable = false;
int entry::g_mssaLevel = 0;
int entry::g_textureSamplerFlags = 0;
bool entry::s_bgfxInit = false;

//std::map<int, MouseButton> entry::g_translateMouseKey;

uint32_t entry::getResetFlags()
{

    uint32_t flags = 0 | BGFX_RESET_NONE;
    if (entry::g_vsyncEnable) {
        flags |= BGFX_RESET_VSYNC;
    }

    switch (entry::g_mssaLevel) {
    case 0:
        break;
    case 1:
        flags |= BGFX_RESET_MSAA_X2;
        break;
    case 2:
        flags |= BGFX_RESET_MSAA_X4;
        break;
    case 3:
        flags |= BGFX_RESET_MSAA_X8;
        break;
    case 4:
        flags |= BGFX_RESET_MSAA_X16; // question : why msaa_16 only differ
        break;
    }
    //    QSurfaceFormat format;
    //    format.setVersion(4, 1);
    ////    format.setProfile(QSurfaceFormat::CoreProfile);
    //    format.setProfile(QSurfaceFormat::CompatibilityProfile);
    //    format.setDepthBufferSize(24);
    //    format.setSamples(bx::pow(2, g_mssaLevel));
    //    QSurfaceFormat::setDefaultFormat(format);

    return flags;
}
