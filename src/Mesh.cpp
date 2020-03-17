#include "Mesh.h"

#include <algorithm>

void PrintInfo(const tinyobj::attrib_t& attrib,
                      const std::vector<tinyobj::shape_t>& shapes,
                      const std::vector<tinyobj::material_t>& materials) {
  std::cout << "# of vertices  : " << (attrib.vertices.size() / 3) << std::endl;
  std::cout << "# of normals   : " << (attrib.normals.size() / 3) << std::endl;
  std::cout << "# of texcoords : " << (attrib.texcoords.size() / 2)
            << std::endl;

  std::cout << "# of shapes    : " << shapes.size() << std::endl;
  std::cout << "# of materials : " << materials.size() << std::endl;

  for (size_t v = 0; v < attrib.vertices.size() / 3; v++) {
    printf("  v[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
           static_cast<const double>(attrib.vertices[3 * v + 0]),
           static_cast<const double>(attrib.vertices[3 * v + 1]),
           static_cast<const double>(attrib.vertices[3 * v + 2]));
  }

  for (size_t v = 0; v < attrib.normals.size() / 3; v++) {
    printf("  n[%ld] = (%f, %f, %f)\n", static_cast<long>(v),
           static_cast<const double>(attrib.normals[3 * v + 0]),
           static_cast<const double>(attrib.normals[3 * v + 1]),
           static_cast<const double>(attrib.normals[3 * v + 2]));
  }

  for (size_t v = 0; v < attrib.texcoords.size() / 2; v++) {
    printf("  uv[%ld] = (%f, %f)\n", static_cast<long>(v),
           static_cast<const double>(attrib.texcoords[2 * v + 0]),
           static_cast<const double>(attrib.texcoords[2 * v + 1]));
  }

  // For each shape
  for (size_t i = 0; i < shapes.size(); i++) {
    printf("shape[%ld].name = %s\n", static_cast<long>(i),
           shapes[i].name.c_str());
    printf("Size of shape[%ld].mesh.indices: %lu\n", static_cast<long>(i),
           static_cast<unsigned long>(shapes[i].mesh.indices.size()));
    printf("Size of shape[%ld].lines.indices: %lu\n", static_cast<long>(i),
           static_cast<unsigned long>(shapes[i].lines.indices.size()));
    printf("Size of shape[%ld].points.indices: %lu\n", static_cast<long>(i),
           static_cast<unsigned long>(shapes[i].points.indices.size()));

    size_t index_offset = 0;

    assert(shapes[i].mesh.num_face_vertices.size() ==
           shapes[i].mesh.material_ids.size());

    assert(shapes[i].mesh.num_face_vertices.size() ==
           shapes[i].mesh.smoothing_group_ids.size());

    printf("shape[%ld].num_faces: %lu\n", static_cast<long>(i),
           static_cast<unsigned long>(shapes[i].mesh.num_face_vertices.size()));

    // For each face
    for (size_t f = 0; f < shapes[i].mesh.num_face_vertices.size(); f++) {
      size_t fnum = shapes[i].mesh.num_face_vertices[f];

      printf("  face[%ld].fnum = %ld\n", static_cast<long>(f),
             static_cast<unsigned long>(fnum));

      // For each vertex in the face
      for (size_t v = 0; v < fnum; v++) {
        tinyobj::index_t idx = shapes[i].mesh.indices[index_offset + v];
        printf("    face[%ld].v[%ld].idx = %d/%d/%d\n", static_cast<long>(f),
               static_cast<long>(v), idx.vertex_index, idx.normal_index,
               idx.texcoord_index);
      }

      printf("  face[%ld].material_id = %d\n", static_cast<long>(f),
             shapes[i].mesh.material_ids[f]);
      printf("  face[%ld].smoothing_group_id = %d\n", static_cast<long>(f),
             shapes[i].mesh.smoothing_group_ids[f]);

      index_offset += fnum;
    }

    printf("shape[%ld].num_tags: %lu\n", static_cast<long>(i),
           static_cast<unsigned long>(shapes[i].mesh.tags.size()));
    for (size_t t = 0; t < shapes[i].mesh.tags.size(); t++) {
      printf("  tag[%ld] = %s ", static_cast<long>(t),
             shapes[i].mesh.tags[t].name.c_str());
      printf(" ints: [");
      for (size_t j = 0; j < shapes[i].mesh.tags[t].intValues.size(); ++j) {
        printf("%ld", static_cast<long>(shapes[i].mesh.tags[t].intValues[j]));
        if (j < (shapes[i].mesh.tags[t].intValues.size() - 1)) {
          printf(", ");
        }
      }
      printf("]");

      printf(" floats: [");
      for (size_t j = 0; j < shapes[i].mesh.tags[t].floatValues.size(); ++j) {
        printf("%f", static_cast<const double>(
                         shapes[i].mesh.tags[t].floatValues[j]));
        if (j < (shapes[i].mesh.tags[t].floatValues.size() - 1)) {
          printf(", ");
        }
      }
      printf("]");

      printf(" strings: [");
      for (size_t j = 0; j < shapes[i].mesh.tags[t].stringValues.size(); ++j) {
        printf("%s", shapes[i].mesh.tags[t].stringValues[j].c_str());
        if (j < (shapes[i].mesh.tags[t].stringValues.size() - 1)) {
          printf(", ");
        }
      }
      printf("]");
      printf("\n");
    }
  }

  for (size_t i = 0; i < materials.size(); i++) {
    printf("material[%ld].name = %s\n", static_cast<long>(i),
           materials[i].name.c_str());
    printf("  material.Ka = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].ambient[0]),
           static_cast<const double>(materials[i].ambient[1]),
           static_cast<const double>(materials[i].ambient[2]));
    printf("  material.Kd = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].diffuse[0]),
           static_cast<const double>(materials[i].diffuse[1]),
           static_cast<const double>(materials[i].diffuse[2]));
    printf("  material.Ks = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].specular[0]),
           static_cast<const double>(materials[i].specular[1]),
           static_cast<const double>(materials[i].specular[2]));
    printf("  material.Tr = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].transmittance[0]),
           static_cast<const double>(materials[i].transmittance[1]),
           static_cast<const double>(materials[i].transmittance[2]));
    printf("  material.Ke = (%f, %f ,%f)\n",
           static_cast<const double>(materials[i].emission[0]),
           static_cast<const double>(materials[i].emission[1]),
           static_cast<const double>(materials[i].emission[2]));
    printf("  material.Ns = %f\n",
           static_cast<const double>(materials[i].shininess));
    printf("  material.Ni = %f\n", static_cast<const double>(materials[i].ior));
    printf("  material.dissolve = %f\n",
           static_cast<const double>(materials[i].dissolve));
    printf("  material.illum = %d\n", materials[i].illum);
    printf("  material.map_Ka = %s\n", materials[i].ambient_texname.c_str());
    printf("  material.map_Kd = %s\n", materials[i].diffuse_texname.c_str());
    printf("  material.map_Ks = %s\n", materials[i].specular_texname.c_str());
    printf("  material.map_Ns = %s\n",
           materials[i].specular_highlight_texname.c_str());
    printf("  material.map_bump = %s\n", materials[i].bump_texname.c_str());
    printf("    bump_multiplier = %f\n", static_cast<const double>(materials[i].bump_texopt.bump_multiplier));
    printf("  material.map_d = %s\n", materials[i].alpha_texname.c_str());
    printf("  material.disp = %s\n", materials[i].displacement_texname.c_str());
    printf("  <<PBR>>\n");
    printf("  material.Pr     = %f\n", static_cast<const double>(materials[i].roughness));
    printf("  material.Pm     = %f\n", static_cast<const double>(materials[i].metallic));
    printf("  material.Ps     = %f\n", static_cast<const double>(materials[i].sheen));
    printf("  material.Pc     = %f\n", static_cast<const double>(materials[i].clearcoat_thickness));
    printf("  material.Pcr    = %f\n", static_cast<const double>(materials[i].clearcoat_thickness));
    printf("  material.aniso  = %f\n", static_cast<const double>(materials[i].anisotropy));
    printf("  material.anisor = %f\n", static_cast<const double>(materials[i].anisotropy_rotation));
    printf("  material.map_Ke = %s\n", materials[i].emissive_texname.c_str());
    printf("  material.map_Pr = %s\n", materials[i].roughness_texname.c_str());
    printf("  material.map_Pm = %s\n", materials[i].metallic_texname.c_str());
    printf("  material.map_Ps = %s\n", materials[i].sheen_texname.c_str());
    printf("  material.norm   = %s\n", materials[i].normal_texname.c_str());
    std::map<std::string, std::string>::const_iterator it(
        materials[i].unknown_parameter.begin());
    std::map<std::string, std::string>::const_iterator itEnd(
        materials[i].unknown_parameter.end());

    for (; it != itEnd; it++) {
      printf("  material.%s = %s\n", it->first.c_str(), it->second.c_str());
    }
    printf("\n");
  }
}

