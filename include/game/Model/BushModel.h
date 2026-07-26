#pragma once

#include "IModel.h"

#include <Graphics/Mesh.h>
#include <Graphics/Shader/ShaderManager.h>
#include <Graphics/Buffers/IndexBuffer.h>
#include <Graphics/Buffers/VertexArray.h>
#include <Graphics/Buffers/VertexBuffer.h>

using namespace MC;
using namespace Math;
using namespace Graphics;

class BushModel : public IModel
{
private:
	u32t m_IndexCount;
	std::unique_ptr<Mesh> m_Mesh;
	MeshData m_MeshData;
	std::unique_ptr<VertexBuffer> m_InstanceBuffer;
public:
	BushModel();
	~BushModel();
public:
	void Build() override;
public:
	void Bind() const override;
	void Unbind() const override;
public:
	VertexBuffer &GetInstanceBuffer() override;
	u32t GetIndexCount() const override;
};