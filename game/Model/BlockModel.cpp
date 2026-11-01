#include "Model/BlockModel.h"

#include <Utils/Util.h>

static std::array<ivec3, 4> front = {
	ivec3(1, 0, 0),
	ivec3(0, 0, 0),
	ivec3(0, 1, 0),
	ivec3(1, 1, 0)
};

static std::array<ivec3, 4> back = {
	ivec3(0, 0, 1),
	ivec3(1, 0, 1),
	ivec3(1, 1, 1),
	ivec3(0, 1, 1)
};

static std::array<ivec3, 4> left = {
	ivec3(0, 0, 0),
	ivec3(0, 0, 1),
	ivec3(0, 1, 1),
	ivec3(0, 1, 0)
};

static std::array<ivec3, 4> right = {
	ivec3(1, 0, 1),
	ivec3(1, 0, 0),
	ivec3(1, 1, 0),
	ivec3(1, 1, 1)
};

static std::array<ivec3, 4> bottom = {
	ivec3(0, 0, 0),
	ivec3(1, 0, 0),
	ivec3(1, 0, 1),
	ivec3(0, 0, 1)
};

static std::array<ivec3, 4> top = {
	ivec3(0, 1, 0),
	ivec3(0, 1, 1),
	ivec3(1, 1, 1),
	ivec3(1, 1, 0)
};

glm::ivec3 FaceNormals[] = {
	{ 0, 0, -1 },
	{ 0, 0, 1 },
	{ -1, 0, 0 },
	{ 1, 0, 0 },
	{ 0, -1, 0 },
	{ 0, 1, 0 }
};

BlockModel::BlockModel()
{

}

BlockModel::~BlockModel()
{

}

void BlockModel::Build()
{
	mc_assert(false, "not implemented function");
}

void BlockModel::Bind() const
{
	mc_assert(false, "not implemented function");
}

void BlockModel::Unbind() const
{
	mc_assert(false, "not implemented function");
}

ivec3 *BlockModel::GetBlockFace(Face f) const
{
	switch (f)
	{
	case Face::FRONT:
		return front.data();
	case Face::BACK:
		return back.data();
	case Face::LEFT:
		return left.data();
	case Face::RIGHT:
		return right.data();
	case Face::BOTTOM:
		return bottom.data();
	case Face::TOP:
		return top.data();
	default:
		mc_assert(false, "invalid block face");
		break;
	}
}

ivec3 &BlockModel::GetBlockFaceNormals(Face f) const
{
	mc_assert(((int)f >= 0 && (int)f <= 5), "invalid block face");
	int idx = (int)f;
	return FaceNormals[idx];
}