Mesh::Mesh(const char *filePath) {
	// ------------- INIT GEOMETRY
	// bgfx::VertexDecl pcvDecl; // question : out of date ?
//	std::string path = std::string(filePath);
//	std::string path(PROJECT_DIR + filePath);
    std::string path = std::string(PROJECT_DIR) + filePath;
    std::string directory = std::string(path).substr(0, path.find_last_of('/'));
	std::cout << "DIRECTORY : '" << directory << "'" << std::endl;

	// bgfx::VertexLayout ms_layout;
	m_layout.begin()
		.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
		.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
		// .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
		.end();

	// vbh = bgfx::createVertexBuffer(
	// 	bgfx::makeRef(cubeVertices, sizeof(cubeVertices)), ms_layout);
	// ibh = bgfx::createIndexBuffer(
	// 	bgfx::makeRef(cubeTriList, sizeof(cubeTriList)));
	const uint16_t stride = m_layout.getStride();

	// const bgfx::Memory * mem = bgfx::alloc(vertices.size * stride);
	// bx::memCopy(vertices, )
	// vbh = bgfx::createVertexBuffer(mem, ms_layout);
	assert(stride == sizeof(Vertex));

	// bgfx::VertexBufferHandle vbh;
	// std::string inputFile = "D:/proto-bgfx/Assets/Teapot/teapot.obj";
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warn;
	std::string err;

	const bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err,
                                      path.c_str(), directory.c_str());

	if (!warn.empty()) {
		std::cout << warn << std::endl;
	}

	if (!err.empty()) {
		std::cerr << err << std::endl;
	}

	if (!ret) {
		// std::cout << "cannot load obj" << std::endl;
		throw std::runtime_error("cannot load obj");
		// exit(1);
	}
