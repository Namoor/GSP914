#include "ShadowMap.h"


ShadowMap::ShadowMap()
{
	m_pDevCon = nullptr;
	m_pDevice = nullptr;

	m_pBuffer = nullptr;
	m_pDepthStencilView = nullptr;
	m_pSRV = nullptr;
}

void ShadowMap::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, int p_MapSizeX, int p_MapSizeY)
{
	m_pDevice = p_pDevice;
	m_pDevCon = p_pDevCon;

	D3D11_TEXTURE2D_DESC _BDesc;
	ZeroMemory(&_BDesc, sizeof(_BDesc));

	_BDesc.Width = p_MapSizeX;
	_BDesc.Height = p_MapSizeY;
	_BDesc.ArraySize = 1;
	_BDesc.MipLevels = 1;

	_BDesc.MiscFlags = 0;
	_BDesc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
	_BDesc.CPUAccessFlags = 0;
	_BDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_TYPELESS;
	_BDesc.SampleDesc.Count = 4;
	_BDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;

	m_pDevice->CreateTexture2D(&_BDesc, nullptr, &m_pBuffer);


	D3D11_DEPTH_STENCIL_VIEW_DESC _DSVDesc;
	ZeroMemory(&_DSVDesc, sizeof(_DSVDesc));
	
	_DSVDesc.Format = DXGI_FORMAT::DXGI_FORMAT_D32_FLOAT;
	_DSVDesc.ViewDimension = D3D11_DSV_DIMENSION::D3D11_DSV_DIMENSION_TEXTURE2DMS;
	//_DSVDesc.Texture2D.MipSlice = 0;

	m_pDevice->CreateDepthStencilView(m_pBuffer, &_DSVDesc, &m_pDepthStencilView);

	D3D11_SHADER_RESOURCE_VIEW_DESC _SRVDesc;
	ZeroMemory(&_SRVDesc, sizeof(_SRVDesc));

	_SRVDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
	_SRVDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2DMS;
	//_SRVDesc.Texture2DMS.MipLevels = 1;
	//_SRVDesc.Texture2D.MostDetailedMip = 0;

	m_pDevice->CreateShaderResourceView(m_pBuffer, &_SRVDesc , &m_pSRV);



	m_ViewPort.Height = p_MapSizeY;
	m_ViewPort.Width = p_MapSizeX;
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.MinDepth = 0;
	m_ViewPort.MaxDepth = 1;
}


void ShadowMap::Bind()
{
	m_pDevCon->RSSetViewports(1, &m_ViewPort);

	m_pDevCon->OMSetRenderTargets(0, nullptr, m_pDepthStencilView);

	m_pDevCon->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH, 1.0f, 0);
}


ID3D11ShaderResourceView* ShadowMap::GetTexture()
{
	return m_pSRV;
}

ID3D11DepthStencilView* ShadowMap::GetDSV()
{
	return m_pDepthStencilView;
}