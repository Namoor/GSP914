#pragma once

#include "d3dinclude.h"

const int ChunkSize = 64;

#include "Camera.h"

#include "Chunk_Structs.h"

class Chunk
{
public:
	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon);

	void Update(float p_DeltaTime);

	void Render(Camera* p_pCamera);

	void CreateFace(D3DXVECTOR3 p_Position, D3DXVECTOR3 Right, D3DXVECTOR3 Up,unsigned int* p_pIndices, ChunkVertexStruct* p_pVertices, int FaceIndex);

	int GetBlockAt(int x, int y, int z);


private:
	int m_BlockData[ChunkSize][ChunkSize][ChunkSize];
	int FaceCount;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11InputLayout* m_pInputLayout;
	ID3D11Buffer* m_pConstantBufferMatrix;
	ID3D11Buffer* m_pConstantBufferLight;

	ID3D11ShaderResourceView* m_pDirtTexture;

	float TimeSinceStart;

};