#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{

}

void DirectionalLight::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pShadowMap = new ShadowMap();
	m_pShadowMap->Init(p_pDevice, p_pDevCon, 800, 600);

	m_Direction = D3DXVECTOR3(0, -1, 0);
}

void DirectionalLight::SetDirection(D3DXVECTOR3 p_Direction)
{
	D3DXVec3Normalize(&m_Direction, &p_Direction);
}


D3DXVECTOR3 DirectionalLight::GetDirection()
{
	return m_Direction;
}

D3DXVECTOR4 DirectionalLight::GetDirection4()
{
	D3DXVECTOR4 _Dir;

	_Dir.x = m_Direction.x;
	_Dir.y = m_Direction.y;
	_Dir.z = m_Direction.z;
	_Dir.w = 0;

	return _Dir;
}

D3DXMATRIX DirectionalLight::GetViewProjectionMatrix()
{
	D3DXMATRIX _LightView;
	D3DXMATRIX _LightProj;

	D3DXVECTOR3 LightTarget(0, 0, 0);
	D3DXVECTOR3 LightUp(0, 1, 0);
	D3DXVECTOR3 LightPosition = LightTarget - m_Direction * 16;

	D3DXMatrixPerspectiveFovLH(&_LightProj, D3DXToRadian(60), 800.0f / 600.0f, 0.1f, 1000.0f);
	D3DXMatrixLookAtLH(&_LightView, &LightPosition, &LightTarget, &LightUp);

	return _LightView * _LightProj;
}

ShadowMap* DirectionalLight::GetShadowMap()
{
	return m_pShadowMap;
}
