#include "PerlinNoiseOctaves.h"

PerlinNoiseOctave::PerlinNoiseOctave()
{
	m_pPerlinNoises = new PerlinNoise2D*[6];
}

void PerlinNoiseOctave::Init()
{
	for (int x = 0; x < 6; x++)
	{
		m_pPerlinNoises[x] = new PerlinNoise2D();
		m_pPerlinNoises[x]->Init();
	}
}

float PerlinNoiseOctave::GetValue(float x, float y)
{
	float ReturnValue = 0;

	ReturnValue += m_pPerlinNoises[0]->GetValue(x, y);

	ReturnValue += m_pPerlinNoises[1]->GetValue(x * 0.5f, y * 0.5f) * 2;

	ReturnValue += m_pPerlinNoises[2]->GetValue(x * 0.25f, y * 0.25f) * 4;

	ReturnValue += m_pPerlinNoises[3]->GetValue(x * 0.125f, y * 0.125f) * 8;

	ReturnValue += m_pPerlinNoises[4]->GetValue(x * 0.0625f, y * 0.0625f) * 16;

	ReturnValue += m_pPerlinNoises[5]->GetValue(x * 0.03125f, y * 0.03125f) * 32;

	return ReturnValue / 4;
}