//    PrintInfo(attrib, shapes, materials);
//    return;

	const int nbMaterial = materials.size();

	printf("# of vertices  = %d\n", (int)(attrib.vertices.size()) / 3);
	printf("# of normals   = %d\n", (int)(attrib.normals.size()) / 3);
	printf("# of texcoords = %d\n", (int)(attrib.texcoords.size()) / 2);
	printf("# of materials = %d\n", nbMaterial);
	printf("# of shapes    = %d\n", (int)shapes.size());

	// Append `default` material
	// materials.push_back(tinyobj::material_t());

	for (size_t i = 0; i < materials.size(); i++) {
		const auto &mat = materials[i];
		// texture_option_t
		std::cout << "material[" << i << "/" << nbMaterial << "] name='"
				  << mat.name << "', diffuseName='"
				  << materials[i].diffuse_texname << "', specularName='"
				  << materials[i].specular_texname << "', bumpName='"
				  << materials[i].bump_texname << "', normalName='"
				  << materials[i].normal_texname << "'" << std::endl;
	}

	if (attrib.vertices.empty()) {
		std::cout << "Mesh: vertex load failed '" << filePath << "'"
				  << std::endl;
		// throw std::runtime_error("bad vertices");
		exit(1);
	}
	// if (attrib.normals.empty()) {
	// 	std::cout << "Mesh: normal load failed '" << filePath << "'"
	// 			  << std::endl;
	// 	// throw std::runtime_error("bad vertices");
	// 	exit(1);
	// }
	if (attrib.texcoords.empty()) {
		std::cout << "Mesh: texcoords load failed '" << filePath << "'"
				  << std::endl;
		// throw std::runtime_error("bad vertices");
		exit(1);
	}
	// std::vector<float[8]> vertices;
	// std::vector<Vertex> vertices;
	// vertices.clear();
	// std::vector<uint16_t> triangle_list;

	// std::vector<Vertex> vertices;
	// std::vector<uint16_t> triangle_list;
	m_groups.clear();
	const int nbShape = shapes.size();
	m_groups.resize(nbShape);

	// m_vertices.clear();
	// m_indices.clear();
	// m_vertices.reserve(1000);
	// m_indices.reserve(1000);

	// attrib.vertices;
	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		auto &shape = shapes[s];
		int nbVertice = 0;
		const int nbFace = shape.mesh.num_face_vertices.size();
		assert(nbFace > 0);
		std::cout << "shape[" << s << "/" << nbShape << "] name='" << shape.name
				  << "', nbFace=" << nbFace;
		// shape.mesh.tags;
		auto &group = m_groups[s];
		auto &vertices = group.m_vertices;
		// vertices.clear();
		auto &indices = group.m_indices;

		// assert(shapes.size() == 3);

		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < nbFace; f++) {
			int fv = shape.mesh.num_face_vertices[f];
			assert(fv == 3);
			nbVertice += fv;

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				// shapes[s].mesh.indices;
				assert(index_offset + v >= 0);
				assert(index_offset + v < shape.mesh.indices.size());
				tinyobj::index_t idx = shape.mesh.indices[index_offset + v];
				assert(3 * idx.vertex_index + 2 < attrib.vertices.size());
				assert(idx.vertex_index >= 0);
				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];

				tinyobj::real_t nx = 0.0;
				tinyobj::real_t ny = 1.0;
				tinyobj::real_t nz = 0.0;
				if (idx.normal_index != -1) {
					assert(3 * idx.normal_index + 2 < attrib.normals.size());
					assert(idx.normal_index >= 0);

					attrib.normals[3 * idx.normal_index + 0];
					attrib.normals[3 * idx.normal_index + 1];
					attrib.normals[3 * idx.normal_index + 2];
				}
				// if (2 * idx.texcoord_index + 1 >= attrib.texcoords.size()) {
				// 	std::cout << "index=" << idx.texcoord_index << ", attrib="
				// << attrib.texcoords.size() << std::endl; 	exit(1);
				// }
				tinyobj::real_t tx = 0.0f;
				tinyobj::real_t ty = 0.0f;
				if (idx.texcoord_index != -1) { // has texture coords
					assert(2 * idx.texcoord_index + 1 <
						   attrib.texcoords.size());
					assert(idx.texcoord_index >= 0);

					tx = attrib.texcoords[2 * idx.texcoord_index + 0];
					ty = attrib.texcoords[2 * idx.texcoord_index + 1];
				}

				// vertices.push_back({{vx, vy, vz}, {nx, ny, nz}}); //
				// question : emplace_back ?
				// vertices.push_back({vx, vy, vz, nx, ny, nz, tx,
				// ty}); // question : emplace_back ?
				vertices.push_back({vx, vy, vz, nx, ny, nz, tx,
									ty}); // question : emplace_back ?
				// Optional: vertex colors
				// tinyobj::real_t red =
				// attrib.colors[3*idx.vertex_index+0]; tinyobj::real_t
				// green = attrib.colors[3*idx.vertex_index+1];
				// tinyobj::real_t blue =
				// attrib.colors[3*idx.vertex_index+2];
				indices.push_back(uint16_t(indices.size()));
			}
			index_offset += fv;

			// per-face material
			// shapes[s].mesh.material_ids[f];
		}
		assert(index_offset == nbVertice);

		const float meanVerticePerPolygon = nbVertice / nbFace;
		std::cout << ", nbVerticePerPolygon=" << meanVerticePerPolygon;

		group.m_vbh = bgfx::createVertexBuffer(
			bgfx::makeRef(vertices.data(), sizeof(Vertex) * vertices.size()),
			m_layout);

		group.m_ibh = bgfx::createIndexBuffer(
			bgfx::makeRef(indices.data(), sizeof(uint16_t) * indices.size()));

		// shapes[s].mesh.material_ids[f];

		// const int nbMat = shape.mesh.material_ids.size();
		// if (nbMat == 0) {
		// 	std::cout << "none material for object" << std::endl;
		// 	exit(1);
		// }
		//  else if (nbMat != 1) {
		// 	std::cout << "multiple materials (" << nbMat
		// 			  << ") for same object : ";
		// 	for (int i = 0; i < nbMat; ++i) {
		// 		const auto &material =
		// 			materials[shapes[s].mesh.material_ids[i]];
		// 		std::cout << material.name << ", ";
		// 	}
		// 	std::cout << std::endl;

		// 	// exit(1);
		// }

		// assert(!shape.mesh.material_ids.empty());
		// assert(nbMaterial > shape.mesh.material_ids[0]);
		if (shape.mesh.material_ids[0] == -1) { // no material
			group.m_diffuse[0] = 0.0f;
			group.m_diffuse[1] = 1.0f;
			group.m_diffuse[2] = 0.0f;
			group.m_diffuse[3] = 0.0f;

		} else { // has material

			const auto &material = materials[shape.mesh.material_ids[0]];

			// tinyobj::material_t * material = nullptr;
			// for (int i =0; i <shapes[s].mesh.material_ids.size(); ++i) {
			//     if ()

			// }
			std::cout << ", material='" << material.name << "'";
			// float vec[3] = {0.0f, 1.0f, 2};
			for (int i = 0; i < 3; ++i) {
				group.m_diffuse[i] = material.diffuse[i];
			}

			if (material.diffuse_texname != "") {
//    std::string directory = std::string(path).substr(0, path.find_last_of('/'));
                std::string str = material.diffuse_texname;
                std::string filename = str.substr(0, str.find_last_of('.'));
                std::string extension = str.substr(str.find_last_of('.') + 1);
//                std::string fileName = material.diffuse_texname.substr()
                std::for_each(filename.begin(), filename.end(), [](char & c) {
                    c = std::tolower(c);
                });
//                std::for_each(extension.begin(), extension.end(), [](char & c) {
//                    c = std::tolower(c);
//                });
                group.m_texture =
//                    new Texture(directory + "/" + material.diffuse_texname);
                    new Texture(directory + "/" + filename + "." + extension);
                std::cout << " hasTexture";

				// group.m_hasDiffuseTexture = true;
				group.m_diffuse[3] = 1.0f;
				// std::cout << "DIFFUSE : " << group.m_diffuse[3] << std::endl;
			} else {
				group.m_diffuse[3] = 0.0f;
			}
		}
		// group.m_diffuse = {material.diffuse[0], material.diffuse[1],
		// material.diffuse[2], material.diffuse[3]};
		group.m_uDiffuse =
			bgfx::createUniform("u_diffuse", bgfx::UniformType::Vec4);

		// group.m_uHasDiffuseTexture =
		// bgfx::createUniform("u_hasDiffuseTexture", bgfx::UniformType::Vec4);

		std::cout << std::endl;
		// group.m_texture = new
		// Texture("D:/proto-bgfx/Assets/Teapot/default.png");
	}
	// std::cout << "load mesh done" << std::endl;

	// m_numVertices = vertices.size();
	// m_vertices = (uint8_t *)malloc(sizeof(Vertex) * m_numVertices);
	// *m_vertices = (uint8_t)vertices.data();
	// m_vbh = bgfx::createVertexBuffer(
	// 	bgfx::makeRef(m_vertices, sizeof(Vertex) * m_numVertices),
	// 	m_layout);

	// m_numIndices = triangle_list.size();
	// // m_indices = std::move(triangle_list.data());
	// m_indices = (uint16_t *)malloc(sizeof(uint16_t) * m_numIndices);
	// *m_indices = (uint16_t)triangle_list.data();
	// m_ibh = bgfx::createIndexBuffer(
	// 	bgfx::makeRef(m_indices, sizeof(uint16_t) * m_numIndices));

	// m_vbh = bgfx::createVertexBuffer(
	// 	bgfx::makeRef(m_vertices.data(), sizeof(Vertex) * m_vertices.size()),
	// 	m_layout);
	// m_ibh = bgfx::createIndexBuffer(
	// 	bgfx::makeRef(m_indices.data(), sizeof(uint16_t) * m_indices.size()));

	// std::cout << m_vertices << std::endl;
}

