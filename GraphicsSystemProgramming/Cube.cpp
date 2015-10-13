#include "Cube.h"

#include "Cube_Vertex.h"

void Cube::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pDevice = p_pDevice;
	m_pDevCon = p_pDevCon;

	// VertexBuffer
	D3D11_BUFFER_DESC _VBDesc;
	ZeroMemory(&_VBDesc, sizeof(_VBDesc));

	_VBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBDesc.ByteWidth = sizeof(Cube_Vertex) * 24;
	_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_VBDesc, nullptr, &m_pVertexBuffer);

	Cube_Vertex _Vertices[24];

	_Vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	_Vertices[0].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[0].TexCoord = D3DXVECTOR2(0, 1);

	_Vertices[1].Position = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	_Vertices[1].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[1].TexCoord = D3DXVECTOR2(0, 0);

	_Vertices[2].Position = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	_Vertices[2].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[2].TexCoord = D3DXVECTOR2(1, 0);

	_Vertices[3].Position = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	_Vertices[3].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[3].TexCoord = D3DXVECTOR2(1, 1);

	_Vertices[4].Position = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	_Vertices[4].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[4].TexCoord = D3DXVECTOR2(0, 1);

	_Vertices[5].Position = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	_Vertices[5].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[5].TexCoord = D3DXVECTOR2(0, 0);

	_Vertices[6].Position = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	_Vertices[6].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[6].TexCoord = D3DXVECTOR2(1, 0);

	_Vertices[7].Position = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	_Vertices[7].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[7].TexCoord = D3DXVECTOR2(1, 1);


	_Vertices[8].Position = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	_Vertices[8].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[8].TexCoord = D3DXVECTOR2(0, 1);

	_Vertices[9].Position = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	_Vertices[9].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[9].TexCoord = D3DXVECTOR2(0, 0);

	_Vertices[10].Position = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	_Vertices[10].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[10].TexCoord = D3DXVECTOR2(1, 0);

	_Vertices[11].Position = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	_Vertices[11].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[11].TexCoord = D3DXVECTOR2(1, 1);


	_Vertices[12].Position = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	_Vertices[12].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[12].TexCoord = D3DXVECTOR2(0, 1);

	_Vertices[13].Position = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	_Vertices[13].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[13].TexCoord = D3DXVECTOR2(0, 0);

	_Vertices[14].Position = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	_Vertices[14].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[14].TexCoord = D3DXVECTOR2(1, 0);

	_Vertices[15].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	_Vertices[15].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[15].TexCoord = D3DXVECTOR2(1, 1);



	_Vertices[16].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	_Vertices[16].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[16].TexCoord = D3DXVECTOR2(0, 1);

	_Vertices[17].Position = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	_Vertices[17].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[17].TexCoord = D3DXVECTOR2(0, 0);

	_Vertices[18].Position = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	_Vertices[18].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[18].TexCoord = D3DXVECTOR2(1, 0);

	_Vertices[19].Position = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	_Vertices[19].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[19].TexCoord = D3DXVECTOR2(1, 1);



	_Vertices[20].Position = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	_Vertices[20].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[20].TexCoord = D3DXVECTOR2(0, 1);

	_Vertices[21].Position = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	_Vertices[21].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[21].TexCoord = D3DXVECTOR2(0, 0);

	_Vertices[22].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	_Vertices[22].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[22].TexCoord = D3DXVECTOR2(1, 0);

	_Vertices[23].Position = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	_Vertices[23].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[23].TexCoord = D3DXVECTOR2(1, 1);


	D3D11_MAPPED_SUBRESOURCE _VBM;

	m_pDevCon->Map(m_pVertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_VBM);
	memcpy(_VBM.pData, _Vertices, sizeof(Cube_Vertex) * 24);
	m_pDevCon->Unmap(m_pVertexBuffer, 0);




	// IndexBuffer
	D3D11_BUFFER_DESC _IBDesc;
	ZeroMemory(&_IBDesc, sizeof(_IBDesc));

	_IBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IBDesc.ByteWidth = sizeof(int) * 36;
	_IBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_IBDesc, nullptr, &m_pIndexBuffer);

	UINT32 _Indices[36];

	// Front
	_Indices[0] = 0;
	_Indices[1] = 1;
	_Indices[2] = 2;

	_Indices[3] = 0;
	_Indices[4] = 2;
	_Indices[5] = 3;

	// Right
	_Indices[6] = 4;
	_Indices[7] = 5;
	_Indices[8] = 6;

	_Indices[9] = 4;
	_Indices[10] = 6;
	_Indices[11] = 7;

	// Back
	_Indices[12] = 8;
	_Indices[13] = 9;
	_Indices[14] = 10;

	_Indices[15] = 8;
	_Indices[16] = 10;
	_Indices[17] = 11;

	// Left
	_Indices[18] = 12;
	_Indices[19] = 13;
	_Indices[20] = 14;

	_Indices[21] = 12;
	_Indices[22] = 14;
	_Indices[23] = 15;

	// Top
	_Indices[24] = 16;
	_Indices[25] = 17;
	_Indices[26] = 18;

	_Indices[27] = 16;
	_Indices[28] = 18;
	_Indices[29] = 19;

	// Bottom
	_Indices[30] = 20;
	_Indices[31] = 21;
	_Indices[32] = 22;

	_Indices[33] = 20;
	_Indices[34] = 22;
	_Indices[35] = 23;

	D3D11_MAPPED_SUBRESOURCE _IBM;

	m_pDevCon->Map(m_pIndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_IBM);
	memcpy(_IBM.pData, _Indices, sizeof(int) * 36);
	m_pDevCon->Unmap(m_pIndexBuffer, 0);




	m_pMaterial = new Material();
	m_pMaterial->Init(m_pDevice, m_pDevCon);
}



void Cube::Update(float p_DeltaTime, Camera* p_pCamera)
{
	m_pMaterial->Update(p_DeltaTime, p_pCamera);


}

void Cube::Render()
{
	UINT _Offset = 0;
	UINT _Stride = sizeof(Cube_Vertex);

	m_pDevCon->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &_Stride, &_Offset);
	m_pDevCon->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	m_pDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);







	m_pMaterial->Bind();

	m_pDevCon->DrawIndexed(36, 0, 0);
}