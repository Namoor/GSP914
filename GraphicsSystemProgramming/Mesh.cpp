#include "Mesh.h"






Mesh::Mesh()
{
	m_pDevCon = nullptr;
	m_pDevice = nullptr;

	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;
}


void Mesh::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, int VertexCount, int IndexCount, int VertexByteSize)
{
	m_pDevice = p_pDevice;
	m_pDevCon = p_pDevCon;

	m_VertexByteSize = VertexByteSize;
	m_VertexCount = VertexCount;
	m_IndexCount = IndexCount;


	// VertexBuffer
	D3D11_BUFFER_DESC _VBDesc;
	ZeroMemory(&_VBDesc, sizeof(_VBDesc));

	_VBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBDesc.ByteWidth = VertexByteSize * m_VertexCount;
	_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_VBDesc, nullptr, &m_pVertexBuffer);



	// IndexBuffer
	D3D11_BUFFER_DESC _IBDesc;
	ZeroMemory(&_IBDesc, sizeof(_IBDesc));

	_IBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IBDesc.ByteWidth = sizeof(int) * m_IndexCount;
	_IBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_IBDesc, nullptr, &m_pIndexBuffer);
}


void Mesh::SetVertexData(void* p_pData)
{
	D3D11_MAPPED_SUBRESOURCE _VBM;

	m_pDevCon->Map(m_pVertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_VBM);
	memcpy(_VBM.pData, p_pData, m_VertexByteSize * m_VertexCount);
	m_pDevCon->Unmap(m_pVertexBuffer, 0);
}

void Mesh::SetIndexData(void* p_pData)
{
	D3D11_MAPPED_SUBRESOURCE _IBM;

	m_pDevCon->Map(m_pIndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_IBM);
	memcpy(_IBM.pData, p_pData, sizeof(int) * m_IndexCount);
	m_pDevCon->Unmap(m_pIndexBuffer, 0);
}

void Mesh::Render(Material* p_pMaterial)
{
	UINT _Offset = 0;
	UINT _Stride = m_VertexByteSize;

	m_pDevCon->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &_Stride, &_Offset);
	m_pDevCon->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	m_pDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	p_pMaterial->Bind();

	m_pDevCon->DrawIndexed(m_IndexCount, 0, 0);
}