#pragma once

#include "IModel.h"

#include "Block/Block.h"

class BlockModel : public IModel
{
public:
	BlockModel();
	~BlockModel() override;
public:
	void Build() override;
public:
	void Bind() const override;
	void Unbind() const override;
public:
	ivec3 *GetBlockFace(Face f) const;
	ivec3 &GetBlockFaceNormals(Face f) const;
};