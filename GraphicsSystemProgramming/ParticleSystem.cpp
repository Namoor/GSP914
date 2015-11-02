#include "ParticleSystem.h"



ParticleSystem::ParticleSystem()
{
	m_ParticleCount = 0;


	m_pDevice = nullptr;
	m_pDevCon = nullptr;

	m_pVertexBuffer = nullptr;
	m_pIndexBuffer = nullptr;

	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;

	m_pInputLayout = nullptr;
}

void ParticleSystem::SpawnParticle(D3DXVECTOR3 p_Position, D3DXVECTOR3 p_Velocity, D3DXVECTOR4 p_Color)
{
	m_ParticleCount ++;
	
	m_Particles[m_ParticleCount - 1].Color = p_Color;
	m_Particles[m_ParticleCount - 1].Position = p_Position;
	m_Particles[m_ParticleCount - 1].Velocity = p_Velocity;
}

void ParticleSystem::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pDevice = p_pDevice;
	m_pDevCon = p_pDevCon;

	D3DX11CreateShaderResourceViewFromFile(m_pDevice, "Dirt.jpg", nullptr, nullptr, &m_pTexture, nullptr);

	// Hack: Am Anfang ein Particle Spawnen
	m_ParticleCount = 0;
	//
	//
	//


	// ConstantBuffer
	D3D11_BUFFER_DESC _CBDesc;
	ZeroMemory(&_CBDesc, sizeof(_CBDesc));

	_CBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	_CBDesc.ByteWidth = 128;
	_CBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	_CBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

	m_pDevice->CreateBuffer(&_CBDesc, nullptr, &m_pConstantBuffer);

	
	// VertexBuffer
	D3D11_BUFFER_DESC _VBDesc;
	ZeroMemory(&_VBDesc, sizeof(_VBDesc));

	_VBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_VERTEX_BUFFER;
	_VBDesc.ByteWidth = sizeof(Particle_Vertex) * 4 * MAXPARTICLES;
	_VBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	_VBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

	m_pDevice->CreateBuffer(&_VBDesc, nullptr, &m_pVertexBuffer);

	// IndexBuffer
	D3D11_BUFFER_DESC _IBDesc;
	ZeroMemory(&_IBDesc, sizeof(_IBDesc));

	_IBDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_INDEX_BUFFER;
	_IBDesc.ByteWidth = sizeof(unsigned int) * 6 * MAXPARTICLES;
	_IBDesc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
	_IBDesc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;

	m_pDevice->CreateBuffer(&_IBDesc, nullptr, &m_pIndexBuffer);

	unsigned int* _Indices = new unsigned int[6 * MAXPARTICLES];

	for (int x = 0; x < MAXPARTICLES; x++)
	{
		_Indices[x * 6 + 0] = x * 4 + 0;
		_Indices[x * 6 + 1] = x * 4 + 2;
		_Indices[x * 6 + 2] = x * 4 + 3;
		_Indices[x * 6 + 3] = x * 4 + 0;
		_Indices[x * 6 + 4] = x * 4 + 3;
		_Indices[x * 6 + 5] = x * 4 + 1;
	}

	D3D11_MAPPED_SUBRESOURCE _MSR;

	m_pDevCon->Map(m_pIndexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MSR);
	memcpy(_MSR.pData, _Indices, sizeof(unsigned int) * 6 * MAXPARTICLES);
	m_pDevCon->Unmap(m_pIndexBuffer, 0);


	// VertexShader
	ID3D10Blob* _VBlob;
	ID3D10Blob* _PBlob;
	ID3D10Blob* _pError;

	if (D3DX11CompileFromFile("ParticleShader.hlsl", nullptr, nullptr, "VShader", "vs_5_0", 0, 0,
		nullptr, &_VBlob, &_pError, nullptr) != S_OK)
	{
		MessageBox(0, (char*)_pError->GetBufferPointer(), "Fehler im VertexShader", 0);
		PostQuitMessage(0);
		return;
	}

	m_pDevice->CreateVertexShader(_VBlob->GetBufferPointer(), _VBlob->GetBufferSize(), nullptr, &m_pVertexShader);



	// PixelShader
	if (D3DX11CompileFromFile("ParticleShader.hlsl", nullptr, nullptr, "PShader", "ps_5_0", 0, 0,
		nullptr, &_PBlob, &_pError, nullptr) != S_OK)
	{
		MessageBox(0, (char*)_pError->GetBufferPointer(), "Fehler im PixelShader", 0);
		PostQuitMessage(0);
		return;
	}

	m_pDevice->CreatePixelShader(_PBlob->GetBufferPointer(), _PBlob->GetBufferSize(), nullptr, &m_pPixelShader);

	// InputLayout
	D3D11_INPUT_ELEMENT_DESC _IED[4];
	//Position
	_IED[0].SemanticName = "POSITION";
	_IED[0].SemanticIndex = 0;
	_IED[0].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
	_IED[0].AlignedByteOffset = 0;
	_IED[0].InstanceDataStepRate = 0;
	_IED[0].InputSlot = 0;
	_IED[0].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;

	// Position in Quad
	_IED[1].SemanticName = "POSITION";
	_IED[1].SemanticIndex = 1;
	_IED[1].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
	_IED[1].AlignedByteOffset = 12;
	_IED[1].InstanceDataStepRate = 0;
	_IED[1].InputSlot = 0;
	_IED[1].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;

	// Color
	_IED[2].SemanticName = "COLOR";
	_IED[2].SemanticIndex = 0;
	_IED[2].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
	_IED[2].AlignedByteOffset = 20;
	_IED[2].InstanceDataStepRate = 0;
	_IED[2].InputSlot = 0;
	_IED[2].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
	
	// UV
	_IED[3].SemanticName = "TEXCOORD";
	_IED[3].SemanticIndex = 0;
	_IED[3].Format = DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
	_IED[3].AlignedByteOffset = 36;
	_IED[3].InstanceDataStepRate = 0;
	_IED[3].InputSlot = 0;
	_IED[3].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;


	m_pDevice->CreateInputLayout(_IED, 4, _VBlob->GetBufferPointer(), _VBlob->GetBufferSize(), &m_pInputLayout);
}

