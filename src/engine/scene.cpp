#include "scene.h"

//#include "timerUtil.h"
#include <cassert>
#include <iostream>

#include "system.h"
//#include <cassert>
#include "fileIO.h"
#include <fstream>
//#include <bimg/bimg.h>
#include <chrono>
#include <cstring>
//#include <iostream>
#include "camerafps.h"

Scene::Scene()
//    : m_dirLight(bx::normalize(bx::Vec3(0.5f, -1.0f, 0.5f)))
//    : m_dirLight(bx::normalize(bx::Vec3(0.0f, -1.0f, 0.5f)))
{
    //    Vertex::init();
    //    m_layout.begin()
    //        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
    //        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
    //        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
    //        // .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
    //        .end();
}

//void Scene::addModel(const char* filename)
void Scene::addModel(const std::string& filename)
{
    //    std::string absoluteFilename(std::string(PROJECT_DIR) + filename);
    std::string absoluteFilename(filename);
    std::string bin = absoluteFilename.substr(0, absoluteFilename.find_last_of('.')) + ".bin";
    bool loadBinFailed = false;
#ifdef AUTO_GENERATE_BIN_MODEL
    if (FileExists(bin)) {
        std::ifstream file;
        file.open(bin, std::ios::binary | std::ios::in);
        if (!file.is_open()) {
            std::cerr << "cannot open file" << std::endl;
            exit(1);
        }
        try {
            load(file);
        } catch (std::exception) {
            loadBinFailed = true;
        }
        //        catch (std::bad_alloc) {
        //            loadBinFailed = true;
        //        }

        file.close();

        if (loadBinFailed) {
            m_objects.clear();
            m_materials.clear();
            m_textures.clear();
        } else {
            return;
        }
    }
#endif

    tinyobj::attrib_t tinyObjAttrib;
    std::vector<tinyobj::shape_t> tinyObjShapes;
    std::vector<tinyobj::material_t> tinyObjMaterials;
    //    std::map<std::string, uint> textures;

    //    timerutil tm;
    //    tm.start();
    auto start = std::chrono::steady_clock::now();

    std::string base_dir = GetBaseDir(absoluteFilename);
    if (base_dir.empty()) {
        base_dir = ".";
    }
#ifdef _WIN32
    base_dir += "\\";
#else
    base_dir += "/";
#endif

    std::string warn;
    std::string err;
    bool ret = tinyobj::LoadObj(&tinyObjAttrib, &tinyObjShapes, &tinyObjMaterials, &warn, &err, absoluteFilename.c_str(),
        base_dir.c_str());
    if (!warn.empty()) {
        std::cout << "[Scene] WARN: " << warn << std::endl;
        exit(1);
    }
    if (!err.empty()) {
        std::cerr << "[Scene] " << err << std::endl;
        exit(1);
    }

    //    tm.end();
    auto end = std::chrono::steady_clock::now();

    if (!ret) {
        std::cerr << "[Scene] Failed to load " << absoluteFilename << std::endl;
        //        throw std::runtime_error("");
        exit(1);
        //        return;
    }

    //    printf("[Scene] Parsing time: %d [ms]\n", (int)tm.msec());
    //    m_parsingTime = tm.msec();
    m_parsingTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    const int nbMaterials = tinyObjMaterials.size();
    const int nbObjects = tinyObjShapes.size();
#ifdef MODEL_LOADER_INFO
    printf("[Scene] # of vertices  = %d\n", (int)(tinyObjAttrib.vertices.size()) / 3);
    printf("[Scene] # of normals   = %d\n", (int)(tinyObjAttrib.normals.size()) / 3);
    printf("[Scene] # of texcoords = %d\n", (int)(tinyObjAttrib.texcoords.size()) / 2);
    printf("[Scene] # of materials = %d\n", nbMaterials);
    printf("[Scene] # of shapes    = %d\n", nbObjects);
#endif

    // Append `default` material
    //        tinyObjMaterials.push_back(tinyobj::material_t());

    //    for (size_t i = 0; i < tinyObjMaterials.size(); i++) {
    //        printf("material[%d].diffuse_texname = %s\n", int(i),
    //            tinyObjMaterials[i].diffuse_texname.c_str());
    //    }

    // Load diffuse textures
    //    {
    //    for (const tinyobj::material_t& material : tinyObjMaterials) {
    m_materials.reserve(tinyObjMaterials.size());
    m_textures.reserve(tinyObjMaterials.size());

    //    timerutil tm;
    //    tm.start();
    start = std::chrono::steady_clock::now();

    //    const size_t nbMaterials = tinyObjMaterials.size();
    for (size_t i = 0; i < nbMaterials; i++) {
        const tinyobj::material_t& tinyObj_material = tinyObjMaterials[i];

        //        printf("material[%d].difname = %s\n", int(i),
        //            material.name.c_str());
        //        m_materials.push_back(Material(material, m_textures, base_dir));
        m_materials.emplace_back(tinyObj_material, m_textures, base_dir);
#ifdef MODEL_LOADER_INFO
        const Material& material = m_materials.back();
        std::cout << "[Scene] Load material[" << i << "/" << nbMaterials << "] : " << material << std::endl;
#endif
    }
    //    for (const auto & texture : m_textures) {
    ////        const auto texture = pair.second;
    //        assert(bgfx::isValid(texture.textureHandle()));
    //    }
    assert(nbMaterials == m_materials.size());

    //    tm.end();
    end = std::chrono::steady_clock::now();
    //    m_loadingMaterialsTime = tm.msec();
    m_loadingMaterialsTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    //    tm.start();
    start = std::chrono::steady_clock::now();

    //    size_t nbIndices =0;
    //    size_t nbMeshes = 0;
    m_objects.reserve(nbObjects);
    bgfx::frame();
    for (size_t i = 0; i < nbObjects; ++i) {
        const tinyobj::shape_t& tinyObj_shape = tinyObjShapes[i];
        //        m_objects.push_back(Object(tinyObj_shape, tinyObjAttrib, tinyObjMaterials, i));
        m_objects.emplace_back(tinyObj_shape, tinyObjAttrib, tinyObjMaterials, i);
        //        nbIndices += m_objects[i].nbTriangles() * 3;
        //        nbMeshes += m_objects[i].nbMeshes();
#ifdef MODEL_LOADER_INFO
        const Object& object = m_objects.back();
        std::cout << "[Scene] Load object[" << i << "/" << nbObjects << "] : " << object << std::endl;
#endif
        bgfx::frame();
    }
    assert(nbObjects == m_objects.size());

    //    tm.end();
    //    m_loadingObjectsTime = tm.msec();
    end = std::chrono::steady_clock::now();
    //    m_loadingMaterialsTime = tm.msec();
    m_loadingObjectsTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    //    std::cout << "[Scene] Parsing time: " << m_parsingTime << " [ms]" << std::endl;
    //    std::cout << "[Scene] Loading tinyObjMaterials time: " << m_loadingMaterialsTime << " [ms]" << std::endl;
    //    std::cout << "[Scene] Loading objects time: " << m_loadingObjectsTime << " [ms]" << std::endl;
    updateStats();

#ifdef AUTO_GENERATE_BIN_MODEL
    //    save(file);
    if (!FileExists(bin) || loadBinFailed) {
        std::ofstream file;
        file.open(bin, std::ios::binary | std::ios::out);
        if (!file.is_open()) {
            std::cerr << "cannot open file" << std::endl;
            exit(1);
        }
        save(file);
        file.close();
    }
#endif
}

