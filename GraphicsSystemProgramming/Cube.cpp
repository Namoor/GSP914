#include "Cube.h"

#include "SpriteBatch_Vertex.h"

void Cube::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pDevice = p_pDevice;
	m_pDevCon = p_pDevCon;

	D3DX11CreateShaderResourceViewFromFile(p_pDevice, "Dirt.jpg", nullptr, nullptr, &m_pTexture, nullptr);

	// VertexBuffer
	D3D11_BUFFER_DESC _VBDesc;
	ZeroMemory(&_VBDesc, sizeof(_VBDesc));

	_VBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBDesc.ByteWidth = sizeof(SpriteBatch_Vertex) * 8;
	_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_VBDesc, nullptr, &m_pVertexBuffer);

	SpriteBatch_Vertex _Vertices[8];

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


	_Vertices[4].Position = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	_Vertices[4].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[4].TexCoord = D3DXVECTOR2(1, 1);

	_Vertices[5].Position = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	_Vertices[5].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[5].TexCoord = D3DXVECTOR2(1, 0);

	_Vertices[6].Position = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	_Vertices[6].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[6].TexCoord = D3DXVECTOR2(0, 0);

	_Vertices[7].Position = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	_Vertices[7].Color = D3DXVECTOR4(1, 1, 1, 1);
	_Vertices[7].TexCoord = D3DXVECTOR2(0, 1);

	D3D11_MAPPED_SUBRESOURCE _VBM;

	m_pDevCon->Map(m_pVertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_VBM);
	memcpy(_VBM.pData, _Vertices, sizeof(SpriteBatch_Vertex) * 8);
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
	_Indices[6] = 3;
	_Indices[7] = 2;
	_Indices[8] = 6;

	_Indices[9] = 6;
	_Indices[10] = 7;
	_Indices[11] = 3;

	// Back
	_Indices[12] = 7;
	_Indices[13] = 6;
	_Indices[14] = 5;

	_Indices[15] = 5;
	_Indices[16] = 4;
	_Indices[17] = 7;

	// Left
	_Indices[18] = 5;
	_Indices[19] = 1;
	_Indices[20] = 0;

	_Indices[21] = 5;
	_Indices[22] = 0;
	_Indices[23] = 4;

	// Top
	_Indices[24] = 1;
	_Indices[25] = 5;
	_Indices[26] = 6;

	_Indices[27] = 6;
	_Indices[28] = 2;
	_Indices[29] = 1;

	// Bottom
	_Indices[30] = 0;
	_Indices[31] = 3;
	_Indices[32] = 7;

	_Indices[33] = 0;
	_Indices[34] = 7;
	_Indices[35] = 4;

	D3D11_MAPPED_SUBRESOURCE _IBM;

	m_pDevCon->Map(m_pIndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_IBM);
	memcpy(_IBM.pData, _Indices, sizeof(int) * 36);
	m_pDevCon->Unmap(m_pIndexBuffer, 0);


	// VertexShader
	ID3D10Blob* _pVertexShader;
	ID3D10Blob* _pPixelShader;
	ID3D10Blob* _pError;

	if (D3DX11CompileFromFile("CubeShader.hlsl", nullptr, nullptr, "VShader", "vs_5_0", 0, 0, nullptr,
		&_pVertexShader, &_pError, nullptr) != S_OK)
	{
		MessageBox(0, (char*)_pError->GetBufferPointer(), "Fehler im Vertexshader", 0);
		PostQuitMessage(0);
		return;
	}
	
	m_pDevice->CreateVertexShader(_pVertexShader->GetBufferPointer(), _pVertexShader->GetBufferSize(), nullptr, &m_pVertexShader);


	// PixelShader
	if (D3DX11CompileFromFile("CubeShader.hlsl", nullptr, nullptr, "PShader", "ps_5_0", 0, 0, nullptr,
		&_pPixelShader, &_pError, nullptr) != S_OK)
	{
		MessageBox(0, (char*)_pError->GetBufferPointer(), "Fehler im Pixelshader", 0);
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

	_IED[1].SemanticName = "COLOR";
	_IED[1].SemanticIndex = 0;
	_IED[1].AlignedByteOffset = 12;
	_IED[1].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	_IED[1].InputSlot = 0;
	_IED[1].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[1].InstanceDataStepRate = 0;

	_IED[2].SemanticName = "TEXCOORD";
	_IED[2].SemanticIndex = 0;
	_IED[2].AlignedByteOffset = 28;
	_IED[2].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
	_IED[2].InputSlot = 0;
	_IED[2].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	_IED[2].InstanceDataStepRate = 0;

	m_pDevice->CreateInputLayout(_IED, 3, _pVertexShader->GetBufferPointer(),
		_pVertexShader->GetBufferSize(), &m_pInputLayout);

	
	
	// ConstantBuffer
	D3D11_BUFFER_DESC _CBD;
	ZeroMemory(&_CBD, sizeof(_CBD));


	_CBD.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	_CBD.ByteWidth = 64;
	_CBD.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_CBD.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_CBD, nullptr, &m_pMatrixConstantBuffer);

}



void Cube::Update(float p_DeltaTime)
{
	// WorldMatrix
	D3DXMatrixTranslation(&WorldMatrix, 2, 0.5f, 2);
	

	// ViewMatrix
	D3DXVECTOR3 _Eye(0,2,0);
	D3DXVECTOR3 _At(2, 0.5f, 2);
	D3DXVECTOR3 _Up(0,1,0);

	D3DXMatrixLookAtLH(&ViewMatrix, &_Eye, &_At, &_Up);


	// PerspektiveMatrix
	D3DXMatrixPerspectiveFovLH(&ProjectionMatrix, D3DXToRadian(50), 800.0f / 600.0f, 0.3f, 1000.0f);



	D3DXMATRIX _WVPMatrix = WorldMatrix * ViewMatrix * ProjectionMatrix;

	D3D11_MAPPED_SUBRESOURCE _MM;
	m_pDevCon->Map(m_pMatrixConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MM);
	memcpy(_MM.pData, &_WVPMatrix, 64);
	m_pDevCon->Unmap(m_pMatrixConstantBuffer, 0);
}

void Cube::Render()
{
	UINT _Offset = 0;
	UINT _Stride = sizeof(SpriteBatch_Vertex);


	m_pDevCon->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &_Stride, &_Offset);
	m_pDevCon->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	m_pDevCon->IASetInputLayout(m_pInputLayout);
	m_pDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pDevCon->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDevCon->PSSetShader(m_pPixelShader, nullptr, 0);

	m_pDevCon->PSSetShaderResources(0, 1, &m_pTexture);

	m_pDevCon->VSSetConstantBuffers(0, 1, &m_pMatrixConstantBuffer);


	m_pDevCon->DrawIndexed(36, 0, 0);




}