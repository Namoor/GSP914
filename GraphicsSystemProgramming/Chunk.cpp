#include "Chunk.h"

#include "Chunk_Structs.h"

void Chunk::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pDevice = p_pDevice;
	m_pDevCon = p_pDevCon;

	TimeSinceStart = 0;

	// World Gen
	for (int x = 0; x < ChunkSize; x++)
		for (int y = 0; y < ChunkSize; y++)
			for (int z = 0; z < ChunkSize; z++)
			{
				if (rand() % 2 > 0)
				{
					m_BlockData[x][y][z] = 1;
				}
				else
				{
					m_BlockData[x][y][z] = 0;
				}
			}





	// Faces Zählen
	FaceCount = 0;

	for (int x = 0; x < ChunkSize; x++)
		for (int y = 0; y < ChunkSize; y++)
			for (int z = 0; z < ChunkSize; z++)
			{
				// Wenn der eigene Block Solide ist, dann Faces generieren
				if (GetBlockAt(x, y, z) != 0)
				{

					// Ein Face für jeden anliegenden LuftBlock
					if (GetBlockAt(x + 1, y, z) == 0)
						FaceCount += 1;

					if (GetBlockAt(x - 1, y, z) == 0)
						FaceCount += 1;

					if (GetBlockAt(x, y + 1, z) == 0)
						FaceCount += 1;

					if (GetBlockAt(x, y - 1, z) == 0)
						FaceCount += 1;

					if (GetBlockAt(x, y, z + 1) == 0)
						FaceCount += 1;

					if (GetBlockAt(x, y, z - 1) == 0)
						FaceCount += 1;
				}
			}



	ChunkVertexStruct* _Vertices = new ChunkVertexStruct[FaceCount * 4];
	unsigned int* _Indices = new unsigned int[FaceCount * 6];

	int FaceIndex = 0;

	for (int x = 0; x < ChunkSize; x++)
		for (int y = 0; y < ChunkSize; y++)
			for (int z = 0; z < ChunkSize; z++)
			{
				// Wenn der eigene Block Solide ist, dann Faces generieren
				if (GetBlockAt(x, y, z) != 0)
				{

					if (GetBlockAt(x + 1, y, z) == 0)
					{
						// Face generieren
						CreateFace(D3DXVECTOR3(x + 0.5f, y, z), D3DXVECTOR3(0, 0, 1), D3DXVECTOR3(0, 1, 0), _Indices, _Vertices, FaceIndex);
						FaceIndex++;
					}
					if (GetBlockAt(x - 1, y, z) == 0)
					{
						// Face generieren
						CreateFace(D3DXVECTOR3(x - 0.5f, y, z), D3DXVECTOR3(0, 0, -1), D3DXVECTOR3(0, 1, 0), _Indices, _Vertices, FaceIndex);
						FaceIndex++;
					}
					if (GetBlockAt(x, y - 1, z) == 0)
					{
						// Face generieren
						CreateFace(D3DXVECTOR3(x, y - 0.5f, z), D3DXVECTOR3(-1, 0, 0), D3DXVECTOR3(0, 0, 1), _Indices, _Vertices, FaceIndex);
						FaceIndex++;
					}
					if (GetBlockAt(x, y + 1, z) == 0)
					{
						// Face generieren
						CreateFace(D3DXVECTOR3(x, y + 0.5f, z), D3DXVECTOR3(1, 0, 0), D3DXVECTOR3(0, 0, 1), _Indices, _Vertices, FaceIndex);
						FaceIndex++;
					}
					if (GetBlockAt(x, y, z + 1) == 0)
					{
						// Face generieren
						CreateFace(D3DXVECTOR3(x, y, z + 0.5f), D3DXVECTOR3(-1, 0, 0), D3DXVECTOR3(0, 1, 0), _Indices, _Vertices, FaceIndex);
						FaceIndex++;
					}
					if (GetBlockAt(x, y, z - 1) == 0)
					{
						// Face generieren
						CreateFace(D3DXVECTOR3(x, y, z - 0.5f), D3DXVECTOR3(1, 0, 0), D3DXVECTOR3(0, 1, 0), _Indices, _Vertices, FaceIndex);
						FaceIndex++;
					}
				}
			}


	// VertexBuffer erstellen

	D3D11_BUFFER_DESC _VBDesc;
	ZeroMemory(&_VBDesc, sizeof(_VBDesc));

	_VBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBDesc.ByteWidth = (FaceCount * 4) * sizeof(ChunkVertexStruct);
	_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_VBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_VBDesc, nullptr, &m_pVertexBuffer);

	D3D11_MAPPED_SUBRESOURCE _VBMSR;

	m_pDevCon->Map(m_pVertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_VBMSR);
	memcpy(_VBMSR.pData, _Vertices, (FaceCount * 4) * sizeof(ChunkVertexStruct));
	m_pDevCon->Unmap(m_pVertexBuffer, 0);

	delete[] _Vertices;







	// Index Buffer erstellen

	D3D11_BUFFER_DESC _IBDesc;
	ZeroMemory(&_IBDesc, sizeof(_IBDesc));

	_IBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IBDesc.ByteWidth = (FaceCount * 6) * sizeof(unsigned int);
	_IBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_IBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_IBDesc, nullptr, &m_pIndexBuffer);

	D3D11_MAPPED_SUBRESOURCE _IBMSR;

	m_pDevCon->Map(m_pIndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_IBMSR);
	memcpy(_IBMSR.pData, _Indices, (FaceCount * 6) * sizeof(unsigned int));
	m_pDevCon->Unmap(m_pIndexBuffer, 0);

	delete[] _Indices;



	// Shader Laden

	ID3D10Blob* _pVShaderBlob;
	ID3D10Blob* _pPShaderBlob;
	ID3D10Blob* _pErrorBlob;


	// VertexShader

	if(D3DX11CompileFromFile("VoxelShader.hlsl", nullptr, nullptr, "VShader", "vs_5_0", 0, 0, nullptr, &_pVShaderBlob, &_pErrorBlob, nullptr) != S_OK)
	{
		MessageBox(0, (char*)_pErrorBlob->GetBufferPointer(), "Fehler im Vertexshader", 0);
		PostQuitMessage(0);
		return;
	}


	m_pDevice->CreateVertexShader(_pVShaderBlob->GetBufferPointer(), _pVShaderBlob->GetBufferSize(), nullptr, &m_pVertexShader);

	// PixelShader
	if(S_OK != D3DX11CompileFromFile("VoxelShader.hlsl", nullptr, nullptr, "PShader", "ps_5_0", 0, 0, nullptr, &_pPShaderBlob, &_pErrorBlob, nullptr))
	{
		MessageBox(0, (char*)_pErrorBlob->GetBufferPointer(), "Fehler im Pixelshader", 0);
		PostQuitMessage(0);
		return;
	}
	
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

	m_pDevice->CreateBuffer(&_CBDesc, nullptr, &m_pConstantBufferMatrix);


	D3D11_BUFFER_DESC _CBDescLight;
	ZeroMemory(&_CBDescLight, sizeof(_CBDescLight));

	_CBDescLight.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	_CBDescLight.ByteWidth = sizeof(ChunkConstantBuffer);
	_CBDescLight.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_CBDescLight.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_CBDescLight, nullptr, &m_pConstantBufferLight);
}

