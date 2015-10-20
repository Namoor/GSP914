#pragma once

#include "d3dinclude.h"

#include "Camera.h"


#include "Mesh.h"

class Cube : public Mesh
{
public:
	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon);
};