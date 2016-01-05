#include "TilingTerrain.h"
#include "TilingTerrain_Structs.h"

void TilingTerrain::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, int SizeX, int SizeY, TilingTerrain_Data* p_pData)
{
	m_pDevice = p_pDevice;
	m_pDeviceContext = p_pDevCon;

	D3DX11CreateShaderResourceViewFromFile(m_pDevice, TEXT("wc3_atlas.png"), nullptr, nullptr, &m_pTexture, nullptr);

	m_pGrass = new TilingTerrain_Layer(0, 0.0f, 1.0f, 0.25f, true, TilingTerrainType::Grass);
	m_pSnow = new TilingTerrain_Layer(0, 0.25f, 1.0f, 0.25f, true, TilingTerrainType::Snow);
	m_pStone = new TilingTerrain_Layer(0, 0.5f, 1.0f, 0.25f, true, TilingTerrainType::Stone);
	m_pCity = new TilingTerrain_Layer(0, 0.75f, 1.0f, 0.25f, false, TilingTerrainType::City);


	int _QuadCount = SizeX * SizeY;

	int _VertexCount = _QuadCount * 4;
	m_IndexCount = _QuadCount * 6;


	TilingTerrain_Vertex* _VertexArray = new TilingTerrain_Vertex[_VertexCount];

	// Datenbefüllen
	for (int y = 0; y < SizeY; y++)
	{
		for (int x = 0; x < SizeX; x++)
		{
			int _VertexBufferOffset = 4 * (x + y * SizeX);

			TilingTerrainType _TL = p_pData->GetData(x, y);
			TilingTerrainType _TR = p_pData->GetData(x + 1, y);
			TilingTerrainType _BL = p_pData->GetData(x, y + 1);
			TilingTerrainType _BR = p_pData->GetData(x + 1, y + 1);

			D3DXVECTOR2 _SnowTexCoords = m_pSnow->GetTexCoords(_TL, _TR, _BL, _BR, false);
			D3DXVECTOR2 _GrassTexCoords = m_pGrass->GetTexCoords(_TL, _TR, _BL, _BR, false);
			D3DXVECTOR2 _CityTexCoords = m_pCity->GetTexCoords(_TL, _TR, _BL, _BR, false);
			D3DXVECTOR2 _StoneTexCoords = m_pStone->GetTexCoords(_TL, _TR, _BL, _BR, true);


			float PixelOffsetX = 0.5f / 512.0f;
			float PixelOffsetY = 0.5f / 1024.0f;

			_VertexArray[_VertexBufferOffset + 0].m_Position = D3DXVECTOR3(x, 0, y);

			_VertexArray[_VertexBufferOffset + 1].m_Position = D3DXVECTOR3(x + 1, 0, y);

			_VertexArray[_VertexBufferOffset + 2].m_Position = D3DXVECTOR3(x, 0, y + 1);

			_VertexArray[_VertexBufferOffset + 3].m_Position = D3DXVECTOR3(x + 1, 0, y + 1);




			_VertexArray[_VertexBufferOffset + 0].m_UV0 = _SnowTexCoords + D3DXVECTOR2(PixelOffsetX, PixelOffsetY);
			_VertexArray[_VertexBufferOffset + 1].m_UV0 = _SnowTexCoords + D3DXVECTOR2(1.0f / 8.0f, 0) + D3DXVECTOR2(-PixelOffsetX, PixelOffsetY);
			_VertexArray[_VertexBufferOffset + 2].m_UV0 = _SnowTexCoords + D3DXVECTOR2(0, 1.0f / 16.0f) + D3DXVECTOR2(PixelOffsetX, -PixelOffsetY);
			_VertexArray[_VertexBufferOffset + 3].m_UV0 = _SnowTexCoords + D3DXVECTOR2(1.0f / 8.0f, 1.0f / 16.0f) + D3DXVECTOR2(-PixelOffsetX, -PixelOffsetY);
		
			_VertexArray[_VertexBufferOffset + 0].m_UV1 = _GrassTexCoords + D3DXVECTOR2(PixelOffsetX, PixelOffsetY);
			_VertexArray[_VertexBufferOffset + 1].m_UV1 = _GrassTexCoords + D3DXVECTOR2(1.0f / 8.0f, 0) + D3DXVECTOR2(-PixelOffsetX, PixelOffsetY);
			_VertexArray[_VertexBufferOffset + 2].m_UV1 = _GrassTexCoords + D3DXVECTOR2(0, 1.0f / 16.0f) + D3DXVECTOR2(PixelOffsetX, -PixelOffsetY);
			_VertexArray[_VertexBufferOffset + 3].m_UV1 = _GrassTexCoords + D3DXVECTOR2(1.0f / 8.0f, 1.0f / 16.0f) + D3DXVECTOR2(-PixelOffsetX, -PixelOffsetY);

			_VertexArray[_VertexBufferOffset + 0].m_UV2 = _CityTexCoords + D3DXVECTOR2(PixelOffsetX, PixelOffsetY);
			_VertexArray[_VertexBufferOffset + 1].m_UV2 = _CityTexCoords + D3DXVECTOR2(1.0f / 8.0f, 0) + D3DXVECTOR2(-PixelOffsetX, PixelOffsetY);
			_VertexArray[_VertexBufferOffset + 2].m_UV2 = _CityTexCoords + D3DXVECTOR2(0, 1.0f / 16.0f) + D3DXVECTOR2(PixelOffsetX, -PixelOffsetY);
			_VertexArray[_VertexBufferOffset + 3].m_UV2 = _CityTexCoords + D3DXVECTOR2(1.0f / 8.0f, 1.0f / 16.0f) + D3DXVECTOR2(-PixelOffsetX, -PixelOffsetY);

			_VertexArray[_VertexBufferOffset + 0].m_UV3 = _StoneTexCoords + D3DXVECTOR2(PixelOffsetX, PixelOffsetY);
			_VertexArray[_VertexBufferOffset + 1].m_UV3 = _StoneTexCoords + D3DXVECTOR2(1.0f / 8.0f, 0) + D3DXVECTOR2(-PixelOffsetX, PixelOffsetY);
			_VertexArray[_VertexBufferOffset + 2].m_UV3 = _StoneTexCoords + D3DXVECTOR2(0, 1.0f / 16.0f) + D3DXVECTOR2(PixelOffsetX, -PixelOffsetY);
			_VertexArray[_VertexBufferOffset + 3].m_UV3 = _StoneTexCoords + D3DXVECTOR2(1.0f / 8.0f, 1.0f / 16.0f) + D3DXVECTOR2(-PixelOffsetX, -PixelOffsetY);

		}
	}
	
	// Buffer erzeugen
	D3D11_BUFFER_DESC _VBDesc;
	ZeroMemory(&_VBDesc, sizeof(_VBDesc));

	_VBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBDesc.ByteWidth = _VertexCount * sizeof(TilingTerrain_Vertex);
	_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_VBDesc, nullptr, &m_pVertexBuffer);

	// Map/Unmap
	D3D11_MAPPED_SUBRESOURCE _VBMapped;

	m_pDeviceContext->Map(m_pVertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_VBMapped);
	memcpy(_VBMapped.pData, _VertexArray, _VertexCount * sizeof(TilingTerrain_Vertex));
	m_pDeviceContext->Unmap(m_pVertexBuffer, 0);

	delete[] _VertexArray;



	// IndexBuffer
	unsigned int* _Indices = new unsigned int[m_IndexCount];

	for (int i = 0; i < _QuadCount; i++)
	{
		_Indices[i * 6 + 0] = 0 + i * 4;
		_Indices[i * 6 + 1] = 2 + i * 4;
		_Indices[i * 6 + 2] = 1 + i * 4;
		_Indices[i * 6 + 3] = 1 + i * 4;
		_Indices[i * 6 + 4] = 2 + i * 4;
		_Indices[i * 6 + 5] = 3 + i * 4;
	}

	// Buffer erzeugen
	D3D11_BUFFER_DESC _IBDesc;
	ZeroMemory(&_IBDesc, sizeof(_IBDesc));

	_IBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IBDesc.ByteWidth = m_IndexCount * sizeof(int);
	_IBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_IBDesc, nullptr, &m_pIndexBuffer);

	// Map/Unmap
	D3D11_MAPPED_SUBRESOURCE _IBMapped;

	m_pDeviceContext->Map(m_pIndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_IBMapped);
	memcpy(_IBMapped.pData, _Indices, m_IndexCount * sizeof(int));
	m_pDeviceContext->Unmap(m_pIndexBuffer, 0);



	delete[] _Indices;



	// Shader
	ID3D10Blob* _pError;
	ID3D10Blob* _pVertexShader;
	ID3D10Blob* _pPixelShader;

	if (D3DX11CompileFromFile("TilingTerrainShader.hlsl", nullptr, nullptr, "VShader", "vs_5_0", 0, 0, nullptr, &_pVertexShader, &_pError, nullptr) != S_OK)
	{
		// Fehlerbehandlung
		MessageBox(0, (char*)_pError->GetBufferPointer(), "Fehler im VertexShader", MB_OK);
		PostQuitMessage(0);
		return;
	}

	m_pDevice->CreateVertexShader(_pVertexShader->GetBufferPointer(), _pVertexShader->GetBufferSize(), nullptr, &m_pVertexShader);


	if (D3DX11CompileFromFile("TilingTerrainShader.hlsl", nullptr, nullptr, "PShader", "ps_5_0", 0, 0, nullptr, &_pPixelShader, &_pError, nullptr) != S_OK)
	{
		// Fehlerbehandlung
		MessageBox(0, (char*)_pError->GetBufferPointer(), "Fehler im PixelShader", MB_OK);
		PostQuitMessage(0);
		return;
	}

	m_pDevice->CreatePixelShader(_pPixelShader->GetBufferPointer(), _pPixelShader->GetBufferSize(), nullptr, &m_pPixelShader);


	D3D11_INPUT_ELEMENT_DESC _IED[5];
	_IED[0].AlignedByteOffset = 0;
	_IED[0].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	_IED[0].SemanticName = "POSITION";
	_IED[0].SemanticIndex = 0;
	_IED[0].InputSlot = 0;
	_IED[0].InstanceDataStepRate = 0;
	_IED[0].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;

	_IED[1].AlignedByteOffset = 12;
	_IED[1].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
	_IED[1].SemanticName = "TEXCOORD";
	_IED[1].SemanticIndex = 0;
	_IED[1].InputSlot = 0;
	_IED[1].InstanceDataStepRate = 0;
	_IED[1].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;

	_IED[2].AlignedByteOffset = 20;
	_IED[2].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
	_IED[2].SemanticName = "TEXCOORD";
	_IED[2].SemanticIndex = 1;
	_IED[2].InputSlot = 0;
	_IED[2].InstanceDataStepRate = 0;
	_IED[2].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;

	_IED[3].AlignedByteOffset = 28;
	_IED[3].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
	_IED[3].SemanticName = "TEXCOORD";
	_IED[3].SemanticIndex = 2;
	_IED[3].InputSlot = 0;
	_IED[3].InstanceDataStepRate = 0;
	_IED[3].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;

	_IED[4].AlignedByteOffset = 36;
	_IED[4].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
	_IED[4].SemanticName = "TEXCOORD";
	_IED[4].SemanticIndex = 3;
	_IED[4].InputSlot = 0;
	_IED[4].InstanceDataStepRate = 0;
	_IED[4].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;

	m_pDevice->CreateInputLayout(_IED, 5, _pVertexShader->GetBufferPointer(), _pVertexShader->GetBufferSize(), &m_pInputLayout);


	D3D11_BUFFER_DESC _MBDesc;
	ZeroMemory(&_MBDesc, sizeof(_MBDesc));

	_MBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	_MBDesc.ByteWidth = sizeof(TilingTerrain_MatrixBuffer);
	_MBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_MBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_MBDesc, nullptr, &m_pMatrixConstantBuffer);
}



void TilingTerrain::Render(Camera* p_pCamera)
{
	D3DXMATRIX _WVP = p_pCamera->GetViewMatrix() * p_pCamera->GetProjMatrix();


	D3D11_MAPPED_SUBRESOURCE _MBMSR;
	m_pDeviceContext->Map(m_pMatrixConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MBMSR);
	memcpy(_MBMSR.pData, &_WVP, sizeof(D3DXMATRIX));
	m_pDeviceContext->Unmap(m_pMatrixConstantBuffer, 0);


	UINT _Offset = 0;
	UINT _Stride = sizeof(TilingTerrain_Vertex);

	m_pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	m_pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &_Stride, &_Offset);
	m_pDeviceContext->IASetInputLayout(m_pInputLayout);
	m_pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	m_pDeviceContext->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDeviceContext->VSSetConstantBuffers(0, 1, &m_pMatrixConstantBuffer);

	m_pDeviceContext->PSSetShader(m_pPixelShader, nullptr, 0);
	m_pDeviceContext->PSSetShaderResources(0, 1, &m_pTexture);


	m_pDeviceContext->DrawIndexed(m_IndexCount, 0, 0);
}








void TilingTerrain::Update(float p_DeltaTime)
{

}