#pragma once



class PerlinNoise2D
{
public:
	PerlinNoise2D();
	void Init();

	float GetValue(float x, float y);

private:
	float* m_pNoise;
};