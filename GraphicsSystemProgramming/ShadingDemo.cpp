#include "ShadingDemo.h"
#include "ShadingDemo_Structs.h"

#define PLANEVERTEXCOUNTPERDIMENSION 2

void ShadingDemo::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pDevice = p_pDevice;
	m_pDevCon = p_pDevCon;



	// Vertex Buffer
	const int VertexCount = PLANEVERTEXCOUNTPERDIMENSION * PLANEVERTEXCOUNTPERDIMENSION;

	D3D11_BUFFER_DESC _VBDesc;
	ZeroMemory(&_VBDesc, sizeof(_VBDesc));

	_VBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBDesc.ByteWidth = VertexCount * sizeof(ShadingDemo_Vertex);
	_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_VBDesc, nullptr, &m_pVertexBuffer);

	ShadingDemo_Vertex _Vertices[VertexCount];

	int _CurrentIndex = 0;

	for (int Y = 0; Y < PLANEVERTEXCOUNTPERDIMENSION; Y++)
		for (int X = 0; X < PLANEVERTEXCOUNTPERDIMENSION; X++)
		{
			float _X = X / (PLANEVERTEXCOUNTPERDIMENSION - 1);
			float _Y = Y / (PLANEVERTEXCOUNTPERDIMENSION - 1);

			float _XP = _X * 2 - 1;
			float _YP = _Y * 2 - 1;

			_Vertices[_CurrentIndex].Position = D3DXVECTOR3(_XP, GetHeightAt(_XP, _YP), _YP);
			_Vertices[_CurrentIndex].Normal = D3DXVECTOR3(0, 1, 0);
			_Vertices[_CurrentIndex].UV = D3DXVECTOR2(_X, _Y);

			_CurrentIndex++;
		}

	D3D11_MAPPED_SUBRESOURCE _VBMSR;

	m_pDevCon->Map(m_pVertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_VBMSR);
	memcpy(_VBMSR.pData, &_Vertices, VertexCount * sizeof(ShadingDemo_Vertex));
	m_pDevCon->Unmap(m_pVertexBuffer, 0);
	

	// IndexBuffer

	D3D11_BUFFER_DESC _IBDesc;
	ZeroMemory(&_IBDesc, sizeof(_IBDesc));

	_IBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IBDesc.ByteWidth = 6 * sizeof(unsigned int);
	_IBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_IBDesc, nullptr, &m_pIndexBuffer);

	unsigned int _Indices[6];

	// Dreieck0
	_Indices[0] = 0;
	_Indices[1] = 2;
	_Indices[2] = 3;

	// Dreieck1
	_Indices[3] = 0;
	_Indices[4] = 3;
	_Indices[5] = 1;

	D3D11_MAPPED_SUBRESOURCE _IBMSR;

	m_pDevCon->Map(m_pIndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_IBMSR);
	memcpy(_IBMSR.pData, &_Indices, 6 * sizeof(unsigned int));
	m_pDevCon->Unmap(m_pIndexBuffer, 0);


	
	// VertexShader
	ID3D10Blob* _pVertexShader;
	ID3D10Blob* _pError;

	if (D3DX11CompileFromFile("ShadingDemo.hlsl", nullptr, nullptr, "VShader", "vs_5_0", 0, 0, nullptr,
		&_pVertexShader, &_pError, nullptr) != S_OK)
	{
		MessageBox(nullptr, (char*)_pError->GetBufferPointer(), "Fehler im VertexShader", MB_OK);

		PostQuitMessage(0);
		return;
	}

	m_pDevice->CreateVertexShader(_pVertexShader->GetBufferPointer(), _pVertexShader->GetBufferSize(), nullptr, &m_pVertexShader);
	
	
	// PixelShader
	ID3D10Blob* _pPixelShader;
	
	if (D3DX11CompileFromFile("ShadingDemo.hlsl", nullptr, nullptr, "PShader", "ps_5_0", 0, 0, nullptr,
		&_pPixelShader, &_pError, nullptr) != S_OK)
	{
		MessageBox(nullptr, (char*)_pError->GetBufferPointer(), "Fehler im PixelShader", MB_OK);

		PostQuitMessage(0);
		return;
	}

	m_pDevice->CreatePixelShader(_pPixelShader->GetBufferPointer(), _pPixelShader->GetBufferSize(), nullptr, &m_pPixelShader);



	// InputLayout

	D3D11_INPUT_ELEMENT_DESC _IED[3];

	_IED[0].SemanticName = "POSITION";
	_IED[0].SemanticIndex = 0;
	_IED[0].AlignedByteOffset = 0;
	_IED[0].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	_IED[0].InputSlot = 0;
	_IED[0].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[0].InstanceDataStepRate = 0;

	_IED[1].SemanticName = "TEXCOORD";
	_IED[1].SemanticIndex = 0;
	_IED[1].AlignedByteOffset = 12;
	_IED[1].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
	_IED[1].InputSlot = 0;
	_IED[1].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[1].InstanceDataStepRate = 0;

	_IED[2].SemanticName = "NORMAL";
	_IED[2].SemanticIndex = 0;
	_IED[2].AlignedByteOffset = 20;
	_IED[2].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	_IED[2].InputSlot = 0;
	_IED[2].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[2].InstanceDataStepRate = 0;

	m_pDevice->CreateInputLayout(_IED, 3, _pVertexShader->GetBufferPointer(), _pVertexShader->GetBufferSize(), &m_pInputLayout);

	// ConstantBufferMatrix
	D3D11_BUFFER_DESC _MatrixBufferDesc;
	ZeroMemory(&_MatrixBufferDesc, sizeof(_MatrixBufferDesc));

	_MatrixBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	_MatrixBufferDesc.ByteWidth = sizeof(ShadingDemo_MatrixConstantBuffer);
	_MatrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_MatrixBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_MatrixBufferDesc, nullptr, &m_pMatrixConstantBuffer);

	// ConstantBufferLight
	D3D11_BUFFER_DESC _LightBufferDesc;
	ZeroMemory(&_LightBufferDesc, sizeof(_LightBufferDesc));

	_LightBufferDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	_LightBufferDesc.ByteWidth = sizeof(ShadingDemo_LightConstantBuffer);
	_LightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_LightBufferDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_LightBufferDesc, nullptr, &m_pLightConstantBuffer);

	// Texture

	D3DX11CreateShaderResourceViewFromFile(m_pDevice, "Dirt.jpg", nullptr, nullptr, &m_pTexture, nullptr);
};

