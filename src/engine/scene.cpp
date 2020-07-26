#include "scene.h"

//#include "timerUtil.h"
#include <cassert>
#include <iostream>

#include "camerafps.h"
#include "fileIO.h"
#include "geometry.h"
#include "system.h"
#include <chrono>
#include <cstring>
#include <fstream>

Materials Scene::m_materials;
std::map<std::string, int> Scene::m_matName2id;
MeshB* Scene::m_mesh = nullptr;

Scene::Scene()
{
    //    m_cameras.reserve(4);
}

//void Scene::addModel(const char* filename)
// obj filename
void Scene::addModel(const std::string& filename)
{
    m_mesh = meshLoad(filename.c_str());
    assert(m_mesh != nullptr);
}

//    //    std::string absoluteFilename(std::string(PROJECT_DIR) + filename);
//    std::string absoluteFilename(filename);
//#ifdef AUTO_GENERATE_BIN_MODEL
//    std::string bin = absoluteFilename.substr(0, absoluteFilename.find_last_of('.')) + ".bin";
////    if (! FileExists(bin)) {
////        parts2bin(bin.c_str());
////    }
//    bool loadBinFailed = false;
//    if (FileExists(bin)) {
//        std::ifstream file;
//        file.open(bin, std::ios::binary | std::ios::in);
//        if (!file.is_open()) {
//            std::cerr << "cannot open file" << std::endl;
//            exit(1);
//        }
//        try {
//            load(file);
//        } catch (std::exception) {
//            loadBinFailed = true;
//        }
//        //        catch (std::bad_alloc) {
//        //            loadBinFailed = true;
//        //        }

//        file.close();

//        if (loadBinFailed) {
////            m_objects.clear();
//            m_materials.clear();
////            m_textures.clear();
//            Material::s_textures.clear();
//        } else {
////#ifdef AUTO_GIT_PARTITION
////            bin2parts(bin.c_str());
////#endif
//            return;
//        }
//    }
//#endif

//    tinyobj::attrib_t tinyObjAttrib;
//    std::vector<tinyobj::shape_t> tinyObjShapes;
//    std::vector<tinyobj::material_t> tinyObjMaterials;
//    //    std::map<std::string, uint> textures;

//    //    timerutil tm;
//    //    tm.start();
//    auto start = std::chrono::steady_clock::now();

//    std::string base_dir = GetBaseDir(absoluteFilename);
//    std::cout << "[Scene] Base dir: " << base_dir << std::endl;

//    std::string warn;
//    std::string err;
//    assert(FileExists(absoluteFilename));
//    bool ret = tinyobj::LoadObj(&tinyObjAttrib, &tinyObjShapes, &tinyObjMaterials, &warn, &err, absoluteFilename.c_str(),
//        base_dir.c_str());
//    if (!warn.empty()) {
//        std::cout << "[Scene] tinyObj WARN: " << warn << std::endl;
////        exit(1);
//    }
//    if (!err.empty()) {
//        std::cerr << "[Scene] tinyObj ERR " << err << std::endl;
//        exit(1);
//    }

//    //    tm.end();
//    auto end = std::chrono::steady_clock::now();

//    if (!ret) {
//        std::cerr << "[Scene] Failed to load " << absoluteFilename << std::endl;
//        //        throw std::runtime_error("");
//        exit(1);
//        //        return;
//    }

//    //    printf("[Scene] Parsing time: %d [ms]\n", (int)tm.msec());
//    //    m_parsingTime = tm.msec();
//    m_parsingTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

//    const int nbMaterials = tinyObjMaterials.size();
//    const int nbObjects = tinyObjShapes.size();
//#ifdef MODEL_LOADER_INFO
//    printf("[Scene] # of vertices  = %d\n", (int)(tinyObjAttrib.vertices.size()) / 3);
//    printf("[Scene] # of normals   = %d\n", (int)(tinyObjAttrib.normals.size()) / 3);
//    printf("[Scene] # of texcoords = %d\n", (int)(tinyObjAttrib.texcoords.size()) / 2);
//    printf("[Scene] # of materials = %d\n", nbMaterials);
//    printf("[Scene] # of shapes    = %d\n", nbObjects);
//#endif

