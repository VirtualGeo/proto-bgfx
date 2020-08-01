
//#include <engine/geometry.h>
#include <entry/entry.h>

#include <bgfx/examples/common/bgfx_utils.h>
//#include <bgfx/examples/common/entry/entry.h>
#include <cassert>

MeshB* g_mesh = nullptr;
CameraFps* g_camera = nullptr;
bgfx::ProgramHandle g_program;
//float g_mtx[16];

namespace entry {
int s_nWindow = 1;

void init(View& view)
{
    //    entry::initFileReaderWriter();

    //    bx::mtxIdentity(g_mtx);
//        bx::mtxScale(entry::s_worldTransform, 0.01f);
    //    Program::init();
        Texture::init();
    //    Geometry::init();
    Material::init();

    // ------------------------------- LOAD MODEL
    //                 g_scene.addModel("/home/gauthier/Downloads/Cougar2/cougar.obj");
    //    if (view.id == 0) {
    //    g_camera = new CameraFps({7.0f, 8.0f, 0.0f});
    entry::s_scene.m_cameras.emplace_back(bx::Vec3 { -25.0f, 8.0f, 2.0f }, bx::Vec3 { 0.0f, 0.0f, 0.0f }); // question : push_back ?
//    entry::s_scene.m_cameras.emplace_back(std::move(CameraFps(bx::Vec3 { -25.0f, 8.0f, 2.0f }, bx::Vec3 { 0.0f }))); // question : push_back ?
    //    entry::s_scene.m_cameras.emplace_back(bx::Vec3 { -5.0f, 2.0f, 0.0f }); // question : push_back ?
    //    s_scene.m_cameras[0].setTarget({ 50.0f, -10.0f, 40.0f });
    g_camera = &entry::s_scene.m_cameras[0];
    //    } else {
    //        entry::s_scene.m_cameras.emplace_back(bx::Vec3 { 5.0, 1.0f, -1.0f }); // question : push_back ?
    //        entry::s_scene.addModel(std::string(PROJECT_DIR) + "examples/assets/sponza/sponza.obj");

    //    entry::s_scene.addModel("/home/gauthier/Downloads/San_Miguel/san-miguel.obj");

    //    entry::s_scene.addModel("/home/gauthier/Downloads/AmazonLumberyardBistro/interior.obj");
    //        entry::s_scene.addModel("/home/gauthier/Downloads/AmazonLumberyardBistro/exterior.obj");
    //    g_mesh = meshLoad("/home/gauthier/tmp/bgfx.cmake/bgfx/examples/runtime/meshes/sanMiguel.bin");

    //    entry::s_scene.addModel(std::string(PROJECT_DIR) + "examples/assets/San_Miguel/san-miguel.obj");

    g_mesh = meshLoad((std::string(PROJECT_DIR) + "examples/assets/San_Miguel/san-miguel.obj").c_str());
//    entry::s_scene.addModel(std::string(PROJECT_DIR) + "examples/assets/San_Miguel/san-miguel.obj");
    //    g_mesh = meshLoad((std::string(PROJECT_DIR) + "examples/assets/sponza/sponza.obj").c_str());

    //    g_mesh = meshLoad((std::string(PROJECT_DIR) + "examples/runtime/meshes/sanMiguel.bin").c_str());

    //    entry::s_scene.addModel("/home/gauthier/Downloads/San_Miguel/san-miguel.obj");
    //    g_mesh = meshLoad("/home/gauthier/Downloads/San_Miguel/san-miguel.bin");

    //    entry::s_scene.addModel("/home/gauthier/Downloads/sponza/sponza.obj");
    //    g_mesh = meshLoad("/home/gauthier/Downloads/sponza/sponza.bin");

    //    g_mesh = meshLoad("/home/gauthier/tmp/bgfx.cmake/bgfx/examples/runtime/meshes/sanMiguel.bin");
    //    g_mesh = meshLoad("/home/gauthier/tmp/bgfx.cmake/bgfx/examples/runtime/meshes/exterior.bin");

    g_program = Program::loadProgram("fastBlinn", "");
    assert(bgfx::isValid(g_program));

    //                    entry::s_scene.addModel("/home/gauthier/Downloads/San_Miguel/san-miguel-blend.obj");
    //                entry::s_scene.addLight(DirLight({ 0.0f, -1.0f, 0.5f }));
}

void shutdown()
{
    Material::shutdown();
    //    Program::shutdown();
    //    Texture::shutdown();
    //    Geometry::shutdown();

    meshUnload(g_mesh);
    bgfx::destroy(g_program);
}

void preRender()
{
}

void render(const View& view)
{
    g_camera->setViewTransform(view);
    //    meshSubmit(entry::s_scene.m_mesh, 0, g_program, entry::s_worldTransform);
    meshSubmit(g_mesh, VIEW_ID_START_WINDOW, g_program, entry::s_worldTransform);

//    entry::s_scene.renderView(view, entry::s_worldTransform);
    //    entry::s_scene.renderView(view, entry::s_worldTransform);

    //        entry::s_scene.renderFromCamera(m_view.iCamera, ratio, m_id, m_shading, entry::g_mtx);
    //    bgfx::dbgTextClear();
    //    bgfx::dbgTextPrintf(40, 30, 0x0f, "Hello world");
}

} // entry
