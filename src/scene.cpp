#include "scene.h"

#include "timerUtil.h"
#include <cassert>
#include <iostream>

#include "system.h"
//#include <cassert>
#include "fileIO.h"
#include <fstream>
//#include <bimg/bimg.h>

Scene::Scene()
{
    //    Vertex::init();
    m_layout.begin()
        .add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
        .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
        // .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
        .end();
}

//void Scene::addModel(const char* filename)
void Scene::addModel(const std::string& filename)
{
    //    std::string absoluteFilename(std::string(PROJECT_DIR) + filename);
    std::string absoluteFilename(filename);
    std::string bin = absoluteFilename.substr(0, absoluteFilename.find_last_of('.')) + ".bin";
    //    std::cout << "bin : " << bin << std::endl;
#ifdef AUTO_GENERATE_BIN_MODEL
    if (FileExists(bin)) {
        std::ifstream file;
        file.open(bin, std::ios::binary | std::ios::in);
        if (!file.is_open()) {
            std::cerr << "cannot open file" << std::endl;
            exit(1);
        }
        load(file);
        file.close();

        return;
    }
#endif

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    //    std::map<std::string, uint> textures;

    timerutil tm;
    tm.start();

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
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, absoluteFilename.c_str(),
        base_dir.c_str());
    if (!warn.empty()) {
        std::cout << "[Scene] WARN: " << warn << std::endl;
        exit(1);
    }
    if (!err.empty()) {
        std::cerr << "[Scene] " << err << std::endl;
        exit(1);
    }

    tm.end();

    if (!ret) {
        std::cerr << "[Scene] Failed to load " << absoluteFilename << std::endl;
        //        throw std::runtime_error("");
        exit(1);
        //        return;
    }

    //    printf("[Scene] Parsing time: %d [ms]\n", (int)tm.msec());
    m_parsingTime = tm.msec();

    const int nbMaterials = materials.size();
    const int nbObjects = shapes.size();
#ifdef DEBUG
    printf("[Scene] # of vertices  = %d\n", (int)(attrib.vertices.size()) / 3);
    printf("[Scene] # of normals   = %d\n", (int)(attrib.normals.size()) / 3);
    printf("[Scene] # of texcoords = %d\n", (int)(attrib.texcoords.size()) / 2);
    printf("[Scene] # of materials = %d\n", nbMaterials);
    printf("[Scene] # of shapes    = %d\n", nbObjects);
#endif

    // Append `default` material
    //    materials.push_back(tinyobj::material_t());

    //    for (size_t i = 0; i < materials.size(); i++) {
    //        printf("material[%d].diffuse_texname = %s\n", int(i),
    //            materials[i].diffuse_texname.c_str());
    //    }

    // Load diffuse textures
    //    {
    //    for (const tinyobj::material_t& material : materials) {
    m_materials.reserve(materials.size());
    m_textures.reserve(materials.size());

    //    timerutil tm;
    tm.start();

    //    const size_t nbMaterials = materials.size();
    for (size_t i = 0; i < nbMaterials; i++) {
        const tinyobj::material_t& tinyObj_material = materials[i];

        //        printf("material[%d].difname = %s\n", int(i),
        //            material.name.c_str());
        //        m_materials.push_back(Material(material, m_textures, base_dir));
        m_materials.emplace_back(tinyObj_material, m_textures, base_dir);
#ifdef DEBUG
        const Material& material = m_materials.back();
        std::cout << "[Scene] Load material[" << i << "/" << nbMaterials << "] : " << material << std::endl;
#endif
    }
    //    for (const auto & texture : m_textures) {
    ////        const auto texture = pair.second;
    //        assert(bgfx::isValid(texture.textureHandle()));
    //    }
    assert(nbMaterials == m_materials.size());

    tm.end();
    m_loadingMaterialsTime = tm.msec();

    tm.start();

    //    size_t nbIndices =0;
    //    size_t nbMeshes = 0;
    m_objects.reserve(nbObjects);
    bgfx::frame();
    for (size_t i = 0; i < nbObjects; ++i) {
        const tinyobj::shape_t& tinyObj_shape = shapes[i];
        //        m_objects.push_back(Object(tinyObj_shape, attrib, materials, i));
        m_objects.emplace_back(tinyObj_shape, attrib, materials, i, m_layout);
        //        nbIndices += m_objects[i].nbTriangles() * 3;
        //        nbMeshes += m_objects[i].nbMeshes();
        //        std::cout << "[Scene] nbIndices:" << nbIndices << std::endl;
        //        std::cout << "[Scene] nbMeshes:" << nbMeshes << std::endl;
        //        std::cout << "[Scene] Load object[" << i << "/" << nbObjects << "] " << m_objects.back().name() << std::endl;
#ifdef DEBUG
        const Object& object = m_objects.back();
        //        std::cout << "[Scene] Load object[" << i << "/" << nbObjects << "] '" << object.name() << "', nbMeshes=" << object.nbMeshes() << ", nbTriangles=" << object.nbTriangles() << std::endl;
        std::cout << "[Scene] Load object[" << i << "/" << nbObjects << "] : " << object << std::endl;
#endif
        bgfx::frame();
    }
    assert(nbObjects == m_objects.size());

    tm.end();
    m_loadingObjectsTime = tm.msec();

    //    std::cout << "[Scene] Parsing time: " << m_parsingTime << " [ms]" << std::endl;
    //    std::cout << "[Scene] Loading materials time: " << m_loadingMaterialsTime << " [ms]" << std::endl;
    //    std::cout << "[Scene] Loading objects time: " << m_loadingObjectsTime << " [ms]" << std::endl;

