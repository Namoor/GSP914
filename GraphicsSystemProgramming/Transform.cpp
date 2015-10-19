#include "Transform.h"



void Transform::SetPosition(D3DXVECTOR3 p_Position)
{
	D3DXMatrixTranslation(&m_WorldMatrix, p_Position.x, p_Position.y, p_Position.z);
}

D3DXMATRIX Transform::GetMatrix()
{
	return m_WorldMatrix;
}