#pragma once

#include "d3dinclude.h"

struct TilingTerrain_Vertex
{
	D3DXVECTOR3 m_Position;
	D3DXVECTOR2 m_UV0; // Snow
	D3DXVECTOR2 m_UV1; // Grass
	D3DXVECTOR2 m_UV2; // City
	D3DXVECTOR2 m_UV3; // Stone
};

struct TilingTerrain_MatrixBuffer
{
	D3DXMATRIX m_WVP;
};