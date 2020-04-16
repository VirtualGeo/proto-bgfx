#ifndef ENTRY_H
#define ENTRY_H

#include <engine/scene.h>
//#include <engine/windowstate.h>

//class entry
//{
//public:
//    entry();
//};

//#include <auto_ptr.h>
#include <memory>

//namespace entry { // question : using namespace with static, attributes are duplicate
class entry {
public:
    //static std::vector<WindowState> s_windows;
    static std::vector<std::unique_ptr<Camera>> s_cameras; // question : Camera* ?
    static Scene s_scene;

    static bool g_showStats;
    static float g_mtx[16];

    static std::string g_renderer;
    static std::string g_vendorID;

    static bool g_vsyncEnable;
    static int g_mssaLevel;
    static int g_textureSamplerFlags;
    static bool s_bgfxInit;

    //static std::map<int, MouseButton> g_translateMouseKey;

    static void printDebugMessage();
    static uint32_t getResetFlags();
};
//} // namespace entry

#endif // ENTRY_H
