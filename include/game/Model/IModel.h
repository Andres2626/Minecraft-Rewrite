#pragma once

#include "common.h"

#include <Graphics/Buffers/VertexBuffer.h>

using namespace MC;
using namespace Graphics;

class IModel
{
public:
	virtual void Build() = 0;
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
	virtual u32t GetIndexCount() const = 0;
	virtual VertexBuffer& GetInstanceBuffer() = 0;
};

enum class ModelType : u32t
{
	/* dynamic models */
	ZOMBIE,
	PARTICLE,

	/* static models */
	BLOCK,
	BUSH,

	END
};
