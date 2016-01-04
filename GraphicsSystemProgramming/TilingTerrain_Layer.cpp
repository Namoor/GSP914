#include "TilingTerrain_Layer.h"




TilingTerrain_Layer::TilingTerrain_Layer(float p_TexX, float p_TexY, float p_TexSizeX, float p_TexSizeY, bool p_Variation, TilingTerrainType p_Type)
{
	m_TexX = p_TexX;
	m_TexY = p_TexY;

	m_TexSizeX = p_TexSizeX;
	m_TexSizeY = p_TexSizeY;

	m_Variataions = p_Variation;
	m_Type = p_Type;
}

D3DXVECTOR2 TilingTerrain_Layer::GetTexCoords(TilingTerrainType p_TL, TilingTerrainType p_TR, TilingTerrainType p_BL, TilingTerrainType p_BR)
{

	int _TileNr = 0; // 0000

	if (m_Type == p_BR) _TileNr |= 1; // 0001
	if (m_Type == p_BL) _TileNr |= 2; // 0010
	if (m_Type == p_TR) _TileNr |= 4; // 0100
	if (m_Type == p_TL) _TileNr |= 8; // 1000


	int TexCoordX = _TileNr % 4;
	int TexCoordY = _TileNr / 4;

	// TODO: Random Tiles

	float _TexX = m_TexSizeX * 0.125f * TexCoordX + m_TexX;
	float _TexY = m_TexSizeY * 0.25f * TexCoordY + m_TexY;

	return D3DXVECTOR2(_TexX, _TexY);
}