#pragma once

#include "d3dinclude.h"

const int ChunkSize = 16;

#include "Camera.h"

class Chunk
{
public:
	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon);

	void Render(Camera* p_pCamera);

private:
	int m_BlockData[ChunkSize][ChunkSize][ChunkSize];
};