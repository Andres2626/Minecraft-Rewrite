#pragma once
#include <Math/Math.h>

using namespace MC::Math;

struct GameProperties {
	ivec3 LevelSize;
	vec3 SkyColor;

	vec4 FogColor;
	float FogStart;
	float FogEnd;

	GameProperties()
		: LevelSize(256, 256, 64), SkyColor(0.5f, 0.8f, 1.0f),
		  FogColor(14 / 255.0f, 11 / 255.0f, 10 / 255.0f, 1.0f),
		  FogStart(-10.0f), FogEnd(20.0f)
	{

	}
};