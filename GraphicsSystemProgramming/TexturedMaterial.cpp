#include "TexturedMaterial.h"



TexturedMaterial::TexturedMaterial()
{
	m_pInputLayout = nullptr;

	m_pVertexShader = nullptr;
	m_pPixelShader = nullptr;
}



void TexturedMaterial::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	Material::Init(p_pDevice, p_pDevCon);

	ID3D10Blob* _pVertexShader = LoadShaders("CubeShader.hlsl", "VShader", "CubeShader.hlsl", "PShader");

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


	
}





void TexturedMaterial::Bind()
{
	m_pDevCon->IASetInputLayout(m_pInputLayout);
	m_pDevCon->VSSetShader(m_pVertexShader, nullptr, 0);
	m_pDevCon->PSSetShader(m_pPixelShader, nullptr, 0);



	m_pDevCon->PSSetShaderResources(0, 1, &m_pTexture);
	m_pDevCon->VSSetConstantBuffers(0, 1, &m_pMatrixConstantBuffer);
}
