#include "2DScene.h"

#include "VertexStruct.h"


Scene2D::Scene2D()
{
	m_pDevice = nullptr;
	m_pDeviceContext = nullptr;
}

Scene2D::~Scene2D()
{

}

void Scene2D::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pDevice = p_pDevice;
	m_pDeviceContext = p_pDevCon;


	VertexStruct Vertices[4];

	Vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0);
	Vertices[1].Position = D3DXVECTOR3(-0.5f, 0.5f, 0);
	Vertices[2].Position = D3DXVECTOR3(0.5f, 0.5f, 0);
	Vertices[3].Position = D3DXVECTOR3(0.5f, -0.5f, 0);


	int Indices[6];
	// triangle oben links
	Indices[0] = 0;
	Indices[1] = 1;
	Indices[2] = 2;

	// triangle unten rechts
	Indices[3] = 0;
	Indices[4] = 2;
	Indices[5] = 3;




	// -------------------------------------------------------- IndexBuffer --------------------------------------
	ID3D11Buffer* _IndexBuffer = nullptr;
	D3D11_BUFFER_DESC _IndexBufferDesc;
	ZeroMemory(&_IndexBufferDesc, sizeof(_IndexBufferDesc));

	_IndexBufferDesc.ByteWidth = 6 * sizeof(int);
	_IndexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IndexBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IndexBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_IndexBufferDesc, nullptr, &_IndexBuffer);

	D3D11_MAPPED_SUBRESOURCE _MappedResource;
	m_pDeviceContext->Map(_IndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MappedResource);
	memcpy(_MappedResource.pData, &Indices, 6 * sizeof(int));
	m_pDeviceContext->Unmap(_IndexBuffer, 0);


	// -------------------------------------------------------- VertexBuffer -------------------------------------
	ID3D11Buffer* _VertexBuffer = nullptr;

	D3D11_BUFFER_DESC _VertexBufferDesc;
	ZeroMemory(&_VertexBufferDesc, sizeof(_VertexBufferDesc));

	_VertexBufferDesc.ByteWidth = 4 * sizeof(VertexStruct);
	_VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VertexBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VertexBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_VertexBufferDesc, nullptr, &_VertexBuffer);

	_MappedResource;
	m_pDeviceContext->Map(_VertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MappedResource);
	memcpy(_MappedResource.pData, &Vertices, 4 * sizeof(VertexStruct));
	m_pDeviceContext->Unmap(_VertexBuffer, 0);


	ID3D11PixelShader* _pPixelShader = nullptr;
	ID3D11VertexShader* _pVertexShader = nullptr;

	ID3D10Blob* _pVertexShaderBlob;
	ID3D10Blob* _pError;

	if (S_OK != D3DX11CompileFromFile(TEXT("UnserErsterShader.hlsl"), nullptr, nullptr, "VShader",
		"vs_5_0", 0, 0, nullptr,
		&_pVertexShaderBlob, &_pError, nullptr))
	{
		MessageBox(0, (char*)_pError->GetBufferPointer(), TEXT("Fehler im VertexShader"), 0);
		PostQuitMessage(1);
		return;
	}

	m_pDevice->CreateVertexShader(_pVertexShaderBlob->GetBufferPointer(), _pVertexShaderBlob->GetBufferSize(),
		nullptr, &_pVertexShader);

	ID3D10Blob* _pPixelShaderBlob;

	if (S_OK != D3DX11CompileFromFile(TEXT("UnserErsterShader.hlsl"), nullptr, nullptr, "PShader",
		"ps_5_0", 0, 0, nullptr,
		&_pPixelShaderBlob, &_pError, nullptr))
	{
		MessageBox(0, (char*)_pError->GetBufferPointer(), TEXT("Fehler im PixelShader"), 0);
		PostQuitMessage(1);
		return;
	}

	m_pDevice->CreatePixelShader(_pPixelShaderBlob->GetBufferPointer(), _pPixelShaderBlob->GetBufferSize(),
		nullptr, &_pPixelShader);

	D3D11_INPUT_ELEMENT_DESC _IED[1];
	// identifizierung auf seiten des Shaders
	_IED[0].SemanticName = "POSITION";
	_IED[0].SemanticIndex = 0;
	// identifizierung auf seiten des Buffers
	_IED[0].AlignedByteOffset = 0;
	_IED[0].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;

	_IED[0].InstanceDataStepRate = 0;
	_IED[0].InputSlot = 0;
	_IED[0].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;

	ID3D11InputLayout* _pInputLayout;
	m_pDevice->CreateInputLayout(_IED, 1, _pVertexShaderBlob->GetBufferPointer(), _pVertexShaderBlob->GetBufferSize(), &_pInputLayout);

}

void Scene2D::Update()
{

}

void Scene2D::Render()
{
	


}

void Scene2D::Shutdown()
{

}