void Scene::clear()
{
    //        bgfx::destroy(m_layout);
    //    m_objects.clear();
    //    m_materials.clear();
    //    for (const auto & texture : m_textures) {
    ////        const auto & texture = pair.second;
    //        assert(bgfx::isValid(texture.textureHandle()));
    //    }
    m_textures.clear(); // bgfx::TextureHandle
    m_objects.clear(); // bgfx::VertexBufferHandle
    //    m_materials.clear();
}

void Scene::load(std::ifstream& file)
{

    //    timerutil tm;
    //    tm.start();
    auto start = std::chrono::steady_clock::now();

    size_t size;
    FileIO::read(size, file);
    m_materials.reserve(size);
    for (int i = 0; i < size; ++i) {
        //            m_materials.emplace_back(file, &m_textures);
        //        m_materials.push_back(file);
        m_materials.emplace_back(file);
#ifdef MODEL_LOADER_INFO
        std::cout << "[Scene] Load material[" << i << "/" << size << "] : " << m_materials.back() << std::endl;
#endif
    }

    FileIO::read(size, file);
    m_textures.reserve(size);
    for (int i = 0; i < size; ++i) {
        //        std::string texName;
        //        std::string baseDir;
        //        FileIO::read(texName, file);
        //        FileIO::read(baseDir, file);
        //        m_textures.push_back(file);
        //        m_textures.emplace_back(texName, baseDir);
        m_textures.emplace_back(file);
#ifdef MODEL_LOADER_INFO
        const Texture& texture = m_textures.back();
        std::cout << "[Scene] Load texture[" << i << "/" << size << "] : " << texture << std::endl;
#endif
    }

    //    tm.end();
    auto end = std::chrono::steady_clock::now();
    //    m_loadingMaterialsTime = tm.msec();
    m_loadingMaterialsTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    //    tm.start();
    start = std::chrono::steady_clock::now();

    FileIO::read(size, file);
    m_objects.reserve(size);
    //    bgfx::frame();
    for (size_t i = 0; i < size; ++i) {
        //        m_objects.push_back(Object(file, i, m_layout));
        //        const Object && object = Object(file, i, m_layout);
        m_objects.emplace_back(file, i);
#ifdef MODEL_LOADER_INFO
        const Object& object = m_objects.back();
        std::cout << "[Scene] Load object[" << i << "/" << size << "] : " << object << std::endl;
#endif
        bgfx::frame();
    }

    //    tm.end();
    end = std::chrono::steady_clock::now();
    //    m_loadingObjectsTime = tm.msec();
    m_loadingObjectsTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    m_parsingTime = 0;
    //    FileIO::read(m_parsingTime, file);
    //    FileIO::read(m_loadingMaterialsTime, file);
    //    FileIO::read(m_loadingObjectsTime, file);
    updateStats();
}

