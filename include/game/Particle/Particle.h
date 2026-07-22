#pragma once

#include <Level/Level.h>

class Particle : public Entity
{
public:
	u32t m_Tex;
	vec2 m_TextureOff;
public:
	Particle(Level &level, const vec3 &pos, const vec3 &motion, u32t tex);
	~Particle();
public:
	void Update() override;
};