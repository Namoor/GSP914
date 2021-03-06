#pragma once

#include "d3dinclude.h"

struct ChunkVertexStruct
{
	D3DXVECTOR3 Position;	// IED0
	D3DXVECTOR2 UV;			// IED1
	D3DXVECTOR3 Normal;		// IED2
};

struct ChunkConstantBuffer
{
	D3DXVECTOR4 LightDir;
	D3DXVECTOR4 RGBLightColor_ALightIntensity;
};