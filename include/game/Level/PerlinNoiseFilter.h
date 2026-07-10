#pragma once

#include "common.h"

class PerlinNoiseFilter
{
private:
	int m_Octave;
public:
	PerlinNoiseFilter(int octave);
	~PerlinNoiseFilter();
public:
	int* read(int width, int height, int *out);
};