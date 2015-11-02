#include "Transform.h"


Transform::Transform()
{
	Position = D3DXVECTOR3(0, 0, 0);
	Scale = D3DXVECTOR3(1, 1, 1);
}

void Transform::SetPosition(D3DXVECTOR3 p_Position)
{
	Position = p_Position;
}

void Transform::SetScale(D3DXVECTOR3 p_Scale)
{
	Scale = p_Scale;
	//D3DXMatrixTranslation(&m_WorldMatrix, p_Position.x, p_Position.y, p_Position.z);
}

D3DXMATRIX Transform::GetMatrix()
{
	D3DXMATRIX _Pos;
	D3DXMATRIX _Scale;

	D3DXMatrixTranslation(&_Pos, Position.x, Position.y, Position.z);
	D3DXMatrixScaling(&_Scale, Scale.x, Scale.y, Scale.z);
	
	m_WorldMatrix = _Scale * _Pos;

	return m_WorldMatrix;
}