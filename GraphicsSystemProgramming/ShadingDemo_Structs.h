#pragma once

#include "d3dinclude.h"

struct ShadingDemo_Vertex
{
	D3DXVECTOR3 Position;	// 3 * 4b = 12 bytes
	D3DXVECTOR2 UV;			// 2 * 4b = 8 bytes
	D3DXVECTOR3 Normal;
};


struct ShadingDemo_LightConstantBuffer
{
	// Directional Light
	D3DXVECTOR4 DirectionalLightDirection;
	D3DXVECTOR4 DirectionalLightColor;

	D3DXVECTOR4 PointLightPosition;
	D3DXVECTOR4 PointLightColor;
};

struct ShadingDemo_MatrixConstantBuffer
{
	D3DXMATRIX MVP;
};