//    // Append `default` material
//    //        tinyObjMaterials.push_back(tinyobj::material_t());

//    //    for (size_t i = 0; i < tinyObjMaterials.size(); i++) {
//    //        printf("material[%d].diffuse_texname = %s\n", int(i),
//    //            tinyObjMaterials[i].diffuse_texname.c_str());
//    //    }

//    // Load diffuse textures
//    //    {
//    //    for (const tinyobj::material_t& material : tinyObjMaterials) {
//    m_materials.reserve(tinyObjMaterials.size());
//    Material::s_textures.reserve(tinyObjMaterials.size());

//    //    timerutil tm;
//    //    tm.start();
//    start = std::chrono::steady_clock::now();

//    //    const size_t nbMaterials = tinyObjMaterials.size();
//    for (size_t i = 0; i < nbMaterials; i++) {
//        const tinyobj::material_t& tinyObj_material = tinyObjMaterials[i];
////        m_matName2id[tinyObj_material.name] = i;
//        assert(m_matName2id.find(tinyObj_material.name) == m_matName2id.end());
////        m_matName2id.at(tinyObj_material.name) = i;
//        m_matName2id[tinyObj_material.name] = i;

//        //        printf("material[%d].difname = %s\n", int(i),
//        //            material.name.c_str());
//        //        m_materials.push_back(Material(material, m_textures, base_dir));
//        m_materials.emplace_back(tinyObj_material, base_dir);
//#ifdef MODEL_LOADER_INFO
//        const Material& material = m_materials.back();
//        std::cout << "[Scene] Loaded material[" << i << "/" << nbMaterials << "] : " << material << std::endl;
//#endif
//    }
//    //    for (const auto & texture : m_textures) {
//    ////        const auto texture = pair.second;
//    //        assert(bgfx::isValid(texture.textureHandle()));
//    //    }
//    assert(nbMaterials == m_materials.size());

//    //    tm.end();
//    end = std::chrono::steady_clock::now();
//    //    m_loadingMaterialsTime = tm.msec();
//    m_loadingMaterialsTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

//    //    tm.start();
//    start = std::chrono::steady_clock::now();

//    //    size_t nbIndices =0;
//    //    size_t nbMeshes = 0;

////    // -------------------------------------- OBJECTS
//    m_objects.reserve(nbObjects);
////    bgfx::frame();
//    for (size_t i = 0; i < nbObjects; ++i) {
//        const tinyobj::shape_t& tinyObj_shape = tinyObjShapes[i];
//        //        m_objects.push_back(Object(tinyObj_shape, tinyObjAttrib, tinyObjMaterials, i));
//        m_objects.emplace_back(tinyObj_shape, tinyObjAttrib, tinyObjMaterials, i);
//        //        nbIndices += m_objects[i].nbTriangles() * 3;
//        //        nbMeshes += m_objects[i].nbMeshes();
//#ifdef MODEL_LOADER_INFO
//        const Object& object = m_objects.back();
//        std::cout << "[Scene] Load object[" << i << "/" << nbObjects << "] : " << object << std::endl;
//#endif
////        bgfx::frame();
//    }
//    assert(nbObjects == m_objects.size());

//    //    tm.end();
//    //    m_loadingObjectsTime = tm.msec();
//    end = std::chrono::steady_clock::now();
//    //    m_loadingMaterialsTime = tm.msec();
//    m_loadingObjectsTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

//    //    std::cout << "[Scene] Parsing time: " << m_parsingTime << " [ms]" << std::endl;
//    //    std::cout << "[Scene] Loading tinyObjMaterials time: " << m_loadingMaterialsTime << " [ms]" << std::endl;
//    //    std::cout << "[Scene] Loading objects time: " << m_loadingObjectsTime << " [ms]" << std::endl;
//    updateStats();