void Scene::save(std::ofstream& file) const
{
    size_t size;
    size = m_materials.size();
    FileIO::write(size, file);
    //    m_materials.reserve(size);
    for (int i = 0; i < size; ++i) {
        //            m_materials.emplace_back(file, &m_textures);
        //        m_materials.push_back(file);
        m_materials[i].save(file);
    }

    size = m_textures.size();
    FileIO::write(size, file);
    //    m_textures.reserve(size);
    for (int i = 0; i < size; ++i) {
        //        m_textures.push_back(file);
        //        m_textures.save(file);
        m_textures[i].save(file);
    }

    size = m_objects.size();
    FileIO::write(size, file);
    //    m_objects.reserve(size);
    for (int i = 0; i < size; ++i) {
        //        m_objects.push_back(file);
        //        m_objects.save(file);
        m_objects[i].save(file);
    }

    //    FileIO::write(m_parsingTime, file);
    //    FileIO::write(m_loadingMaterialsTime, file);
    //    FileIO::write(m_loadingObjectsTime, file);
}

void Scene::printStats(int& line)
{
    bgfx::dbgTextPrintf(0, ++line, 0x0F, "Scene: Verts:%d | Tris:%d | Verts/Tris:%.2f | Objects:%d | Textures:%d (%.1f MiB)",
        m_nbVertices, m_nbTriangles, (float)m_nbVertices / m_nbTriangles, m_nbObjects, m_nbTextures, m_texturesSize);
    bgfx::dbgTextPrintf(0, ++line, 0x0F, "   Vertex buffer:%d | Index buffer:%d | Index buffer/Vertex buffer:%.2f",
        m_nbVertexBuffer, m_nbIndexBuffer, (float)m_nbIndexBuffer / m_nbVertexBuffer);
    bgfx::dbgTextPrintf(0, ++line, 0x0F, "   TinyObj parsing time: %d ms", m_parsingTime);
    bgfx::dbgTextPrintf(0, ++line, 0x0F, "   Loading materials time: %d ms", m_loadingMaterialsTime);
    bgfx::dbgTextPrintf(0, ++line, 0x0F, "   Loading objects time: %d ms", m_loadingObjectsTime);
    bgfx::dbgTextPrintf(0, ++line, 0x0F, "   Total loading time: %d ms", m_totalLoadingTime);
    bgfx::dbgTextPrintf(0, ++line, 0x0F, "   Total draw call: %d", m_nbIndexBuffer + 1);
}

//void Scene::addLight(SpotLight&& spotLight)
//{
////    m_spotLights.emplace_back(spotLight); // question : std_move not required
//    m_spotLights.emplace_back(spotLight); // question : std_move not required
//}

