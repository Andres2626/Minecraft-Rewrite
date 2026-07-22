#pragma once
#include <Math/Math.h>

using namespace MC::Math;

struct FogProperties 
{
	vec4 color;
	float density;
};

struct GameProperties {
	ivec3 LevelSize;
	vec3 SkyColor;

	FogProperties fg0;
	FogProperties fg1;
	bool fullscreen;

	GameProperties()
		: LevelSize(256, 64, 256), SkyColor(0.5f, 0.8f, 1.0f)
	{
		fg0.color = vec4(14 / 255.0f, 11 / 255.0f, 10 / 255.0f, 1.0f);
		fg0.density = 0.001f;
		fg1.color = vec4(254 / 255.0f, 251 / 255.0f, 250.0 / 255.0f, 1.0f);
		fg1.density = 0.06f;
		fullscreen = true;
	}
};

extern GameProperties GlobalGP;