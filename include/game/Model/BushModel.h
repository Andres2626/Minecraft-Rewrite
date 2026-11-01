#pragma once

#include "IModel.h"

#include <Graphics/Mesh.h>
#include <Graphics/InstancedMesh.h>
#include <Graphics/Shader/ShaderManager.h>

#include <memory>

using namespace MC;
using namespace Math;
using namespace Graphics;

class BushModel : public IInstancedModel
{
private:
	u32t m_IndexCount;
	std::unique_ptr<Mesh> m_Mesh;
	std::unique_ptr<InstancedMesh> m_InsMesh;
public:
	BushModel();
	~BushModel() override;
public:
	void Build() override;
public:
	void Bind() const override;
	void Unbind() const override;
public:
	u32t GetIndexCount() const override;
	InstancedMesh &GetInstancedMesh() const override;
};