void Chunk::Update(float p_DeltaTime)
{
	TimeSinceStart += p_DeltaTime;
}

void Chunk::Render(Camera* p_pCamera)
{
	D3DXMATRIX _Mat = p_pCamera->GetViewMatrix() * p_pCamera->GetProjMatrix();

	D3D11_MAPPED_SUBRESOURCE _CBMSR;

	m_pDevCon->Map(m_pConstantBufferMatrix, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_CBMSR);
	memcpy(_CBMSR.pData, &_Mat, sizeof(D3DXMATRIX));
	m_pDevCon->Unmap(m_pConstantBufferMatrix, 0);

	D3DXMATRIX _SunRotation;
	D3DXMatrixRotationX(&_SunRotation, TimeSinceStart);

	ChunkConstantBuffer _CCB;

	D3DXVECTOR3 _LightDir(2, -3, 1);
	D3DXVec3Normalize(&_LightDir, &_LightDir);

	_CCB.LightDir = D3DXVECTOR4(_LightDir.x, _LightDir.y, _LightDir.z, 0);

	D3DXVec4Transform(&_CCB.LightDir, &_CCB.LightDir, &_SunRotation);

	float Intensity = 1;
	_CCB.RGBLightColor_ALightIntensity = D3DXVECTOR4(1 * Intensity, 1 * Intensity, 0.7f * Intensity, 0);

	m_pDevCon->Map(m_pConstantBufferLight, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_CBMSR);
	memcpy(_CBMSR.pData, &_CCB, sizeof(ChunkConstantBuffer));
	m_pDevCon->Unmap(m_pConstantBufferLight, 0);



	UINT stride = sizeof(ChunkVertexStruct);
	UINT offset = 0;

	m_pDevCon->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	m_pDevCon->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_pDevCon->IASetInputLayout(m_pInputLayout);
	m_pDevCon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pDevCon->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDevCon->VSSetConstantBuffers(0, 1, &m_pConstantBufferMatrix);

	m_pDevCon->PSSetShader(m_pPixelShader, nullptr, 0);
	m_pDevCon->PSSetShaderResources(0, 1, &m_pDirtTexture);
	m_pDevCon->PSSetConstantBuffers(0, 1, &m_pConstantBufferLight);


	m_pDevCon->DrawIndexed(FaceCount * 2 * 3, 0, 0);

}


