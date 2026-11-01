#pragma once

#include "IModel.h"

#include "Character/Cube.h"

#include <Graphics/Mesh.h>
#include <Graphics/Shader/ShaderManager.h>

#include <memory>
#include <vector>

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

class ZombieModel : public IInstancedModel
{
protected:
	u32t m_partOffset;
	u32t m_IndexSize;
	std::unique_ptr<Mesh> m_Mesh;
	std::unique_ptr<InstancedMesh> m_InsMesh;
	std::vector<ZombiePart> m_Parts;
public:
	ZombieModel();
	~ZombieModel() override;
public:
	void Build() override;
	void PushPart(const Cube& cube);
public:
	void Bind() const override;
	void Unbind() const override;
public:
	u32t GetIndexCount() const override;
	InstancedMesh &GetInstancedMesh() const override;
	std::vector<ZombiePart> &GetParts();
};