#ifdef AUTO_GENERATE_BIN_MODEL
    //    save(file);
    if (!FileExists(bin)) {
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

void Scene::draw(const bgfx::ViewId id, const Program& program, const float* mtx, const uint64_t state) const
{
    for (const Object& object : m_objects) {
        //    const uint nbObjects = m_objects.size();
        //            bgfx::setTransform(mtx);
        //            bgfx::setState(state);
        //        for (int i =0; i <nbObjects; ++i) {
        //            const Object & object = m_objects[i];

        object.draw(id, program, mtx, state, m_materials, m_textures);

        //             bgfx::submit(id, program, 0, i != nbObjects - 1);
    }
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

    timerutil tm;
    tm.start();

    size_t size;
    FileSystem::read(size, file);
    m_materials.reserve(size);
    for (int i = 0; i < size; ++i) {
        //            m_materials.emplace_back(file, &m_textures);
        //        m_materials.push_back(file);
        m_materials.emplace_back(file);
#ifdef DEBUG
        std::cout << "[Scene] Load material[" << i << "/" << size << "] : " << m_materials.back() << std::endl;
#endif
    }

    FileSystem::read(size, file);
    m_textures.reserve(size);
    for (int i = 0; i < size; ++i) {
        //        std::string texName;
        //        std::string baseDir;
        //        FileSystem::read(texName, file);
        //        FileSystem::read(baseDir, file);
        //        m_textures.push_back(file);
        //        m_textures.emplace_back(texName, baseDir);
        m_textures.emplace_back(file);
#ifdef DEBUG
        const Texture& texture = m_textures.back();
        std::cout << "[Scene] Load texture[" << i << "/" << size << "] : " << texture << std::endl;
#endif
    }

    tm.end();
    m_loadingMaterialsTime = tm.msec();
    tm.start();

    FileSystem::read(size, file);
    m_objects.reserve(size);
    //    bgfx::frame();
    for (size_t i = 0; i < size; ++i) {
        //        m_objects.push_back(Object(file, i, m_layout));
        //        const Object && object = Object(file, i, m_layout);
        m_objects.emplace_back(file, i, m_layout);
#ifdef DEBUG
        const Object& object = m_objects.back();
        std::cout << "[Scene] Load object[" << i << "/" << size << "] : " << object << std::endl;
#endif
        bgfx::frame();
    }

    tm.end();
    m_loadingObjectsTime = tm.msec();

    m_parsingTime = 0;
    //    FileSystem::read(m_parsingTime, file);
    //    FileSystem::read(m_loadingMaterialsTime, file);
    //    FileSystem::read(m_loadingObjectsTime, file);
}

void Scene::save(std::ofstream& file) const
{
    size_t size;
    size = m_materials.size();
    FileSystem::write(size, file);
    //    m_materials.reserve(size);
    for (int i = 0; i < size; ++i) {
        //            m_materials.emplace_back(file, &m_textures);
        //        m_materials.push_back(file);
        m_materials[i].save(file);
    }

    size = m_textures.size();
    FileSystem::write(size, file);
    //    m_textures.reserve(size);
    for (int i = 0; i < size; ++i) {
        //        m_textures.push_back(file);
        //        m_textures.save(file);
        m_textures[i].save(file);
    }

    size = m_objects.size();
    FileSystem::write(size, file);
    //    m_objects.reserve(size);
    for (int i = 0; i < size; ++i) {
        //        m_objects.push_back(file);
        //        m_objects.save(file);
        m_objects[i].save(file);
    }

    //    FileSystem::write(m_parsingTime, file);
    //    FileSystem::write(m_loadingMaterialsTime, file);
    //    FileSystem::write(m_loadingObjectsTime, file);
}

// ------------------------ GETTERS
size_t Scene::nbVertices() const
{
    size_t nbVertices = 0;
    for (const Object& object : m_objects) {
        nbVertices += object.nbVertices();
    }

    return nbVertices;
}

size_t Scene::nbTriangles() const
{
    size_t nbTriangles = 0;
    for (const Object& object : m_objects) {
        nbTriangles += object.nbTriangles();
    }

    return nbTriangles;
}

size_t Scene::nbObjects() const
{
    return m_objects.size();
}

size_t Scene::texturesSize() const
{
    size_t texturesSize = 0;
    for (const Texture& texture : m_textures) {
        texturesSize += texture.textureSize();
    }

    return texturesSize;
}

size_t Scene::nbVertexBuffer() const
{
    return m_objects.size();
}

size_t Scene::nbIndexBuffer() const
{
    size_t ret = 0;
    for (const auto& object : m_objects) {
        ret += object.nbMeshes();
    }
    return ret;
}

size_t Scene::nbTextures() const
{
    return m_textures.size();
}

int Scene::parsingTime() const
{
    return m_parsingTime;
}

int Scene::loadingMaterialsTime() const
{
    return m_loadingMaterialsTime;
}

int Scene::loadingObjectsTime() const
{
    return m_loadingObjectsTime;
}
