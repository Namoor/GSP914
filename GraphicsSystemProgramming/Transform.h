#pragma once

#include "d3dinclude.h"

class Transform
{
public:
	void SetPosition(D3DXVECTOR3 p_Position);

	D3DXMATRIX GetMatrix();

private:
	D3DXMATRIX m_WorldMatrix;
};