float ShadingDemo::GetHeightAt(float x, float z)
{
	return x * x + z * z;
}

void ShadingDemo::Update(float DeltaTime)
{

}

void ShadingDemo::Render(Camera* p_pCamera)
{
	// Constant Buffer Aktualisieren
	ShadingDemo_LightConstantBuffer _LCB;
	ShadingDemo_MatrixConstantBuffer _MCB;

	_LCB.DirectionalLightDirection = D3DXVECTOR4(0, -1, 0, 0);
	_LCB.DirectionalLightColor = D3DXVECTOR4(1, 1, 1, 0);

	_MCB.MVP = p_pCamera->GetViewMatrix() * p_pCamera->GetProjMatrix();

	D3D11_MAPPED_SUBRESOURCE _MSR;

	m_pDevCon->Map(m_pLightConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MSR);
	memcpy(_MSR.pData, &_LCB, sizeof(_LCB));
	m_pDevCon->Unmap(m_pLightConstantBuffer, 0);

	m_pDevCon->Map(m_pMatrixConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MSR);
	memcpy(_MSR.pData, &_MCB, sizeof(_MCB));
	m_pDevCon->Unmap(m_pMatrixConstantBuffer, 0);

	// Alles Binden

	unsigned int Offset = 0;
	unsigned int Stride = sizeof(ShadingDemo_Vertex);

	m_pDevCon->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	m_pDevCon->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &Stride, &Offset);
	m_pDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pDevCon->IASetInputLayout(m_pInputLayout);

	m_pDevCon->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDevCon->VSSetConstantBuffers(0, 1, &m_pMatrixConstantBuffer);

	m_pDevCon->PSSetShader(m_pPixelShader, nullptr, 0);
	m_pDevCon->PSSetConstantBuffers(0, 1, &m_pLightConstantBuffer);
	m_pDevCon->PSSetShaderResources(0, 1, &m_pTexture);

	// DrawCall
	m_pDevCon->DrawIndexed((PLANEVERTEXCOUNTPERDIMENSION - 1) * (PLANEVERTEXCOUNTPERDIMENSION - 1) * 6, 0, 0);
}



