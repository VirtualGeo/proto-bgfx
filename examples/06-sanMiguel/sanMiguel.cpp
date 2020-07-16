#include <engine/geometry.h>
#include <entry/entry.h>

#include <bgfx/examples/common/bgfx_utils.h>
#include <bgfx/examples/common/entry/entry.h>

MeshB * g_mesh = nullptr;
CameraFps * g_camera = nullptr;
bgfx::ProgramHandle g_program;
float g_mtx[16];

namespace entry {
int s_nWindow = 1;

void init(View& view)
{
    entry::initFileReaderWriter();
    bx::mtxIdentity(g_mtx);
//    bx::mtxScale(g_mtx, 0.001f);

    // ------------------------------- LOAD MODEL
    //                 g_scene.addModel("/home/gauthier/Downloads/Cougar2/cougar.obj");
    //    if (view.id == 0) {
//    g_camera = new CameraFps({7.0f, 8.0f, 0.0f});
    entry::s_scene.m_cameras.emplace_back(bx::Vec3 { -25.0f, 8.0f, 2.0f }); // question : push_back ?
//    s_scene.m_cameras[0].setTarget({ 50.0f, -10.0f, 40.0f });
    g_camera = &entry::s_scene.m_cameras[0];
    //    } else {
    //        entry::s_scene.m_cameras.emplace_back(bx::Vec3 { 5.0, 1.0f, -1.0f }); // question : push_back ?
    //    entry::s_scene.addModel(std::string(PROJECT_DIR) + "examples/assets/sponza/sponza.obj");

//    entry::s_scene.addModel("/home/gauthier/Downloads/San_Miguel/san-miguel.obj");

    //    entry::s_scene.addModel("/home/gauthier/Downloads/AmazonLumberyardBistro/interior.obj");
//        entry::s_scene.addModel("/home/gauthier/Downloads/AmazonLumberyardBistro/exterior.obj");
//    g_mesh = meshLoad("/home/gauthier/tmp/bgfx.cmake/bgfx/examples/runtime/meshes/sanMiguel.bin");
    g_mesh = meshLoad((std::string(PROJECT_DIR) + "examples/runtime/meshes/sanMiguel.bin").c_str());
//    g_mesh = meshLoad("/home/gauthier/tmp/bgfx.cmake/bgfx/examples/runtime/meshes/exterior.bin");

    g_program = loadProgram("fast.vert", "fast.frag");
//                    entry::s_scene.addModel("/home/gauthier/Downloads/San_Miguel/san-miguel-blend.obj");
//                entry::s_scene.addLight(DirLight({ 0.0f, -1.0f, 0.5f }));
}

void shutdown()
{
    meshUnload(g_mesh);
    bgfx::destroy(g_program);
}

void preRender()
{
}

void render(const View& view)
{
    g_camera->setViewTransform(view);
    meshSubmit(g_mesh, 0, g_program, g_mtx);
//    entry::s_scene.renderView(view, entry::s_worldTransform);

//        entry::s_scene.renderFromCamera(m_view.iCamera, ratio, m_id, m_shading, entry::g_mtx);
    //    bgfx::dbgTextClear();
    //    bgfx::dbgTextPrintf(40, 30, 0x0f, "Hello world");
}

} // entry
