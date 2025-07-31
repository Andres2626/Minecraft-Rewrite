#include "Chunk.h"

#include "Level.h"
#include "Graphics/Shader/Shaderflags.h"
#include "Graphics/Renderer.h"

/* push vertex properties in the array */
#define PUSH_VERTEX(f, brig, vb, ub) \
	this->vertices.push_back(t.f[vb] + fpos.x); \
	this->vertices.push_back(t.f[vb + 1] + fpos.y); \
	this->vertices.push_back(t.f[vb + 2] + fpos.z); \
	this->vertices.push_back(t.texcoords[ub]); \
	this->vertices.push_back(t.texcoords[ub + 1]); \
	this->vertices.push_back(brig)

/* push indices in the array */
#define PUSH_INDICES(c) \
	this->indices.push_back(c); \
	this->indices.push_back(c + 1); \
	this->indices.push_back(c + 2); \
	this->indices.push_back(c + 2); \
	this->indices.push_back(c + 3); \
	this->indices.push_back(c)

/* create block */
Tile rock = Tile(ROCK);
Tile grass = Tile(GRASS);

Chunk::Chunk(Level* level, const glm::ivec3& pos)
   : lev(level), pos(pos), m_Dirty(true)
{
	this->lev = level;
	this->m_Dirty = true;

	/* build chunk box */
	box = MC::Physics::AABB({ pos.x * CHUNK_XYZ, pos.y * CHUNK_XYZ, pos.z * CHUNK_XYZ }, 
		                   { (pos.x * CHUNK_XYZ) + CHUNK_XYZ, (pos.y * CHUNK_XYZ) + CHUNK_XYZ, 
		                     (pos.z * CHUNK_XYZ) + CHUNK_XYZ });

	/* create mesh buffers */
	this->VAO = std::make_unique<MC::Graphics::VertexArray>();
	this->VBO = std::make_unique<MC::Graphics::VertexBuffer>();
	this->IBO = std::make_unique<MC::Graphics::IndexBuffer>();

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
	if (!this->m_Dirty)
		return; /* the chunk has not marked as dirty */

	/* reset indices and vertices vector */
	std::vector<float>().swap(this->vertices);
	std::vector<unsigned int>().swap(this->indices);

	Tile tl = rock;
	for (int cx = 0; cx <= CHUNK_XYZ; cx++) {
		for (int cy = 0; cy <= CHUNK_XYZ; cy++) {
			for (int cz = 0;cz <= CHUNK_XYZ; cz++) {

				/* convert chunk position to global position */
				glm::ivec3 p((pos.x * CHUNK_XYZ) + cx, (pos.y * CHUNK_XYZ) + cy, 
					         (pos.z * CHUNK_XYZ) + cz);

				
				/* The top blocks of the chunk will always be grass (for now) */
				if (p.y == (this->lev->size.z * 2 / 3))
					tl = grass;
				else if (p.y < (this->lev->size.z * 2 / 3) || p.y >= 0)
					tl = rock;

				/*
				* Push all the vertices in the buffer.
				* if the block have adjacent solid blocks ignore this
				* block (render optimization)
				*/
				if (lev->IsSolidTile(p)) {
					if (!lev->IsSolidTile({ p.x + 1, p.y, p.z })) {
						AddFace(p, Face::RIGHT, tl);
					}
					if (!lev->IsSolidTile({ p.x - 1, p.y, p.z })) {
						AddFace(p, Face::LEFT, tl);
					}
					if (!lev->IsSolidTile({ p.x, p.y + 1, p.z })) {
						AddFace(p, Face::TOP, tl);
					}

					if (!lev->IsSolidTile({ p.x, p.y - 1, p.z })) {
						AddFace(p, Face::BOTTOM, tl);
					}
					if (!lev->IsSolidTile({ p.x, p.y, p.z + 1 })) {
						AddFace(p, Face::BACK, tl);
					}

					if (!lev->IsSolidTile({ p.x, p.y, p.z - 1 })) {
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

	this->VAO.reset(new MC::Graphics::VertexArray());
	this->VBO.reset(new MC::Graphics::VertexBuffer());
	this->IBO.reset(new MC::Graphics::IndexBuffer());
	this->VBO->Build(vertices.size() * sizeof(float), vertices.data());
	this->IBO->Build(indices.size() * sizeof(unsigned int), indices.data());
	VL.AddAttribute(SHADER_VERTEX_BIT, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VL.AddAttribute(SHADER_TEX_BIT, 2, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	VL.AddAttribute(SHADER_BRIG_BIT, 1, GL_FLOAT, 6 * sizeof(float), (void*)(5 * sizeof(float)));

	this->VAO->Link(VL);
	this->VAO->Unbind();

	this->m_Dirty = false;
}

void Chunk::Render(MC::Graphics::Shader* shader) const
{
	/* push model matrix */
	shader->Set4x4("s_M", glm::mat4(1.0f));

	/* 
	 * DRAW PROCESS
	 * TODO: Separate the buffers in two parts: light (for light blocks) and
	 * black (oclussion blocks) for avoid fs_in.brightness < 0.6f in fragment 
	 * shader (view assets/Shaders/chunk.shader)
	 */
	this->VAO->Bind();
	MC::Graphics::Renderer::DrawElements(GL_TRIANGLES, IBO->GetSize());
	this->VAO->Unbind();
}

void Chunk::AddFace(const glm::vec3& fpos, Face f, Tile t) 
{
	int count = this->vertices.size() / 6;

	/* brigthness face values */
	float c1 = 1.0f;
	float c2 = 0.8f;
	float c3 = 0.6f;

	/* 
	* Push all specified FACE block in main array. 
	* TODO: Separate buffers.
	*/
	if (f == Face::FRONT) {
		float br = this->lev->GetBrigthness({ fpos.x, fpos.y, fpos.z - 1 }) * c2;
		PUSH_VERTEX(vfront, br, 0, 0);
		PUSH_VERTEX(vfront, br, 3, 2);
		PUSH_VERTEX(vfront, br, 6, 4);
		PUSH_VERTEX(vfront, br, 9, 6);
	}
	else if (f == Face::BACK) {
		float br = this->lev->GetBrigthness({ fpos.x, fpos.y, fpos.z + 1 }) * c2;
		PUSH_VERTEX(vback, br, 0, 0);
		PUSH_VERTEX(vback, br, 3, 2);
		PUSH_VERTEX(vback, br, 6, 4);
		PUSH_VERTEX(vback, br, 9, 6);
	}
	else if (f == Face::LEFT) {
		float br = this->lev->GetBrigthness({ fpos.x - 1, fpos.y, fpos.z }) * c3;
		PUSH_VERTEX(vleft, br, 0, 0);
		PUSH_VERTEX(vleft, br, 3, 2);
		PUSH_VERTEX(vleft, br, 6, 4);
		PUSH_VERTEX(vleft, br, 9, 6);
	}
	else if (f == Face::RIGHT) {
		float br = this->lev->GetBrigthness({ fpos.x + 1, fpos.y, fpos.z }) * c3;
		PUSH_VERTEX(vright, br, 0, 0);
		PUSH_VERTEX(vright, br, 3, 2);
		PUSH_VERTEX(vright, br, 6, 4);
		PUSH_VERTEX(vright, br, 9, 6);
	}
	else if (f == Face::BOTTOM) {
		float br = this->lev->GetBrigthness({ fpos.x, fpos.y - 1, fpos.z }) * c1;
		PUSH_VERTEX(vbottom, br, 0, 0);
		PUSH_VERTEX(vbottom, br, 3, 2);
		PUSH_VERTEX(vbottom, br, 6, 4);
		PUSH_VERTEX(vbottom, br, 9, 6);
	}
	else if (f == Face::TOP) {
		float br = this->lev->GetBrigthness({ fpos.x, fpos.y + 1, fpos.z }) * c1;
		PUSH_VERTEX(vtop, br, 0, 0);
		PUSH_VERTEX(vtop, br, 3, 2);
		PUSH_VERTEX(vtop, br, 6, 4);
		PUSH_VERTEX(vtop, br, 9, 6);
	}

	PUSH_INDICES(count);
}

