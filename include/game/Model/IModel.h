#pragma once

#include "common.h"

#include <Graphics/Mesh.h>
#include <Graphics/InstancedMesh.h>

using namespace MC;
using namespace Graphics;

class IModel
{
public:
    virtual ~IModel() = default;
public:
	virtual void Build() = 0;
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
};

class IInstancedModel : public IModel
{
public:
	virtual ~IInstancedModel() = default;
public:
	virtual u32t GetIndexCount() const = 0;
	virtual InstancedMesh &GetInstancedMesh() const = 0;
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