//#ifdef AUTO_GENERATE_BIN_MODEL
//    //    save(file);
//    if (!FileExists(bin) || loadBinFailed) {
//        std::ofstream file;
//        file.open(bin, std::ios::binary | std::ios::out);
//        if (!file.is_open()) {
//            std::cerr << "cannot open file" << std::endl;
//            exit(1);
//        }
//        save(file);
//        file.close();
//    }
//#endif

////    m_mesh = meshLoad(filename.c_str());
////    assert(m_mesh != nullptr);
//}

void Scene::clear()
{
    //        bgfx::destroy(m_layout);
    //    m_objects.clear();
    //    m_materials.clear();
    //    for (const auto & texture : m_textures) {
    ////        const auto & texture = pair.second;
    //        assert(bgfx::isValid(texture.textureHandle()));
    //    }
    Material::s_textures.clear(); // bgfx::TextureHandle
    //    m_objects.clear(); // bgfx::VertexBufferHandle
    //    m_meshes.clear();
    meshUnload(m_mesh);
    //    delete m_mesh;
    //    m_mesh = nullptr;
    m_materials.clear();
}

//void Scene::load(std::ifstream& file)
//{

//    //    timerutil tm;
//    //    tm.start();
//    auto start = std::chrono::steady_clock::now();

//    size_t size;
//    FileIO::read(size, file);
//    m_materials.reserve(size);
//    for (int i = 0; i < size; ++i) {
//        //            m_materials.emplace_back(file, &m_textures);
//        //        m_materials.push_back(file);
//        m_materials.emplace_back(file);
//        const Material& material = m_materials.back();
//        assert(m_matName2id.find(material.m_name) == m_matName2id.end());
////        m_matName2id.at(tinyObj_material.name) = i;
//        m_matName2id[material.m_name] = i;
//#ifdef MODEL_LOADER_INFO
//        std::cout << "[Scene] Load material[" << i << "/" << size << "] : " << m_materials.back() << std::endl;
//#endif
////        bgfx::frame();
//    }

//    FileIO::read(size, file);
//    Material::s_textures.reserve(size);
//    for (int i = 0; i < size; ++i) {
//        //        std::string texName;
//        //        std::string baseDir;
//        //        FileIO::read(texName, file);
//        //        FileIO::read(baseDir, file);
//        //        m_textures.push_back(file);
//        //        m_textures.emplace_back(texName, baseDir);
//        Material::s_textures.emplace_back(file);
//#ifdef MODEL_LOADER_INFO
//        const Texture& texture = Material::s_textures.back();
//        std::cout << "[Scene] Load texture[" << i << "/" << size << "] : " << texture << std::endl;
//#endif
////        bgfx::frame();
//    }

//    //    tm.end();
//    auto end = std::chrono::steady_clock::now();
//    //    m_loadingMaterialsTime = tm.msec();
//    m_loadingMaterialsTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
//    //    tm.start();
//    start = std::chrono::steady_clock::now();
////    bgfx::frame();

//    FileIO::read(size, file);
//    m_objects.reserve(size);
//    //    bgfx::frame();
//    for (size_t i = 0; i < size; ++i) {
//        //        m_objects.push_back(Object(file, i, m_layout));
//        //        const Object && object = Object(file, i, m_layout);
//        m_objects.emplace_back(file, i);
//#ifdef MODEL_LOADER_INFO
//        const Object& object = m_objects.back();
//        std::cout << "[Scene] Load object[" << i << "/" << size << "] : " << object << std::endl;
//#endif
//        bgfx::frame();
//    }

//    //    tm.end();
//    end = std::chrono::steady_clock::now();
//    //    m_loadingObjectsTime = tm.msec();
//    m_loadingObjectsTime = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

//    m_parsingTime = 0;
//    //    FileIO::read(m_parsingTime, file);
//    //    FileIO::read(m_loadingMaterialsTime, file);
//    //    FileIO::read(m_loadingObjectsTime, file);
//    updateStats();
//}

//void Scene::save(std::ofstream& file) const
//{
//    size_t size;
//    size = m_materials.size();
//    FileIO::write(size, file);
//    //    m_materials.reserve(size);
//    for (int i = 0; i < size; ++i) {
//        //            m_materials.emplace_back(file, &m_textures);
//        //        m_materials.push_back(file);
//        m_materials[i].save(file);
//    }

