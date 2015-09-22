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

	D3DX11CreateShaderResourceViewFromFile(m_pDevice, TEXT("TestTexture.jpg"),
		nullptr, nullptr, &m_pTexture, nullptr);

	m_pSpriteBatch = new SpriteBatch();
	m_pSpriteBatch->Init(m_pDevice, m_pDeviceContext);


	VertexStruct Vertices[4];

	Vertices[0].Position = D3DXVECTOR3(-0.5f, -0.5f, 0);
	Vertices[1].Position = D3DXVECTOR3(-0.5f, 0.5f, 0);
	Vertices[2].Position = D3DXVECTOR3(0.5f, 0.5f, 0);
	Vertices[3].Position = D3DXVECTOR3(0.5f, -0.5f, 0);

	Vertices[0].Color = D3DXVECTOR4(1, 1, 1, 1);
	Vertices[1].Color = D3DXVECTOR4(1, 1, 1, 1);
	Vertices[2].Color = D3DXVECTOR4(1, 1, 1, 1);
	Vertices[3].Color = D3DXVECTOR4(1, 1, 1, 1);

	Vertices[0].UV = D3DXVECTOR2(0, 1);
	Vertices[1].UV = D3DXVECTOR2(0, 0);
	Vertices[2].UV = D3DXVECTOR2(1, 0);
	Vertices[3].UV = D3DXVECTOR2(1, 1);



	unsigned int Indices[6];
	// triangle oben links
	Indices[0] = 0;
	Indices[1] = 1;
	Indices[2] = 2;

	// triangle unten rechts
	Indices[3] = 0;
	Indices[4] = 2;
	Indices[5] = 3;




	// -------------------------------------------------------- IndexBuffer --------------------------------------
	m_pIndexBuffer = nullptr;
	D3D11_BUFFER_DESC _IndexBufferDesc;
	ZeroMemory(&_IndexBufferDesc, sizeof(_IndexBufferDesc));

	_IndexBufferDesc.ByteWidth = 6 * sizeof(int);
	_IndexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IndexBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IndexBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_IndexBufferDesc, nullptr, &m_pIndexBuffer);

	D3D11_MAPPED_SUBRESOURCE _MappedResource;
	m_pDeviceContext->Map(m_pIndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MappedResource);
	memcpy(_MappedResource.pData, &Indices, 6 * sizeof(int));
	m_pDeviceContext->Unmap(m_pIndexBuffer, 0);


	// -------------------------------------------------------- VertexBuffer -------------------------------------
	m_pVertexBuffer = nullptr;

	D3D11_BUFFER_DESC _VertexBufferDesc;
	ZeroMemory(&_VertexBufferDesc, sizeof(_VertexBufferDesc));

	_VertexBufferDesc.ByteWidth = 4 * sizeof(VertexStruct);
	_VertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VertexBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VertexBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_VertexBufferDesc, nullptr, &m_pVertexBuffer);

	_MappedResource;
	m_pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MappedResource);
	memcpy(_MappedResource.pData, &Vertices, 4 * sizeof(VertexStruct));
	m_pDeviceContext->Unmap(m_pVertexBuffer, 0);


	m_pPixelShader = nullptr;
	m_pVertexShader = nullptr;

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
		nullptr, &m_pVertexShader);

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
		nullptr, &m_pPixelShader);

	D3D11_INPUT_ELEMENT_DESC _IED[3];
	// identifizierung auf seiten des Shaders
	_IED[0].SemanticName = "POSITION";
	_IED[0].SemanticIndex = 0;
	// identifizierung auf seiten des Buffers
	_IED[0].AlignedByteOffset = 0;
	_IED[0].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;

	_IED[0].InstanceDataStepRate = 0;
	_IED[0].InputSlot = 0;
	_IED[0].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;

	// COLOR
	_IED[1].SemanticName = "COLOR";
	_IED[1].SemanticIndex = 0;
	_IED[1].AlignedByteOffset = 12;
	_IED[1].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	_IED[1].InstanceDataStepRate = 0;
	_IED[1].InputSlot = 0;
	_IED[1].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;

	// UV
	_IED[2].SemanticName = "TEXCOORD";
	_IED[2].SemanticIndex = 0;
	_IED[2].AlignedByteOffset = 28;
	_IED[2].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
	_IED[2].InstanceDataStepRate = 0;
	_IED[2].InputSlot = 0;
	_IED[2].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;


	m_pDevice->CreateInputLayout(_IED, 3, _pVertexShaderBlob->GetBufferPointer(),
		_pVertexShaderBlob->GetBufferSize(), &m_pInputLayout);

}

void Scene2D::Update()
{

}

void Scene2D::Render()
{
	m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);

	m_pDeviceContext->IASetInputLayout(m_pInputLayout);

	m_pDeviceContext->PSSetShaderResources(0, 1, &m_pTexture);

	UINT Stride = sizeof(VertexStruct);
	UINT Offset = 0;

	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer,
		DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &Stride, &Offset);

	m_pDeviceContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//m_pDeviceContext->DrawIndexed(6, 0, 0);


	m_pSpriteBatch->Begin();

	for (int i = 0; i < 200000; i++)
	{
		int x = rand() % 1000;
		int y = rand() % 1000;

		m_pSpriteBatch->DrawTexture(m_pTexture, Rect(x, y, 10, 10), Rect(0, 0, 0, 0), D3DXVECTOR4(1, 1, 1, 1));
	}

	m_pSpriteBatch->End();

}

void Scene2D::Shutdown()
{

}

