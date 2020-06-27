#include "entry.h"

//std::vector<WindowState> entry::s_windows;
//std::vector<std::unique_ptr<Camera>> entry::s_cameras; // question : Camera* ?
//Scene entryFake::s_scene;

bool entryFake::g_showStats = false;
//float entryFake::g_mtx[16];

std::string entryFake::g_renderer;
std::string entryFake::g_vendorID;

bool entryFake::g_vsyncEnable = false;
int entryFake::g_mssaLevel = 0;
int entryFake::g_textureSamplerFlags = 0;
bool entryFake::s_bgfxInit = false;

//std::map<int, MouseButton> entry::g_translateMouseKey;

uint32_t entryFake::getResetFlags()
{

    uint32_t flags = 0 | BGFX_RESET_NONE;
    if (entryFake::g_vsyncEnable) {
        flags |= BGFX_RESET_VSYNC;
    }

    switch (entryFake::g_mssaLevel) {
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

namespace entry {
    Scene s_scene;
    float s_worldTransform[16];
//    static uint32_t s_debug = BGFX_DEBUG_NONE;
//    static uint32_t s_reset = BGFX_RESET_NONE;
//    static uint32_t s_width = 800;
//    static uint32_t s_height = 600;


//    static Scene s_scene;

//int runApp(AppI* app)
//{
//    app->init(s_width, s_height);
//    bgfx::frame();

//    return 0;
//}

} // entry
