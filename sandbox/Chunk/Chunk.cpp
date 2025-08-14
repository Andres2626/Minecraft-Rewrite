#include "Chunk/Chunk.h"

#include "Level/Level.h"

#include <Graphics/Shader/Shaderflags.h>
#include <Graphics/Renderer.h>

/* push vertex properties in the array */
#define PUSH_VERTEX(f, brig, vb, ub) \
	vertices.push_back(t.f[vb] + fpos.x); \
	vertices.push_back(t.f[vb + 1] + fpos.y); \
	vertices.push_back(t.f[vb + 2] + fpos.z); \
	vertices.push_back(t.texcoords[ub]); \
	vertices.push_back(t.texcoords[ub + 1]); \
	vertices.push_back(brig)

/* create block */
Tile rock = Tile(ROCK);
Tile grass = Tile(GRASS);

Chunk::Chunk(Level* level, const ivec3& pos)
   : m_Level(level), m_Pos(pos), m_Dirty(true)
{
	m_Level = level;
	m_Dirty = true;

	/* build chunk box */
	m_Box = AABB({ pos.x * CHUNK_XYZ, pos.y * CHUNK_XYZ, pos.z * CHUNK_XYZ }, 
		       { (pos.x * CHUNK_XYZ) + CHUNK_XYZ, (pos.y * CHUNK_XYZ) + CHUNK_XYZ, 
		       (pos.z * CHUNK_XYZ) + CHUNK_XYZ });

	/* create mesh buffers */
	this->VAO = std::make_unique<VertexArray>();
	this->VBO = std::make_unique<VertexBuffer>();
	this->IBO = std::make_unique<IndexBuffer>();

	/* build buffer mesh */
	this->Build();
}

Chunk::~Chunk() 
{
	/* delete pointers */
	this->VAO.reset();
	this->VBO.reset();
	this->IBO.reset();
}

