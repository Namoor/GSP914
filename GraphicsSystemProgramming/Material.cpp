#include "Material.h"



Material::Material()
{
	m_pInputLayout = nullptr;

	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
}



void Material::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pDevCon = p_pDevCon;
	m_pDevice = p_pDevice;


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


	D3DX11CreateShaderResourceViewFromFile(p_pDevice, "Dirt.jpg", nullptr, nullptr, &m_pTexture, nullptr);


	// ConstantBuffer
	D3D11_BUFFER_DESC _CBD;
	ZeroMemory(&_CBD, sizeof(_CBD));


	_CBD.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	_CBD.ByteWidth = 64;
	_CBD.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_CBD.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_CBD, nullptr, &m_pMatrixConstantBuffer);

}


void Material::Update(float p_DeltaTime, Camera* p_pCamera)
{
	static float x = 0;
	x += p_DeltaTime;

	// WorldMatrix
	D3DXMatrixTranslation(&WorldMatrix, 0, 0.0f, 0);
	D3DXMATRIX _Rotation;
	D3DXMatrixRotationY(&_Rotation, x);


	D3DXMATRIX _WVPMatrix = WorldMatrix * p_pCamera->GetViewMatrix() * p_pCamera->GetProjMatrix();

	D3D11_MAPPED_SUBRESOURCE _MM;
	m_pDevCon->Map(m_pMatrixConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MM);
	memcpy(_MM.pData, &_WVPMatrix, 64);
	m_pDevCon->Unmap(m_pMatrixConstantBuffer, 0);

}


void Material::Bind()
{
	m_pDevCon->IASetInputLayout(m_pInputLayout);
	m_pDevCon->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDevCon->PSSetShader(m_pPixelShader, nullptr, 0);



	m_pDevCon->PSSetShaderResources(0, 1, &m_pTexture);
	m_pDevCon->VSSetConstantBuffers(0, 1, &m_pMatrixConstantBuffer);
}
