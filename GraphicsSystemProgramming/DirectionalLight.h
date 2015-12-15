#pragma once

#include "d3dinclude.h"
#include "ShadowMap.h"

class DirectionalLight
{
public:
	DirectionalLight();
	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon);

	void SetDirection(D3DXVECTOR3 p_Direction);


	D3DXVECTOR3 GetDirection();
	D3DXVECTOR4 GetDirection4();
	
	D3DXMATRIX GetViewProjectionMatrix();
	
	ShadowMap* GetShadowMap();

private:
	ShadowMap* m_pShadowMap;
	D3DXVECTOR3 m_Direction;
};