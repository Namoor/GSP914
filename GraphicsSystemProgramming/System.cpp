#include "System.h"


// WM_QUIT: Application soll beendet werden
// WM_DESTROY: Window wurde destroyed

LRESULT CALLBACK WindowProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (Message == WM_DESTROY)
	{
		//Programm beenden

		PostQuitMessage(0);
	}

	return DefWindowProc(hwnd, Message, wParam, lParam);
}


void System::Run(IScene* p_pScene)
{
	m_pScene = p_pScene;

	InitWindow();
	InitGraphics();


	m_pScene->Init(m_pDevice, m_pDeviceContext);



	MSG msg;

	//GameLoop
	while (true)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//Message bearbeiten
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}



		float _Color[4];
		_Color[0] = (rand() % 256) / 255.0f;
		_Color[1] = (rand() % 256) / 255.0f;
		_Color[2] = (rand() % 256) / 255.0f;
		_Color[3] = 1;

		m_pDeviceContext->ClearRenderTargetView(m_pBackBuffer, _Color);

		m_pScene->Update();

		m_pScene->Render();

		m_pSwapChain->Present(0, 0);

	}

	m_pScene->Shutdown();
}


void System::InitWindow()
{
	//WindowClass erstellen
	WNDCLASSEX _WindowsClass;

	ZeroMemory(&_WindowsClass, sizeof(_WindowsClass));

	_WindowsClass.cbSize = sizeof(_WindowsClass);
	_WindowsClass.lpszClassName = TEXT("WindowBauplan");

	_WindowsClass.style = CS_HREDRAW | CS_VREDRAW;
	_WindowsClass.hInstance = GetModuleHandle(0);
	_WindowsClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	_WindowsClass.hbrBackground = (HBRUSH)COLOR_GRAYTEXT;
	_WindowsClass.lpfnWndProc = WindowProc;

	//WindowClass Registieren
	RegisterClassEx(&_WindowsClass);

	//WindowsRect anlegen und anpassen
	RECT WindowRect;
	WindowRect.top = 100;
	WindowRect.left = 100;
	WindowRect.bottom = 200;
	WindowRect.right = 200;

	//Fenster erzeugen
	m_Window = CreateWindowEx(0,
		TEXT("WindowBauplan"),
		TEXT("Neues Fenster"),
		WS_OVERLAPPEDWINDOW,
		WindowRect.left,
		WindowRect.top,
		WindowRect.right - WindowRect.left,
		WindowRect.bottom - WindowRect.top,
		NULL,
		NULL,
		GetModuleHandle(0),
		NULL);

	// Fenster anzeigen
	ShowWindow(m_Window, SW_SHOWDEFAULT);
}

void System::InitGraphics()
{

	D3D_FEATURE_LEVEL _FeatureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	D3D_FEATURE_LEVEL _FeatureLevelOut;

	DXGI_SWAP_CHAIN_DESC _Desc;
	ZeroMemory(&_Desc, sizeof(_Desc));

	_Desc.BufferCount = 1;
	_Desc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	_Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	_Desc.OutputWindow = m_Window;
	_Desc.Windowed = true;
	_Desc.SampleDesc.Count = 4;


	HRESULT _HR = D3D11CreateDeviceAndSwapChain(nullptr,
		D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		0, // D3D11_CREATE_DEVICE_FLAG::D3D11_CREATE_DEVICE_DEBUG,
		&_FeatureLevel,
		1,
		D3D11_SDK_VERSION,
		&_Desc,
		&m_pSwapChain,
		&m_pDevice,
		&_FeatureLevelOut,
		&m_pDeviceContext);


	// ID3D11Texture2D : Textur

	// ID3D11ShaderResourceView : Objekt um aus der Textur zu lesen
	// ID3D11RenderTargetView : Objekt um in die Textur zu schreiben

	ID3D11Texture2D* m_pBackbufferTexture = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pBackbufferTexture);

	m_pDevice->CreateRenderTargetView(m_pBackbufferTexture, nullptr, &m_pBackBuffer);

}