//void Scene::addLight(DirLight&& dirLight)
//{
//    m_dirLights.emplace_back(dirLight);
//}

//void Scene::addLight(PointLight&& pointLight)
//{
//    m_pointLights.emplace_back(pointLight);
//}

void Scene::updateLightShadowMaps()
{
    //    entry::s_scene.draw(m_id, m_shading, entry::g_mtx, state, camera, ratio);
    float mtx[16];
    bx::mtxIdentity(mtx);
    //    uint64_t state = 0;
    //    const uint64_t state = 0 | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A
    //        | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS
    //        | BGFX_STATE_CULL_CCW | BGFX_STATE_BLEND_NORMAL | BGFX_STATE_MSAA;
    const uint64_t state = 0
        | BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_MSAA
        //            | BGFX_STATE_CULL_MASK
        | BGFX_STATE_CULL_CCW;

    //        | BGFX_STATE_FRONT_CCW;

    int viewId = 3;
    for (auto& spotLight : m_spotLights) {
        spotLight.updateLightShadowMaps(viewId);
        draw(viewId, Shading::SHADOW, mtx, state);
        ++viewId;

        //        bgfx::touch(1);
        //        bgfx::frame();
    }

    for (auto& camera : m_cameras) {
        if (camera.m_spotLightEnable) {
            camera.m_spotLight.updateLightShadowMaps(viewId);
            draw(viewId, Shading::SHADOW, mtx, state);
            ++viewId;
            //        if (camera->m_type == Camera::FPS) {
            //            static_cast<CameraFps&>(camera)

            //        }
        }
    }

    //    for (auto& dirLight : m_dirLights) {
    //        dirLight.updateLightShadowMaps();
    //    }
    //    for (auto& pointLight : m_pointLights) {
    //        pointLight.updateLightShadowMaps();
    //    }
}

