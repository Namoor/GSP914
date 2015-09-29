#pragma once

#include "d3dinclude.h"


class IScene
{
public:
	virtual void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext) = 0;

	virtual void Update(float p_DeltaTime) = 0;
	virtual void Render() = 0;

	virtual void Shutdown() = 0;

};