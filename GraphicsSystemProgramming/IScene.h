#pragma once

#include "d3dinclude.h"


class IScene
{
public:
	virtual void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDeviceContext, ID3D11RenderTargetView* p_pBackBuffer, ID3D11DepthStencilView* p_pDSV) = 0;

	virtual void Update(float p_DeltaTime) = 0;
	virtual void Render() = 0;

	virtual void Shutdown() = 0;

};