void Scene::render(const bgfx::ViewId id, const Shading& shading, const float* mtx,
    const uint64_t state) const
{
    //    float view[16];
    //    // bx::mtxLookAt(view, eye, at);
    //    bx::mtxLookAt(view, camera.m_pos, bx::add(camera.m_pos, camera.m_front), camera.m_up);

    //    float proj[16];
    //    bx::mtxProj(proj, camera.m_fov, ratio, 0.1f, 100.0f,
    //        bgfx::getCaps()->homogeneousDepth);
    //    bgfx::setViewTransform(id, view, proj);

    //    bgfx::setState(state);
    //    bgfx::setTransform(mtx);
    //    bgfx::touch(id);
    //    draw(id, shading, mtx, state);
    //    bgfx::submit(id, Program::m_programs[shading], 0, BGFX_DISCARD_ALL);
    //    return;

    //    draw(id, shading, mtx, state);
    //    return;

    switch (shading) {
    case RENDERED:
        //        const int size = Program::s_num_vec4_dirLight * m_dirLights.size();
        //        std::vector<float[4]> buffer;
        //        float buffer[Program::s_numDirLightMax][8];
        //                float buffer[Program::s_numDirLightMax][Program::s_num_vec4_dirLight][4];
        //        float buffer[Program::s_dirLightSizeMax] = {0.0f};

        //        //        float * buffer[8];
        ////                std::array<float[4], Program::s_num_vec4_dirLight> buffer;
        ////                std::vector<std::array<std::array<float, 4>, Program::s_num_vec4_dirLight>> buffer;
        //                int i = 0;
        //                for (const auto& dirLight : m_dirLights) {
        ////                int offset = 0;
        ////                    for (int i =0; i <m_dirLights.size(); ++i) {
        ////                        const auto & dirLight = m_dirLights[i];
        ////                    const float temp[8] {
        ////                        dirLight.m_direction.x, dirLight.m_direction.y, dirLight.m_direction.z, 0.0,
        ////                        dirLight.m_diffuse.x, dirLight.m_diffuse.y, dirLight.m_diffuse.z, 0.0f
        ////                    };
        ////                    buffer[i][0][0] = 0;
        //                    buffer[i+ 0] = dirLight.m_direction.x;
        //                    buffer[i+ 1] = dirLight.m_direction.y;
        //                    buffer[i+ 2] = dirLight.m_direction.z;

        //                    buffer[i+ 4] = dirLight.m_diffuse.x;
        //                    buffer[i+ 5] = dirLight.m_diffuse.y;
        //                    buffer[i+ 6] = dirLight.m_diffuse.z;

        //        //            buffer[i] = &temp;
        ////                    memcpy(&buffer[i], temp, sizeof(temp));
        //                    //            &buffer[i] = std::move(temp);

        //                    i += 8;
        ////                    ++i;
        //                }
        //                buffer[3] = m_dirLights.size();

        ////        bgfx::setUniform(Program::m_uDirLights, buffer, Program::s_dirLightSizeMax);
        //        bgfx::setUniform(Program::m_uDirLights, buffer, Program::s_num_vec4_dirLight * m_dirLights.size());

        if (!m_dirLights.empty()) {
            float buffer[Program::s_dirLightSizeMax] = { 0.0f };
            int i = 0;
            for (const auto& dirLight : m_dirLights) {
                memcpy(&buffer[i], dirLight.m_data, 4 * Program::s_num_vec4_dirLight * sizeof(float));
                i += 4 * Program::s_num_vec4_dirLight;
            }
            //            buffer[3] = m_dirLights.size();
            bgfx::setUniform(Program::m_uDirLights, buffer, Program::s_num_vec4_dirLight * m_dirLights.size());
        }

        if (!m_spotLights.empty()) {
            float buffer[Program::s_spotLightSizeMax] = { 0.0f };
            int i = 0;
            //            buffer[0] = m_spotLights.size();
            for (const auto& spotLight : m_spotLights) {
                memcpy(&buffer[i], spotLight.m_data, 4 * Program::s_num_vec4_spotLight * sizeof(float));
                i += 4 * Program::s_num_vec4_spotLight;
            }
            bgfx::setUniform(Program::m_uSpotLights, buffer, Program::s_num_vec4_spotLight * m_spotLights.size());
        }

        if (!m_pointLights.empty()) {
            float buffer[Program::s_pointLightSizeMax] = { 0.0f };
            int i = 0;
            for (const auto& pointLight : m_pointLights) {
                memcpy(&buffer[i], pointLight.m_data, 4 * Program::s_num_vec4_pointLight * sizeof(float));
                i += 4 * Program::s_num_vec4_pointLight;
            }
            //            buffer[3] = m_pointLights.size();
            bgfx::setUniform(Program::m_uPointLights, buffer, Program::s_num_vec4_pointLight * m_pointLights.size());
        }
        //                const float temp[Program::s_numDirLightMax][Program::s_num_vec4_dirLight][4] {
        //                    {{0.0f, -1.0f, 0.5f, 1.0f},
        //                    {1.0f, 1.0f, 1.0f, 0.0f}},
        //                    {{0.0f, 0.0f, 0.0f, 0.0f},
        //                    {1.0f, 1.0f, 1.0f, 0.0f}}
        //                };
        ////                bgfx::setUniform(Program::m_uDirLights, temp, 4);

        //        const auto& dirLight = m_dirLights[0];
        //        const auto& dirLight2 = m_dirLights[1];

        //        float buffer[Program::s_dirLightSizeMax];
        //        for (int i =0; i <m_dirLights.size(); ++i) {

        //        }

        //        std::array<std::array<float, 4>, Program::s_num_vec4_dirLight> array {{0.0f}, {0.0f}};
        //        const float buffer[Program::s_numDirLightMax][Program::s_num_vec4_dirLight][4] {
        //            { { dirLight.m_direction.x, dirLight.m_direction.y, dirLight.m_direction.z, 1.0f },
        //                { dirLight.m_diffuse.x, dirLight.m_diffuse.y, dirLight.m_diffuse.z, 1.0f } },
        //            { { dirLight2.m_direction.x, dirLight2.m_direction.y, dirLight2.m_direction.z, 1.0f },
        //                { dirLight2.m_diffuse.x, dirLight2.m_diffuse.y, dirLight2.m_diffuse.z, 1.0f } },
        //        };
        //        bgfx::setUniform(Program::m_uDirLights, buffer, Program::s_dirLightSizeMax);

        //        using type = float[Program::s_num_vec4_dirLight * 4];
        //        typedef float[Program::s_num_vec4_dirLight * 4] type
        //        std::vector<type> buffer;

        //            new type(  dirLight.m_direction.x, dirLight.m_direction.y, dirLight.m_direction.z, 1.0f
        //                , dirLight.m_diffuse.x, dirLight.m_diffuse.y, dirLight.m_diffuse.z, 1.0f  ));

        //            { { dirLight2.m_direction.x, dirLight2.m_direction.y, dirLight2.m_direction.z, 1.0f },
        //                { dirLight2.m_diffuse.x, dirLight2.m_diffuse.y, dirLight2.m_diffuse.z, 1.0f } },
        //        bgfx::setUniform(Program::m_uDirLights, buffer, Program::s_dirLightSizeMax);

        //        const float temp[Program::s_numDirLightMax][Program::s_num_vec4_dirLight][4] {
        //            {{0.0f, -1.0f, 0.5f, 1.0f},
        //            {1.0f, 1.0f, 1.0f, 0.0f}},
        //            {{0.0f, 0.0f, 0.0f, 0.0f},
        //            {1.0f, 1.0f, 1.0f, 0.0f}}
        //        };
        //        bgfx::setUniform(Program::m_uDirLights, temp, 4);
        break;
    }

    //    bgfx::touch(id);
    //    bgfx::submit(id, Program::m_programs[shading], 0, BGFX_DISCARD_ALL);
    draw(id, shading, mtx, state);
}