//    size = Material::s_textures.size();
//    FileIO::write(size, file);
//    //    m_textures.reserve(size);
//    for (int i = 0; i < size; ++i) {
//        //        m_textures.push_back(file);
//        //        m_textures.save(file);
//        Material::s_textures[i].save(file);
//    }

//    size = m_objects.size();
//    FileIO::write(size, file);
//    //    m_objects.reserve(size);
//    for (int i = 0; i < size; ++i) {
//        //        m_objects.push_back(file);
//        //        m_objects.save(file);
//        m_objects[i].save(file);
//    }

//    //    FileIO::write(m_parsingTime, file);
//    //    FileIO::write(m_loadingMaterialsTime, file);
//    //    FileIO::write(m_loadingObjectsTime, file);
//}

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
        | BGFX_STATE_WRITE_RGB
        | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z
        | BGFX_STATE_DEPTH_TEST_LESS
        | BGFX_STATE_CULL_CCW
        | BGFX_STATE_MSAA;
        //            | BGFX_STATE_CULL_MASK

    //        | BGFX_STATE_FRONT_CCW;

    int viewId = VIEW_ID_START_SHADOW;
    for (auto& dirLight : m_dirLights) {
        dirLight.updateLightShadowMaps(viewId);
        draw(viewId, Shading::SHADOW, mtx, state);
        ++viewId;
    }

    for (auto& spotLight : m_spotLights) {
        spotLight.updateLightShadowMaps(viewId);
        draw(viewId, Shading::SHADOW, mtx, state);
        //        spotLight.drawDebug();
        ++viewId;

        //        bgfx::touch(1);
        //        bgfx::frame();
    }

    for (auto& camera : m_cameras) {
        if (camera.m_spotLight.m_enable) {
            camera.m_spotLight.updateLightShadowMaps(viewId);
            //            const float ratio = 1.0f;
            //            camera.setViewTransform(ratio, viewId);
            draw(viewId, Shading::SHADOW, mtx, state);
            //            bgfx::setViewRect(viewId, 50 + (viewId - 3) *210, 200, 200, 200);
            //            bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0x00FF00FF);
            //            draw(viewId, Shading::MATERIAL, mtx, state);
            //            //            camera.m_spotLight.drawDebug();
            ++viewId;
            //            bgfx::frame();
            //        if (camera->m_type == Camera::FPS) {
            //            static_cast<CameraFps&>(camera)

            //        }
        }
    }
    //    bgfx::frame();
    //    return;

    if (m_drawDebug) {
        for (auto& dirLight : m_dirLights) {
            dirLight.drawDebug();
        }
        //    return;

        for (auto& spotLight : m_spotLights) {
            spotLight.drawDebug();
        }
        for (auto& camera : m_cameras) {
            if (camera.m_spotLight.m_enable) {
                camera.m_spotLight.drawDebug();
            }
        }
    }
    //    bgfx::setTexture(4, Program::m_sShadowMaps, );

    //    for (auto& dirLight : m_dirLights) {
    //        dirLight.updateLightShadowMaps();
    //    }
    //    for (auto& pointLight : m_pointLights) {
    //        pointLight.updateLightShadowMaps();
    //    }
}

