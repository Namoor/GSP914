#pragma once

#include "PerlinNoise.h"

class PerlinNoiseOctave
{
public:
	PerlinNoiseOctave();

	void Init();

	float GetValue(float x, float y);

private:
	PerlinNoise2D** m_pPerlinNoises;
};