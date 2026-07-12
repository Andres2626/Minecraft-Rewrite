#pragma once

#include "Cube.h"

#include <Graphics/Shader/ShaderManager.h>
#include <Graphics/Buffers/IndexBuffer.h>
#include <Graphics/Buffers/VertexArray.h>
#include <Graphics/Buffers/VertexBuffer.h>

#define MAX_INSTANCES 1000

struct ZombieInstance
{
	vec3 pos;
	float rotY;
	float timeoff;
	float speed;
	float brig;
};

struct ZombiePart
{
	u32t offset;
	u32t size;
	vec3 rot;
};

class ZombieModel
{
protected:
	u32t m_partOffset;
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VBO;
	std::unique_ptr<VertexBuffer> m_InstanceBuffer;
	std::unique_ptr<IndexBuffer> m_IBO;
	std::vector<ZombiePart> m_Parts;
public:
	ZombieModel();
	~ZombieModel();
public:
	static ZombieModel& Get();
public:
	void Build();
	void PushPart(const Cube& cube);
	void ZombieModel::UpdateInstances(const std::vector<ZombieInstance>& instances);
public:
	void Bind() const;
	void Unbind() const;
public:
	std::vector<ZombiePart>& GetParts() { return m_Parts; };
};
