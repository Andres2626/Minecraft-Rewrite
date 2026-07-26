#pragma once

#include "Model/IModel.h"

#include <Math/Math.h>

#include <unordered_map>
#include <vector>

using namespace MC;
using namespace Math;

struct StaticBlockInstance
{
	vec3 pos;
	float brightness;
};

class StaticModelRenderer
{
private:
	std::unordered_map<ModelType, std::vector<StaticBlockInstance>> m_Instances;
public:
	StaticModelRenderer();
	~StaticModelRenderer();
public:
	void Begin();
	void Sumbit(ModelType type, std::vector<StaticBlockInstance> &instances);
	void Sumbit(ModelType type, StaticBlockInstance &instance);
	void End();
};