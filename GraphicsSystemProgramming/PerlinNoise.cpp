#include "PerlinNoise.h"

#include <iostream>


PerlinNoise2D::PerlinNoise2D()
{
	m_pNoise = new float[128 * 128];

	for (int x = 0; x < 128 * 128; x++)
	{
		m_pNoise[x] = 0;
	}
}

void PerlinNoise2D::Init()
{
	for (int x = 0; x < 128 * 128; x++)
	{
		m_pNoise[x] = rand() % 1024 / 1024.0f;
	}
}

float PerlinNoise2D::GetValue(float x, float y)
{
	int _X0 = floor(x);
	int _X1 = _X0 + 1;

	int _Y0 = floor(y);
	int _Y1 = _Y0 + 1;


	float _X0Y0 = m_pNoise[_X0 + _Y0 * 128];
	float _X0Y1 = m_pNoise[_X0 + _Y1 * 128];
	float _X1Y0 = m_pNoise[_X1 + _Y0 * 128];
	float _X1Y1 = m_pNoise[_X1 + _Y1 * 128];

	float FactorX = x - _X0;
	float FactorY = y - _Y0;



	float _V0 = FactorX * _X1Y0 + (1 - FactorX) * _X0Y0;
	float _V1 = FactorX * _X1Y1 + (1 - FactorX) * _X0Y1;

	return FactorY * _V1 + (1 - FactorY) * _V0;
}