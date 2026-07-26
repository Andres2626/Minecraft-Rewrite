#pragma once

#include "IModel.h"

#include <Graphics/MeshFactory.h>
#include <Graphics/Shader/ShaderManager.h>
#include <Graphics/Buffers/IndexBuffer.h>
#include <Graphics/Buffers/VertexArray.h>
#include <Graphics/Buffers/VertexBuffer.h>

using namespace MC;
using namespace Math;
using namespace Graphics;

struct ParticleInstance
{
	vec3 pos;
	u32t tex;
	vec2 texOffset;
};

class ParticleModel : public IModel
{
private:
	u32t m_IndexSize;
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<VertexBuffer> m_VBO;
	std::unique_ptr<VertexBuffer> m_InstanceBuffer;
	std::unique_ptr<IndexBuffer> m_IBO;
public:
	ParticleModel();
	~ParticleModel();
public:
	void Build();
public:
	void Bind() const;
	void Unbind() const;
public:
	VertexBuffer &GetInstanceBuffer() override;
	u32t GetIndexCount() const override;
};