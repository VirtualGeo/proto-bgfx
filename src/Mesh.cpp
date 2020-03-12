#include "Mesh.h"

Mesh::Mesh(const char *filePath) {
	// ------------- INIT GEOMETRY
	// bgfx::VertexDecl pcvDecl; // question : out of date ?
	std::string path = std::string(filePath);
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

	const bool ret =
		tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filePath);

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

	printf("# of vertices  = %d\n", (int)(attrib.vertices.size()) / 3);
	printf("# of normals   = %d\n", (int)(attrib.normals.size()) / 3);
	printf("# of texcoords = %d\n", (int)(attrib.texcoords.size()) / 2);
	printf("# of materials = %d\n", (int)materials.size());
	printf("# of shapes    = %d\n", (int)shapes.size());

	// Append `default` material
	// materials.push_back(tinyobj::material_t());

	for (size_t i = 0; i < materials.size(); i++) {
		const auto &mat = materials[i];
		// texture_option_t
		std::cout << "material[" << i << "] name='" << mat.name
				  << "', diffuseName='" << materials[i].diffuse_texname
				  << "', specularName='" << materials[i].specular_texname
				  << "', bumpName='" << materials[i].bump_texname
				  << "', normalName='" << materials[i].normal_texname << "'"
				  << std::endl;
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
	m_groups.resize(shapes.size());

	// m_vertices.clear();
	// m_indices.clear();
	// m_vertices.reserve(1000);
	// m_indices.reserve(1000);

	// attrib.vertices;
	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++) {
		auto &group = m_groups[s];
		auto &vertices = group.m_vertices;
		// vertices.clear();
		auto &indices = group.m_indices;

		// assert(shapes.size() == 3);
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
			int fv = shapes[s].mesh.num_face_vertices[f];

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++) {
				// access to vertex
				// shapes[s].mesh.indices;
				tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
				tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
				tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
				tinyobj::real_t nx = 0.5;
				// attrib.normals[3 * idx.normal_index + 0];
				tinyobj::real_t ny = 0.5;
				// attrib.normals[3 * idx.normal_index + 1];
				tinyobj::real_t nz = 0.5;
				// attrib.normals[3 * idx.normal_index + 2];
				tinyobj::real_t tx =
					attrib.texcoords[2 * idx.texcoord_index + 0];
				tinyobj::real_t ty =
					attrib.texcoords[2 * idx.texcoord_index + 1];

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

		group.m_vbh = bgfx::createVertexBuffer(
			bgfx::makeRef(vertices.data(), sizeof(Vertex) * vertices.size()),
			m_layout);

		group.m_ibh = bgfx::createIndexBuffer(
			bgfx::makeRef(indices.data(), sizeof(uint16_t) * indices.size()));

		// shapes[s].mesh.material_ids[f];

		const int nbMat = shapes[s].mesh.material_ids.size();
		if (nbMat == 0) {
			std::cout << "none material for object" << std::endl;
			exit(1);
		}
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

		const auto &material = materials[shapes[s].mesh.material_ids[0]];

		// tinyobj::material_t * material = nullptr;
		// for (int i =0; i <shapes[s].mesh.material_ids.size(); ++i) {
		//     if ()

		// }

		if (material.diffuse_texname != "") {
			group.m_texture =
				new Texture(directory + "/" + material.diffuse_texname);
		}
		// group.m_texture = new
		// Texture("D:/proto-bgfx/Assets/Teapot/default.png");
	}

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
        if (group.m_texture == nullptr) {
            continue;
        }

		bgfx::setTransform(mtx);
		bgfx::setState(state);

		// if (group.m_texture != nullptr) {
			bgfx::setTexture(0, group.m_texture->m_sampler,
							 group.m_texture->m_texture);

		// }
        // else {
        //     continue;
        // }

		bgfx::setIndexBuffer(group.m_ibh);
		bgfx::setVertexBuffer(0, group.m_vbh);
		// bgfx::submit(id, program, 0, it != itEnd - 1);
		bgfx::submit(id, program);
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