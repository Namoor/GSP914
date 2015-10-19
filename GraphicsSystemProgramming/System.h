#pragma once
#include <Windows.h>
#include "d3dinclude.h"

#include "IScene.h"

#include "Timer.h"

class System
{
public:
	void Run(IScene* p_pScene);

private:
	void InitWindow();
	void InitGraphics();


private:
	IScene* m_pScene;

	HWND m_Window;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;
	IDXGISwapChain* m_pSwapChain;

	ID3D11RenderTargetView* m_pBackBuffer;
	ID3D11DepthStencilView* m_pDepthStencilView;

	Timer* m_pTimer;
};