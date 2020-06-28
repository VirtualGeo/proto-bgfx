
#include <entry/entry.h>
#include <engine/geometry.h>

//class ExampleHelloWorld : public entry::AppI {
//public:
namespace entry {

void init(View & view)
{
    Geometry::init();
    Texture::init();
    Program::init();
}

void shutdown()
{
    Geometry::shutdown();
    Texture::shutdown();
    Program::shutdown();
}

void update(const View & view)
{
//    const Camera & camera = entry::s_scene.m_cameras[view.iCamera];
//    camera.setViewTransform(view.ratio, view.id);

//    bgfx::setViewTransform(view.id,
    Geometry::drawQuad();
//    Geometry::drawCube();
//    Geometry::drawUVSphere();
    float vec4[4] = { 1.0f };
    bgfx::setUniform(Program::m_hasDiffuseTexture, vec4);
    bgfx::setTexture(0, Program::m_sDiffuse,
        Texture::m_sampleTextures[Texture::Sample(Texture::CHECKER_BOARD)].textureHandle(), Texture::s_textureSamplerFlags);
    bgfx::submit(view.id, Program::m_programs[Shading::MATERIAL]);
}

} // entry

//private:
//};

//int main() {
//    ExampleHelloWorld app;
////    return entry::runApp(&app);
//    return 0;
//}
