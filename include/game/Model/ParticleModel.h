#pragma once

#include "IModel.h"

#include <Graphics/MeshFactory.h>
#include <Graphics/Shader/ShaderManager.h>

#include <memory>

using namespace MC;
using namespace Math;
using namespace Graphics;

struct ParticleInstance
{
	vec3 pos;
	u32t tex;
	vec2 texOffset;
};

class ParticleModel : public IInstancedModel
{
private:
	u32t m_IndexSize;
	std::unique_ptr<Mesh> m_Mesh;
	std::unique_ptr<InstancedMesh> m_InsMesh;
public:
	ParticleModel();
	~ParticleModel() override;
public:
	void Build() override;
public:
	void Bind() const override;
	void Unbind() const override;
public:
	u32t GetIndexCount() const override;
	InstancedMesh &GetInstancedMesh() const override;
};