void Scene::draw(const bgfx::ViewId id, const Shading& shading, const float* mtx, const uint64_t state) const
{
    for (const Object& object : m_objects) {
        //    const uint nbObjects = m_objects.size();
        //            bgfx::setTransform(mtx);
        //            bgfx::setState(state);
        //        for (int i =0; i <nbObjects; ++i) {
        //            const Object & object = m_objects[i];

        object.draw(id, shading, mtx, state, m_materials, m_textures);

        //             bgfx::submit(id, program, 0, i != nbObjects - 1);
    }
}

//void Scene::addLight(Light &&light)
//{
//    m_lights.emplace_back(light);
////    m_lights.push_back(std::move(light));
//}

void Scene::updateStats()
{
    m_nbVertices = 0;
    for (const Object& object : m_objects) {
        m_nbVertices += object.nbVertices();
    }

    m_nbTriangles = 0;
    for (const Object& object : m_objects) {
        m_nbTriangles += object.nbTriangles();
    }

    m_nbObjects = m_objects.size();

    m_nbMeshes = 0;
    for (const auto& object : m_objects) {
        m_nbMeshes += object.nbMeshes();
    }

    m_texturesSize = 0;
    for (const Texture& texture : m_textures) {
        m_texturesSize += texture.textureSize();
    }
    m_texturesSize /= 1000000.0f;

    m_nbTextures = m_textures.size();

    m_totalLoadingTime = m_parsingTime + m_loadingMaterialsTime + m_loadingObjectsTime;
    m_nbVertexBuffer = m_nbObjects;
    m_nbIndexBuffer = m_nbMeshes;
}

// ------------------------ GETTERS
//size_t Scene::nbVertices() const
//{
//    size_t nbVertices = 0;
//    for (const Object& object : m_objects) {
//        nbVertices += object.nbVertices();
//    }

//    return nbVertices;
//}

//size_t Scene::nbTriangles() const
//{
//    size_t nbTriangles = 0;
//    for (const Object& object : m_objects) {
//        nbTriangles += object.nbTriangles();
//    }

//    return nbTriangles;
//}

//size_t Scene::nbObjects() const
//{
//    return m_objects.size();
//}

//size_t Scene::texturesSize() const
//{
//    size_t texturesSize = 0;
//    for (const Texture& texture : m_textures) {
//        texturesSize += texture.textureSize();
//    }

//    return texturesSize;
//}

//size_t Scene::nbVertexBuffer() const
//{
//    return m_objects.size();
//}

//size_t Scene::nbIndexBuffer() const
//{
//    size_t ret = 0;
//    for (const auto& object : m_objects) {
//        ret += object.nbMeshes();
//    }
//    return ret;
//}

//size_t Scene::nbTextures() const
//{
//    return m_textures.size();
//}

//int Scene::parsingTime() const
//{
//    return m_parsingTime;
//}

//int Scene::loadingMaterialsTime() const
//{
//    return m_loadingMaterialsTime;
//}

//int Scene::loadingObjectsTime() const
//{
//    return m_loadingObjectsTime;
//}