int Chunk::GetBlockAt(int x, int y, int z)
{
	if (x < 0 || x >= ChunkSize)
		return 0;

	if (y < 0 || y >= ChunkSize)
		return 0;

	if (z < 0 || z >= ChunkSize)
		return 0;

	return m_BlockData[x][y][z];
}

void Chunk::CreateFace(D3DXVECTOR3 p_Position, D3DXVECTOR3 Right, D3DXVECTOR3 Up, unsigned int* p_pIndices, ChunkVertexStruct* p_pVertices, int p_FaceIndex)
{
	D3DXVECTOR3 _TL = p_Position + (- Right + Up) * 0.5f;
	D3DXVECTOR3 _TR = p_Position + (+ Right + Up) * 0.5f;
	D3DXVECTOR3 _BL = p_Position + (- Right - Up) * 0.5f;
	D3DXVECTOR3 _BR = p_Position + (+ Right - Up) * 0.5f;

	D3DXVECTOR3 _Normal;
	D3DXVec3Cross(&_Normal, &Up, &Right);

	p_pVertices[p_FaceIndex * 4 + 0].Position = _BL;
	p_pVertices[p_FaceIndex * 4 + 1].Position = _BR;
	p_pVertices[p_FaceIndex * 4 + 2].Position = _TR;
	p_pVertices[p_FaceIndex * 4 + 3].Position = _TL;

	p_pVertices[p_FaceIndex * 4 + 0].Normal = _Normal;
	p_pVertices[p_FaceIndex * 4 + 1].Normal = _Normal;
	p_pVertices[p_FaceIndex * 4 + 2].Normal = _Normal;
	p_pVertices[p_FaceIndex * 4 + 3].Normal = _Normal;

	p_pVertices[p_FaceIndex * 4 + 0].UV = D3DXVECTOR2(0, 1);
	p_pVertices[p_FaceIndex * 4 + 1].UV = D3DXVECTOR2(1, 1);
	p_pVertices[p_FaceIndex * 4 + 2].UV = D3DXVECTOR2(1, 0);
	p_pVertices[p_FaceIndex * 4 + 3].UV = D3DXVECTOR2(0, 0);


	p_pIndices[p_FaceIndex * 6 + 0] = p_FaceIndex * 4 + 0;
	p_pIndices[p_FaceIndex * 6 + 1] = p_FaceIndex * 4 + 3;
	p_pIndices[p_FaceIndex * 6 + 2] = p_FaceIndex * 4 + 2;
	p_pIndices[p_FaceIndex * 6 + 3] = p_FaceIndex * 4 + 0;
	p_pIndices[p_FaceIndex * 6 + 4] = p_FaceIndex * 4 + 2;
	p_pIndices[p_FaceIndex * 6 + 5] = p_FaceIndex * 4 + 1;
}