#include "Material.h"



void Material::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pDevCon = p_pDevCon;
	m_pDevice = p_pDevice;




	// ConstantBuffer
	D3D11_BUFFER_DESC _CBD;
	ZeroMemory(&_CBD, sizeof(_CBD));


	_CBD.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
	_CBD.ByteWidth = 64;
	_CBD.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
	_CBD.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;

	m_pDevice->CreateBuffer(&_CBD, nullptr, &m_pMatrixConstantBuffer);

}


ID3D10Blob* Material::LoadShaders(char* VertexShaderSourceFile, char* VertexShaderFunctionName, char* PixelShaderSourceFile, char* PixelShaderFunctionName)
{
	// VertexShader
	ID3D10Blob* _pVertexShader;
	ID3D10Blob* _pPixelShader;
	ID3D10Blob* _pError;

	if (D3DX11CompileFromFile(VertexShaderSourceFile, nullptr, nullptr, VertexShaderFunctionName, "vs_5_0", 0, 0, nullptr,
		&_pVertexShader, &_pError, nullptr) != S_OK)
	{
		MessageBox(0, (char*)_pError->GetBufferPointer(), "Fehler im Vertexshader", 0);
		PostQuitMessage(0);
		return nullptr;
	}

	m_pDevice->CreateVertexShader(_pVertexShader->GetBufferPointer(), _pVertexShader->GetBufferSize(), nullptr, &m_pVertexShader);


	// PixelShader
	if (D3DX11CompileFromFile(PixelShaderSourceFile, nullptr, nullptr, PixelShaderFunctionName, "ps_5_0", 0, 0, nullptr,
		&_pPixelShader, &_pError, nullptr) != S_OK)
	{
		MessageBox(0, (char*)_pError->GetBufferPointer(), "Fehler im Pixelshader", 0);
		PostQuitMessage(0);
		return nullptr;
	}

	m_pDevice->CreatePixelShader(_pPixelShader->GetBufferPointer(), _pPixelShader->GetBufferSize(), nullptr, &m_pPixelShader);

	return _pVertexShader;
}


void Material::SetMatrixData(Transform* p_pTransform, Camera* p_pCamera)
{
	D3DXMATRIX _WVPMatrix = p_pTransform->GetMatrix() * p_pCamera->GetViewMatrix() * p_pCamera->GetProjMatrix();

	D3D11_MAPPED_SUBRESOURCE _MM;
	m_pDevCon->Map(m_pMatrixConstantBuffer, 0, D3D11_MAP::D3D11_MAP_WRITE_DISCARD, 0, &_MM);
	memcpy(_MM.pData, &_WVPMatrix, 64);
	m_pDevCon->Unmap(m_pMatrixConstantBuffer, 0);
}