#pragma once

#include "TilingTerrain_Data.h"
#include "d3dinclude.h"


class TilingTerrain_Layer
{
private:
	float m_TexX;
	float m_TexY;

	float m_TexSizeX;
	float m_TexSizeY;

	bool m_Variataions;
	TilingTerrainType m_Type;


public:
	TilingTerrain_Layer(float p_TexX, float p_TexY, float p_TexSizeX, float p_TexSizeY, bool p_Variation, TilingTerrainType m_Type);

	D3DXVECTOR2 GetTexCoords(TilingTerrainType p_TL, TilingTerrainType p_TR, TilingTerrainType p_BL, TilingTerrainType p_BR, bool Fill);
};

