#include "Plane.h"


void Plane::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pDevice = p_pDevice;
	m_pDevCon = p_pDevCon;



	PlaneVertexStruct* _Vertices = new PlaneVertexStruct[4];
	unsigned int* _Indices = new unsigned int[6];

	_Vertices[0].Position = D3DXVECTOR3(-2, 0, -2);
	_Vertices[1].Position = D3DXVECTOR3(2, 0, -2);
	_Vertices[2].Position = D3DXVECTOR3(2, 0, 2);
	_Vertices[3].Position = D3DXVECTOR3(-2, 0, 2);

	_Vertices[0].Normal = D3DXVECTOR3(0, 1, 0);
	_Vertices[1].Normal = D3DXVECTOR3(0, 1, 0);
	_Vertices[2].Normal = D3DXVECTOR3(0, 1, 0);
	_Vertices[3].Normal = D3DXVECTOR3(0, 1, 0);

	_Vertices[0].UV = D3DXVECTOR2(0, 1);
	_Vertices[1].UV = D3DXVECTOR2(1, 1);
	_Vertices[2].UV = D3DXVECTOR2(1, 0);
	_Vertices[3].UV = D3DXVECTOR2(0, 0);

	_Indices[0] = 0;
	_Indices[1] = 3;
	_Indices[2] = 2;

	_Indices[3] = 0;
	_Indices[4] = 2;
	_Indices[5] = 1;

	// VertexBuffer erstellen

	D3D11_BUFFER_DESC _VBDesc;
	ZeroMemory(&_VBDesc, sizeof(_VBDesc));

	_VBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBDesc.ByteWidth = 4 * sizeof(PlaneVertexStruct);
	_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_VBDesc, nullptr, &m_pVertexBuffer);

	D3D11_MAPPED_SUBRESOURCE _VBMSR;

	m_pDevCon->Map(m_pVertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_VBMSR);
	memcpy(_VBMSR.pData, _Vertices, 4 * sizeof(PlaneVertexStruct));
	m_pDevCon->Unmap(m_pVertexBuffer, 0);

	delete[] _Vertices;







	// Index Buffer erstellen

	D3D11_BUFFER_DESC _IBDesc;
	ZeroMemory(&_IBDesc, sizeof(_IBDesc));

	_IBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IBDesc.ByteWidth = 6 * sizeof(unsigned int);
	_IBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_IBDesc, nullptr, &m_pIndexBuffer);

	D3D11_MAPPED_SUBRESOURCE _IBMSR;

	m_pDevCon->Map(m_pIndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_IBMSR);
	memcpy(_IBMSR.pData, _Indices, 6 * sizeof(unsigned int));
	m_pDevCon->Unmap(m_pIndexBuffer, 0);

	delete[] _Indices;



	// Shader Laden

	ID3D10Blob* _pVShaderBlob;
	ID3D10Blob* _pPShaderBlob;
	ID3D10Blob* _pErrorBlob;


	// VertexShader
	D3DX11CompileFromFile("PlaneShader.hlsl", nullptr, nullptr, "VShader", "vs_5_0", 0, 0, nullptr, &_pVShaderBlob, &_pErrorBlob, nullptr);
	m_pDevice->CreateVertexShader(_pVShaderBlob->GetBufferPointer(), _pVShaderBlob->GetBufferSize(), nullptr, &m_pVertexShader);

	// PixelShader
	D3DX11CompileFromFile("PlaneShader.hlsl", nullptr, nullptr, "PShader", "ps_5_0", 0, 0, nullptr, &_pPShaderBlob, &_pErrorBlob, nullptr);
	m_pDevice->CreatePixelShader(_pPShaderBlob->GetBufferPointer(), _pPShaderBlob->GetBufferSize(), nullptr, &m_pPixelShader);


	// InputLayout erstellen

	D3D11_INPUT_ELEMENT_DESC _IED[3];

	// Position
	_IED[0].SemanticName = "POSITION";
	_IED[0].SemanticIndex = 0;
	_IED[0].AlignedByteOffset = 0;
	_IED[0].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	_IED[0].InputSlot = 0;
	_IED[0].InstanceDataStepRate = 0;
	_IED[0].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;

	// UV
	_IED[1].SemanticName = "TEXCOORD";
	_IED[1].SemanticIndex = 0;
	_IED[1].AlignedByteOffset = 12;
	_IED[1].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
	_IED[1].InputSlot = 0;
	_IED[1].InstanceDataStepRate = 0;
	_IED[1].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;

	// Normal
	_IED[2].SemanticName = "NORMAL";
	_IED[2].SemanticIndex = 0;
	_IED[2].AlignedByteOffset = 20;
	_IED[2].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	_IED[2].InputSlot = 0;
	_IED[2].InstanceDataStepRate = 0;
	_IED[2].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;

	m_pDevice->CreateInputLayout(_IED, 3, _pVShaderBlob->GetBufferPointer(), _pVShaderBlob->GetBufferSize(), &m_pInputLayout);


	D3DX11CreateShaderResourceViewFromFile(m_pDevice, "Dirt.jpg", nullptr, nullptr, &m_pDirtTexture, nullptr);

	D3D11_BUFFER_DESC _CBDesc;
	ZeroMemory(&_CBDesc, sizeof(_CBDesc));

	_CBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	_CBDesc.ByteWidth = sizeof(D3DXMATRIX);
	_CBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_CBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_CBDesc, nullptr, &m_pConstantBuffer);
}

void Plane::Render(Camera* p_pCamera)
{
	D3DXMATRIX _Mat = p_pCamera->GetViewMatrix() * p_pCamera->GetProjMatrix();

	D3D11_MAPPED_SUBRESOURCE _CBMSR;

	m_pDevCon->Map(m_pConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_CBMSR);
	memcpy(_CBMSR.pData, &_Mat, sizeof(D3DXMATRIX));
	m_pDevCon->Unmap(m_pConstantBuffer, 0);

	UINT stride = sizeof(PlaneVertexStruct);
	UINT offset = 0;

	m_pDevCon->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	m_pDevCon->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pDevCon->IASetInputLayout(m_pInputLayout);
	m_pDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pDevCon->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDevCon->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	m_pDevCon->PSSetShader(m_pPixelShader, nullptr, 0);
	m_pDevCon->PSSetShaderResources(0, 1, &m_pDirtTexture);


	m_pDevCon->DrawIndexed(6, 0, 0);

}