void Scene::setLightUniforms()
{
    int nLight = 0;
    if (!m_dirLights.empty()) {
        float buffer[DirLight::s_dirLightSizeMax] = { 0.0f };
        int i = 0;
        for (const auto& dirLight : m_dirLights) {
            memcpy(&buffer[i], dirLight.m_data, 4 * DirLight::s_num_vec4_dirLight * sizeof(float));
            i += 4 * DirLight::s_num_vec4_dirLight;
            //            bgfx::setTexture(4 + nLight, Program::m_sShadowMaps[nLight], Program::m_shadowMapTexture[nLight]);
            ++nLight;
        }
        //            buffer[3] = m_dirLights.size();
        bgfx::setUniform(DirLight::s_uDirLightsUH, buffer, DirLight::s_num_vec4_dirLight * m_dirLights.size());
    }

    //    if (!m_pointLights.empty()) {
    //        float buffer[PointLight::s_pointLightSizeMax] = { 0.0f };
    //        int i = 0;
    //        for (const auto& pointLight : m_pointLights) {
    //            memcpy(&buffer[i], pointLight.m_data, 4 * PointLight::s_num_vec4_pointLight * sizeof(float));
    //            i += 4 * PointLight::s_num_vec4_pointLight;
    //            //            bgfx::setTexture(4 + nLight, Program::m_sShadowMaps[nLight], Program::m_shadowMapTexture[nLight]);
    //            ++nLight;
    //        }
    //        //            buffer[3] = m_pointLights.size();
    //        bgfx::setUniform(PointLight::s_uPointLightsUH, buffer, PointLight::s_num_vec4_pointLight * m_pointLights.size());
    //    }

    float buffer[SpotLight::s_spotLightSizeMax] = { 0.0f };
    int i = 0;
    if (!m_spotLights.empty()) {
        //            buffer[0] = m_spotLights.size();
        for (const auto& spotLight : m_spotLights) {
            memcpy(&buffer[i], spotLight.m_data, 4 * SpotLight::s_num_vec4_spotLight * sizeof(float));
            i += 4 * SpotLight::s_num_vec4_spotLight;
            //            bgfx::setTexture(4 + nLight, SpotLight::m_sShadowMaps[nLight], SpotLight::m_shadowMapTexture[nLight]);
            ++nLight;
        }
    }
    int nSpotLightCameraEnable = 0;
    for (const auto& camera : m_cameras) {
        if (camera.m_spotLight.m_enable) {
            memcpy(&buffer[i], camera.m_spotLight.m_data, 4 * SpotLight::s_num_vec4_spotLight * sizeof(float));
            i += 4 * SpotLight::s_num_vec4_spotLight;
            ++nSpotLightCameraEnable;
            //            bgfx::setTexture(4 + nLight, Program::m_sShadowMaps[nLight], Program::m_shadowMapTexture[nLight]);
            ++nLight;
        }
    }
    if (i > 0) {
        bgfx::setUniform(SpotLight::s_uSpotLightsUH, buffer, SpotLight::s_num_vec4_spotLight * (m_spotLights.size() + nSpotLightCameraEnable));
    }
    //        }
}

void Scene::setLightShadowSamplers()
{
    int nLight = 0;
    if (!m_dirLights.empty()) {
        for (const auto& dirLight : m_dirLights) {
            bgfx::setTexture(4 + nLight, dirLight.m_sShadowMapUH, dirLight.m_shadowMapTH);
            ++nLight;
        }
    }
    //    if (!m_pointLights.empty()) {
    //        for (const auto& pointLight : m_pointLights) {
    //            bgfx::setTexture(4 + nLight, pointLight.m_sShadowMapUH, pointLight.m);
    //            ++nLight;
    //        }
    //    }

    if (!m_spotLights.empty()) {
        for (const auto& spotLight : m_spotLights) {
            bgfx::setTexture(4 + nLight, spotLight.m_sShadowMapUH, spotLight.m_shadowMapTH);
            ++nLight;
        }
    }
    for (const auto& camera : m_cameras) {
        if (camera.m_spotLight.m_enable) {
            const auto& spotLight = camera.m_spotLight;
            bgfx::setTexture(4 + nLight, spotLight.m_sShadowMapUH, spotLight.m_shadowMapTH);
            ++nLight;
        }
    }
}

int Scene::getEnableSpotLight()
{
    int sum = 0;
    for (const auto& light : m_spotLights) {
        if (light.m_enable) {
            ++sum;
        }
    }
    return sum;
}

