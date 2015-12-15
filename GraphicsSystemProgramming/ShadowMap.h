#pragma once

#include "d3dinclude.h"

// Shadowmap speichert für jeden beliebigen Pixel die entfernung vom Licht,
// um nachher zu bestimmen welche Punkte vom Licht beleuchtet werden und welche nicht
class ShadowMap
{
public:
	ShadowMap();
	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, int p_MapSizeX, int p_MapSizeY);


	void Bind();

	ID3D11ShaderResourceView* GetTexture();
	ID3D11DepthStencilView* GetDSV();

private:
	ID3D11DepthStencilView* m_pDepthStencilView;
	ID3D11ShaderResourceView* m_pSRV;

	D3D11_VIEWPORT m_ViewPort;

	ID3D11Texture2D* m_pBuffer;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;
};