void ParticleSystem::Update(float DeltaTime)
{
	for (int x = 0; x < m_ParticleCount; x++)
	{
		m_Particles[x].Velocity += D3DXVECTOR3(0.5f, 0.5f, 0.5f) * DeltaTime;

		m_Particles[x].Position += m_Particles[x].Velocity * DeltaTime;
	}
}

void ParticleSystem::Render(Camera* p_pCamera)
{
	Particle_CBuffer _CBuffer;
	_CBuffer.m_WVP = p_pCamera->GetViewMatrix() * p_pCamera->GetProjMatrix();
	_CBuffer.m_Right = p_pCamera->GetRight4();
	_CBuffer.m_Up = p_pCamera->GetUp4();

	D3D11_MAPPED_SUBRESOURCE _MSRC;

	m_pDevCon->Map(m_pConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MSRC);
	memcpy(_MSRC.pData, &_CBuffer, sizeof(Particle_CBuffer));
	m_pDevCon->Unmap(m_pConstantBuffer, 0);



	for (int x = 0; x < m_ParticleCount; x++)
	{
		m_Vertices[x * 4 + 0].Position = m_Particles[x].Position;
		m_Vertices[x * 4 + 1].Position = m_Particles[x].Position;
		m_Vertices[x * 4 + 2].Position = m_Particles[x].Position;
		m_Vertices[x * 4 + 3].Position = m_Particles[x].Position;

		m_Vertices[x * 4 + 0].Color = m_Particles[x].Color;
		m_Vertices[x * 4 + 1].Color = m_Particles[x].Color;
		m_Vertices[x * 4 + 2].Color = m_Particles[x].Color;
		m_Vertices[x * 4 + 3].Color = m_Particles[x].Color;

		m_Vertices[x * 4 + 0].UV = D3DXVECTOR2(0, 0);
		m_Vertices[x * 4 + 1].UV = D3DXVECTOR2(1, 0);
		m_Vertices[x * 4 + 2].UV = D3DXVECTOR2(0, 1);
		m_Vertices[x * 4 + 3].UV = D3DXVECTOR2(1, 1);

		m_Vertices[x * 4 + 0].PositionInQuad = D3DXVECTOR2(-1, -1);
		m_Vertices[x * 4 + 1].PositionInQuad = D3DXVECTOR2(1, -1);
		m_Vertices[x * 4 + 2].PositionInQuad = D3DXVECTOR2(-1, 1);
		m_Vertices[x * 4 + 3].PositionInQuad = D3DXVECTOR2(1, 1);
	}

	D3D11_MAPPED_SUBRESOURCE _MSR;

	m_pDevCon->Map(m_pVertexBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MSR);
	memcpy(_MSR.pData, m_Vertices, 4 * sizeof(Particle_Vertex) * m_ParticleCount);
	m_pDevCon->Unmap(m_pVertexBuffer, 0);

	UINT Stride = sizeof(Particle_Vertex);
	UINT Offset = 0;

	m_pDevCon->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &Stride, &Offset);
	m_pDevCon->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);
	m_pDevCon->IASetInputLayout(m_pInputLayout);
	m_pDevCon->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_pDevCon->PSSetShader(m_pPixelShader, nullptr, 0);
	m_pDevCon->PSSetShaderResources(0, 1, &m_pTexture);

	m_pDevCon->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDevCon->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	m_pDevCon->DrawIndexed(m_ParticleCount * 6, 0, 0);
}


