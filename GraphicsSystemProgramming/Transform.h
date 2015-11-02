#pragma once

#include "d3dinclude.h"

class Transform
{
public:
	Transform();

	void SetPosition(D3DXVECTOR3 p_Position);
	void SetScale(D3DXVECTOR3 p_Position);

	D3DXMATRIX GetMatrix();

private:
	D3DXMATRIX m_WorldMatrix;

	D3DXVECTOR3 Position;
	D3DXVECTOR3 Scale;
};