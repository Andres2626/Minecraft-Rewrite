#include "Block/Block.h"

#include "Entity/EntityManager.h"

Block::Block(const BlockType& id, u32t texid)
	: m_ID(id), m_UpdateFlag(0), m_TexID(texid)
{
}

Block::~Block()
{

}

void Block::Update(Level *lev, const ivec3 &pos, Random &random)
{

}

void Block::OnDestroy(Level *lev, const ivec3 &pos, EntityManager &entities)
{
	/* discard if block is air */
	if (m_ID == BlockType::AIR)
		return;

	int spread = 4;
	for (int offsetX = 0; offsetX < spread; offsetX++) {
		for (int offsetY = 0; offsetY < spread; offsetY++) {
			for (int offsetZ = 0; offsetZ < spread; offsetZ++) {
				float tx = pos.x + (offsetX + 0.5f) / spread;
				float ty = pos.y + (offsetY + 0.5f) / spread;
				float tz = pos.z + (offsetZ + 0.5f) / spread;
				float mx = tx - pos.x - 0.5f;
				float my = ty - pos.y - 0.5f;
				float mz = tz - pos.z - 0.5f;

				auto pl = std::make_unique<Particle>(*lev, vec3(tx, ty, tz), vec3(mx, my, mz), (u32t)m_TexID);
				entities.Register<Particle>(std::move(pl));
			}
		}
	}
}
