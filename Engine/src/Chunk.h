#pragma once

#include "ChunkDef.h"

#include "Tile.h"
#include "Physics/AABB.h"
#include "Graphics/Shader/Shader.h"
#include "Graphics/Buffers/IndexBuffer.h"
#include "Graphics/Buffers/VertexArray.h"
#include "Graphics/Buffers/VertexBuffer.h"

#include <vector>

class Level;

class Chunk {
protected:
	std::unique_ptr<MC::Graphics::VertexArray> VAO;
	std::unique_ptr<MC::Graphics::VertexBuffer> VBO;
	std::unique_ptr<MC::Graphics::IndexBuffer> IBO;
	Level* lev;
public:
	/* VBO buffer */
	std::vector<float> vertices;

	/* IBO buffer */
	std::vector<unsigned int> indices;
private:
	/* Does the chunk have to be rebuilt? */
	bool m_Dirty; 
	glm::ivec3 pos;
	MC::Physics::AABB box;
public:
	Chunk(Level* level, const glm::ivec3& pos);
	~Chunk();
public:
	/* calculate mesh */
	void Build();

	/* draw chunk */
	void Render(MC::Graphics::Shader* shader) const;
private:
	void AddFace(const glm::vec3& fpos, Face f, Tile t);
public:
	inline void SetDirty() { this->m_Dirty = true; }
	inline bool GetDirty() { return this->m_Dirty; }
	inline glm::ivec3 GetPosition() { return this->pos; }
	inline MC::Physics::AABB GetBox() { return this->box; }
};