#include "entry.h"

namespace entry {

Scene s_scene;
float s_worldTransform[16];
float s_fps;
size_t s_iFrame = 0;
double s_sum = 0.0;
size_t s_counter = 0;
size_t s_epoch = 10;
double s_deltaTime;
std::chrono::time_point<std::chrono::high_resolution_clock> s_lastTime;
bool s_showStats = false;
std::string s_renderer;
std::string s_vendorID;
bool s_vsyncEnable = false;
int s_mssaLevel = 0;
int s_textureSamplerFlags = 0;

bool s_bgfxInitialized = false;

//void init(View&) {};
//int shutdown() {
//    return 0;
//};
//bool update(const View&) {
//    return true;
//};


uint32_t getResetFlags()
{
    uint32_t flags = 0 | BGFX_RESET_NONE;
    if (entry::s_vsyncEnable) {
        flags |= BGFX_RESET_VSYNC;
    }

    switch (entry::s_mssaLevel) {
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

} // entry
