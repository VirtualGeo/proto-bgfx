
#include <entry/entry.h>
#include <engine/geometry.h>

//class ExampleHelloWorld : public entry::AppI {
//public:
namespace entry {

void init()
{
}

int shutdown()
{
}

bool update(const View & view)
{
    Geometry::drawQuad();
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
