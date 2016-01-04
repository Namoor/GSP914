#pragma once


enum TilingTerrainType
{
	Snow = 0,
	Grass = 1,
	City = 2,
	Stone = 3
};


class TilingTerrain_Data
{
public:
	void Init(int SizeX, int SizeY, TilingTerrainType p_DefaultType);

	void SetData(int x, int y, TilingTerrainType p_Value);
	TilingTerrainType GetData(int x, int y);

private:
	TilingTerrainType* m_pData;
	int m_SizeX;
	int m_SizeY;
};