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
#include "windowstate.h"

//namespace entry { // question : using namespace with static, attributes are duplicate
class entryFake {
public:
    //static std::vector<WindowState> s_windows;
//    static std::vector<std::unique_ptr<Camera>> s_cameras; // question : Camera* ?
//    static Scene s_scene;

    static bool g_showStats;
//    static float g_mtx[16];

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

namespace entry {
    extern Scene s_scene;
    extern float s_worldTransform[16];
//    class AppI {
//    public:
//        virtual void init(uint32_t width, uint32_t height) = 0;
//        virtual int shutdown() = 0;
//        virtual bool update() = 0;
        void init();
        int shutdown();
        bool update(const View & view);
//    private:
//    };

//    int runApp(AppI * app);
} // entry

#endif // ENTRY_H
