
#include <entry/entry.h>
#include <engine/geometry.h>

static Material s_material;
static bgfx::UniformHandle s_sDiffuseUH = BGFX_INVALID_HANDLE;

namespace entry {
    int s_nWindow = 1;

void init(View & view)
{
    Geometry::init();
    Texture::init();
    Program::init();
    Material::init();

//    s_material = new Material();
    s_material.m_iTexDiffuse = 1;
    s_sDiffuseUH = bgfx::createUniform("s_diffuse", bgfx::UniformType::Sampler);
}

void shutdown()
{
    Geometry::shutdown();
    Texture::shutdown();
    Program::shutdown();
    Material::shutdown();

    bgfx::destroy(s_sDiffuseUH);
}

void preRender()
{
}

void render(const View & view)
{
//    const Camera & camera = entry::s_scene.m_cameras[view.iCamera];
//    camera.setViewTransform(view.ratio, view.id);

//    bgfx::setViewTransform(view.id,
    Geometry::drawQuad();
//    Geometry::drawCube();
//    Geometry::drawUVSphere();

    s_material.submit();
//    float vec4[4] = { 1.0f };
//    bgfx::setUniform(Program::m_hasDiffuseTexture, vec4);
    bgfx::setTexture(0, s_sDiffuseUH,
        Texture::m_sampleTextures[Texture::Sample(Texture::CHECKER_BOARD)].textureHandle(), Texture::s_textureSamplerFlags);

//    bgfx::submit(view.id, Program::m_programs[view.shading]);
    bgfx::submit(view.id, Program::m_programs[Shading::MATERIAL]);
//    Program::submit(view.id, view.shading, s_material);
}

} // entry