void Scene::renderView(const View& view, const float mtx[16])
{
    const uint64_t state = 0
        | BGFX_STATE_WRITE_RGB
        | BGFX_STATE_WRITE_A
        | BGFX_STATE_WRITE_Z
        | BGFX_STATE_DEPTH_TEST_LESS
        //            | BGFX_STATE_CULL_CCW
        //            | BGFX_STATE_BLEND_NORMAL
        | BGFX_STATE_MSAA;
    //        | BGFX_STATE_CULL_CCW | BGFX_STATE_MSAA;

    //    assert(0 <= m_iCamera && m_iCamera < entry::s_cameras.size());
    assert(0 <= view.iCamera && view.iCamera < m_cameras.size());
    //    const auto& camera = *entry::s_scene.m_cameras[m_iCamera];
    const auto& camera = m_cameras[view.iCamera];
    //    float view[16];
    //    // bx::mtxLookAt(view, eye, at);
    //    bx::mtxLookAt(view, camera.m_pos, bx::add(camera.m_pos, camera.m_front), camera.m_up);

    //    float proj[16];
    //    //    const float ratio = float(m_width) / m_height;
    //    bx::mtxProj(proj, camera.m_fov, ratio, 0.01f, 100.0f,
    //        bgfx::getCaps()->homogeneousDepth);
    //    bgfx::setViewTransform(viewId, view, proj);
    //    camera.setViewTransform(view.ratio, view.id);
    camera.setViewTransform(view);

    switch (view.shading) {
    case RENDERED:
        const float viewPos[4] = { camera.m_pos.x, camera.m_pos.y, camera.m_pos.z, 0.0f };
        bgfx::setUniform(Program::m_uViewPos, viewPos);
        float invModel[16];
        bx::mtxInverse(invModel, mtx);
        bgfx::setUniform(Program::m_uInvModel, invModel);
        break;
    }

    //    Geometry::drawQuad();
    draw(view.id, view.shading, mtx, state);
}

void Scene::draw(const bgfx::ViewId id, const Shading& shading, const float* mtx, const uint64_t state) const
{
    //    for (const Object& object : m_objects) {
    //        object.draw(id, shading, mtx, state, m_materials);
    ////        bgfx::submit(id, Program::m_programs[shading]);
    //    }

    assert(m_mesh != nullptr);
    //    meshSubmit(m_mesh, id, Program::m_programs[shading], mtx, state);
    m_mesh->submit(id, shading, mtx, state);

    //    m_mesh->submit(id, mtx);
    //    m_mesh->submit(id, Program::m_programs[shading], mtx);

    //    const uint nbObjects = m_objects.size();
    //            bgfx::setTransform(mtx);
    //            bgfx::setState(state);
    //    for (int i = 0; i < nbObjects; ++i) {
    //    for (auto it = m_objects.begin(), itEnd = m_objects.end(); it != itEnd; ++it) {
    //        //        const Object& object = m_objects[i];
    //        const Object& object = *it;

    //        object.draw(id, shading, mtx, state, m_materials, m_textures);

    //        //             bgfx::submit(id, program, 0, i != nbObjects - 1);
    //        bgfx::submit(id, Program::m_programs[shading], 0, (it == itEnd - 1) ? (BGFX_DISCARD_INDEX_BUFFER | BGFX_DISCARD_VERTEX_STREAMS | BGFX_DISCARD_STATE) : BGFX_DISCARD_NONE);
    //    }
}

//void Scene::addLight(Light &&light)
//{
//    m_lights.emplace_back(light);
////    m_lights.push_back(std::move(light));
//}

void Scene::updateStats()
{
    //    m_nbVertices = 0;
    //    for (const Object& object : m_objects) {
    //        m_nbVertices += object.nbVertices();
    //    }

    //    m_nbTriangles = 0;
    //    for (const Object& object : m_objects) {
    //        m_nbTriangles += object.nbTriangles();
    //    }

    //    m_nbObjects = m_objects.size();

    //    m_nbMeshes = 0;
    //    for (const auto& object : m_objects) {
    //        m_nbMeshes += object.nbMeshes();
    //    }

    m_texturesSize = 0;
    for (const Texture& texture : Material::s_textures) {
        m_texturesSize += texture.textureSize();
    }
    m_texturesSize /= 1000000.0f;

    m_nbTextures = Material::s_textures.size();

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
