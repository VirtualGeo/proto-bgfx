
#include <engine/geometry.h>
#include <entry/entry.h>

//class ExampleHelloWorld : public entry::AppI {
//public:
namespace entry {

void init(View & view)
{
    // ------------------------------- LOAD MODEL
    //                 g_scene.addModel("/home/gauthier/Downloads/Cougar2/cougar.obj");
    entry::s_scene.addModel(std::string(PROJECT_DIR) + "examples/assets/sponza/sponza.obj");

    //                entry::s_scene.addModel("/home/gauthier/Downloads/Cougar2/cougar.obj");
    //                entry::s_scene.addModel("/home/gauthier/Downloads/San_Miguel/san-miguel-low-poly.obj");
    //                entry::s_scene.addModel("/home/gauthier/Downloads/San_Miguel/san-miguel.obj");
    //                entry::s_scene.addModel("/home/gauthier/Downloads/San_Miguel/san-miguel-blend.obj");

    //        entry::s_scene.addModel("C:\\Users\\gauthier.bouyjou\\Downloads\\CornellBox\\CornellBox-Original.obj");
    //        entry::s_scene.addModel("C:\\Users\\gauthier.bouyjou\\Downloads\\export\\cougar.obj");

    //        entry::s_scene.addLight(DirLight({ 0.0f, -1.0f, 0.5f }));
    //        entry::s_scene.addSpotLight(SpotLight({ 1.0f, 0.0f, 0.0f }, { -5.0f, 1.0f, 0.0f }));
    //        entry::s_scene.addLight({ 1.0f, 0.0f, 0.0f }, { -5.0f, 1.0f, 0.0f });
    //        entry::s_scene.addLight(bx::Vec3(1.0f, 0.0f, 0.0f), bx::Vec3(-5.0f, 1.0f, 0.0f));
    //        entry::s_scene.addSpotLight(bx::Vec3(1.0f, -0.1f, 0.1f), bx::Vec3(-3.0f, 1.0f, 0.0f));

    //        entry::s_scene.addSpotLight(bx::Vec3(0.0f, 0.0f, 1.0f), bx::Vec3(0.0f, 1.0f, 0.0f));
    //        entry::s_scene.addLight<SpotLight>({});

    //        entry::s_scene.addLight(PointLight({ 0.0f, 1.0f, 0.0f }));
}

int shutdown()
{
}

bool update(const View & view)
{
    entry::s_scene.renderView(view, entry::s_worldTransform);
//    entry::s_scene.renderFromCamera(m_view.iCamera, ratio, m_id, m_shading, entry::g_mtx);
    //    bgfx::dbgTextClear();
//    bgfx::dbgTextPrintf(40, 30, 0x0f, "Hello world");
}

} // entry

//private:
//};

//int main() {
//    ExampleHelloWorld app;
////    return entry::runApp(&app);
//    return 0;
//}
