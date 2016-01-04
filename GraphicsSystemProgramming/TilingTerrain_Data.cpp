#include "TilingTerrain_Data.h"

void TilingTerrain_Data::Init(int SizeX, int SizeY, TilingTerrainType p_DefaultType)
{
	m_SizeX = SizeX;
	m_SizeY = SizeY;

	m_pData = new TilingTerrainType[m_SizeX * m_SizeY];

	for (int x = 0; x < m_SizeX; x++)
	{
		for (int y = 0; y < m_SizeY; y++)
		{
			SetData(x, y, p_DefaultType);
		}
	}
}

void TilingTerrain_Data::SetData(int x, int y, TilingTerrainType p_Value)
{
	m_pData[x * m_SizeY + y] = p_Value;
}

TilingTerrainType TilingTerrain_Data::GetData(int x, int y)
{
	return m_pData[x * m_SizeY + y];
}