void Mesh::submit(bgfx::ViewId id, bgfx::ProgramHandle program,
				  const float *mtx, uint64_t state) {
	// bgfx::setTransform(mtx);
	// bgfx::setState(state);

	for (GroupArray::const_iterator it = m_groups.begin(),
									itEnd = m_groups.end();
		 it != itEnd; ++it) {
		const Group &group = *it;
		// if (group.m_texture == nullptr) {
		// bgfx::set
		// continue;
		// }

		bgfx::setTransform(mtx);
		bgfx::setState(state);

		// if (group.m_texture != nullptr) {
		bgfx::setUniform(group.m_uDiffuse, group.m_diffuse);

		if (group.m_texture != nullptr) {
			// uint32_t flags = UINT32_MAX;
			// uint32_t flags = BGFX_TEXTURE_USE_DEFAULT;

			// const uint64_t textureFlags =
				// 0 | BGFX_CAPS_FORMAT_TEXTURE_MIP_AUTOGEN;
			// const uint64_t textureFlags = 0 | BGFX_TEXTURE_NONE;
			// const uint64_t samplerFlags = 0 | BGFX_SAMPLER_NONE;
			bgfx::setTexture(0, group.m_texture->m_sampler,
							 group.m_texture->m_texture);
							//  textureFlags | samplerFlags);

			// bgfx::setUniform(group.m_uHasDiffuseTexture, (void*)true);
		}
		// else {
		// bgfx::setTexture(0, group.m_texture->m_sampler, bgfx::Tex)
		// }

		// }
		// else {
		//     continue;
		// }

		bgfx::setIndexBuffer(group.m_ibh);
		bgfx::setVertexBuffer(0, group.m_vbh);
		// bgfx::submit(id, program, 0, it != itEnd - 1);
		bgfx::submit(id, program);
		// std::cout << "submit" << std::endl;
	}

	// bgfx::setIndexBuffer(m_ibh);
	// bgfx::setVertexBuffer(0, m_vbh);
	// bgfx::submit(id, program);
}

Mesh::~Mesh() {
	// delete m_vertices;
	// m_vertices = nullptr;
	// delete m_indices;
	// m_indices = nullptr;
	// bgfx::destroy(m_vbh);
	// bgfx::destroy(m_ibh);
	// bgfx::destroy(m_layout);

	for (GroupArray::const_iterator it = m_groups.begin(),
									itEnd = m_groups.end();
		 it != itEnd; ++it) {
		const Group &group = *it;
		bgfx::destroy(group.m_vbh);

		bgfx::destroy(group.m_uDiffuse);

		if (bgfx::isValid(group.m_ibh)) {
			bgfx::destroy(group.m_ibh);
		}

		// if (NULL != group.m_vertices) {
		// 	BX_FREE(allocator, group.m_vertices);
		// }

		// if (NULL != group.m_indices) {
		// 	BX_FREE(allocator, group.m_indices);
		// }
	}
	m_groups.clear();
}