void Chunk::Build() 
{
	/* if the chunk is marked as dirty, rebuild the vertices and indices */
	if (!m_Dirty)
		return; /* if the chunk is not marked at dirty, advoid chunk rebuilding */
	
	/* Increment CUPS by 1 */
	m_Level->IncrementUpdates();

	/* reset indices and vertices vector */
	std::vector<float>().swap(this->vertices);
	std::vector<unsigned int>().swap(this->indices);

	Tile tl = rock;
	for (int cx = 0; cx <= CHUNK_XYZ; cx++) {
		for (int cy = 0; cy <= CHUNK_XYZ; cy++) {
			for (int cz = 0;cz <= CHUNK_XYZ; cz++) {

				/* convert chunk position to global position */
				ivec3 p((m_Pos.x * CHUNK_XYZ) + cx, (m_Pos.y * CHUNK_XYZ) + cy,
					   (m_Pos.z * CHUNK_XYZ) + cz);

				/* The top blocks of the chunk will always be grass (for now) */
				if (p.y == (m_Level->GetSize().z * 2 / 3))
					tl = grass;
				else if (p.y < (m_Level->GetSize().z * 2 / 3) || p.y >= 0)
					tl = rock;

				/*
				 * Push all the vertices in the buffer.
				 * if the block have adjacent solid blocks ignore this
				 * block (render optimization)
				 */
				if (m_Level->IsSolidTile(p)) {
					if (!m_Level->IsSolidTile({ p.x + 1, p.y, p.z })) {
						AddFace(p, Face::RIGHT, tl);
					}
					if (!m_Level->IsSolidTile({ p.x - 1, p.y, p.z })) {
						AddFace(p, Face::LEFT, tl);
					}
					if (!m_Level->IsSolidTile({ p.x, p.y + 1, p.z })) {
						AddFace(p, Face::TOP, tl);
					}

					if (!m_Level->IsSolidTile({ p.x, p.y - 1, p.z })) {
						AddFace(p, Face::BOTTOM, tl);
					}
					if (!m_Level->IsSolidTile({ p.x, p.y, p.z + 1 })) {
						AddFace(p, Face::BACK, tl);
					}

					if (!m_Level->IsSolidTile({ p.x, p.y, p.z - 1 })) {
						AddFace(p, Face::FRONT, tl);
					}
				}
			}
		}
	}

	/* 
	 * BUILD MESH PROCESS 
	 *  -- Create vertex layout object
	 *  -- Reset buffer pointer (VAO, IBO and VBO)
	 *  -- Push all vertices/indices in buffer array
	 *  -- Setup mesh attributes
	 *  -- Link attributes to VAO
	 *  -- Unbind VAO
	 *  -- Mark the chunk as not dirty.
	 */
	MC::Graphics::VertexLayout VL;

	VAO.reset(new VertexArray());
	VBO.reset(new VertexBuffer());
	IBO.reset(new IndexBuffer());
	VBO->Build(vertices.size() * sizeof(float), vertices.data());
	IBO->Build(indices.size() * sizeof(unsigned int), indices.data());
	VL.AddAttribute(SHADER_VERTEX_BIT, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VL.AddAttribute(SHADER_TEX_BIT, 2, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	VL.AddAttribute(SHADER_BRIG_BIT, 1, GL_FLOAT, 6 * sizeof(float), (void*)(5 * sizeof(float)));

	this->VAO->Link(VL);
	this->VAO->Unbind();

	this->m_Dirty = false;
}

void Chunk::Render(Shader* shader) const
{
	/* push model matrix */
	shader->Set4x4("s_M", mat4(1.0f));

	/* DRAW PROCESS */
	VAO->Bind();
	m_Level->IncrementDrawCalls();
	Renderer::DrawElements(GL_TRIANGLES, IBO->GetSize());
	VAO->Unbind();
}

void Chunk::AddFace(const vec3& fpos, Face f, Tile t) 
{
	int count = vertices.size() / 6;

	/* brigthness face values */
	float c1 = 1.0f;
	float c2 = 0.8f;
	float c3 = 0.6f;
	float br = 0.0f;

	/* 
	* Push FACES in main vertex and indices. 
	* TODO: Separate buffers.
	*/
	if (f == Face::FRONT) {
		br = m_Level->GetBrigthness({ fpos.x, fpos.y, fpos.z - 1 }) * c2;
		PUSH_VERTEX(vfront, br, 0, 0);
		PUSH_VERTEX(vfront, br, 3, 2);
		PUSH_VERTEX(vfront, br, 6, 4);
		PUSH_VERTEX(vfront, br, 9, 6);
	}
	else if (f == Face::BACK) {
		br = m_Level->GetBrigthness({ fpos.x, fpos.y, fpos.z + 1 }) * c2;
		PUSH_VERTEX(vback, br, 0, 0);
		PUSH_VERTEX(vback, br, 3, 2);
		PUSH_VERTEX(vback, br, 6, 4);
		PUSH_VERTEX(vback, br, 9, 6);
	}
	else if (f == Face::LEFT) {
		br = m_Level->GetBrigthness({ fpos.x - 1, fpos.y, fpos.z }) * c3;
		PUSH_VERTEX(vleft, br, 0, 0);
		PUSH_VERTEX(vleft, br, 3, 2);
		PUSH_VERTEX(vleft, br, 6, 4);
		PUSH_VERTEX(vleft, br, 9, 6);
	}
	else if (f == Face::RIGHT) {
		br = m_Level->GetBrigthness({ fpos.x + 1, fpos.y, fpos.z }) * c3;
		PUSH_VERTEX(vright, br, 0, 0);
		PUSH_VERTEX(vright, br, 3, 2);
		PUSH_VERTEX(vright, br, 6, 4);
		PUSH_VERTEX(vright, br, 9, 6);
	}
	else if (f == Face::BOTTOM) {
		br = m_Level->GetBrigthness({ fpos.x, fpos.y - 1, fpos.z }) * c1;
		PUSH_VERTEX(vbottom, br, 0, 0);
		PUSH_VERTEX(vbottom, br, 3, 2);
		PUSH_VERTEX(vbottom, br, 6, 4);
		PUSH_VERTEX(vbottom, br, 9, 6);
	}
	else if (f == Face::TOP) {
		br = m_Level->GetBrigthness({ fpos.x, fpos.y + 1, fpos.z }) * c1;
		PUSH_VERTEX(vtop, br, 0, 0);
		PUSH_VERTEX(vtop, br, 3, 2);
		PUSH_VERTEX(vtop, br, 6, 4);
		PUSH_VERTEX(vtop, br, 9, 6);
	}

	PushIndices(count);
}

void Chunk::PushIndices(int count)
{
	indices.push_back(count);
	indices.push_back(count + 1);
	indices.push_back(count + 2);
	indices.push_back(count + 2);
	indices.push_back(count + 3);
	indices.push_back(count);
}

