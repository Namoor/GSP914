#include <Windows.h>

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>


// WM_QUIT: Application soll beendet werden
// WM_DESTROY: Window wurde destroyed

LRESULT CALLBACK WindowProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (Message == WM_DESTROY)
	{
		//Programm beenden

		PostQuitMessage(0);
	}

	return DefWindowProc(hwnd,Message,wParam,lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	//WindowClass erstellen
	WNDCLASSEX _WindowsClass;

	ZeroMemory(&_WindowsClass, sizeof(_WindowsClass));

	_WindowsClass.cbSize = sizeof(_WindowsClass);
	_WindowsClass.lpszClassName = TEXT("WindowBauplan");

	_WindowsClass.style = CS_HREDRAW | CS_VREDRAW;
	_WindowsClass.hInstance = hInstance;
	_WindowsClass.hCursor = LoadCursor(NULL,IDC_ARROW);
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
	HWND _Window = CreateWindowEx(0,
					TEXT("WindowBauplan"),
					TEXT("Neues Fenster"),
					WS_OVERLAPPEDWINDOW,
					WindowRect.left,
					WindowRect.top,
					WindowRect.right - WindowRect.left,
					WindowRect.bottom - WindowRect.top,
					NULL,
					NULL,
					hInstance,
					NULL);

	// Fenster anzeigen
	ShowWindow(_Window, SW_SHOWDEFAULT);

	MSG msg;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;

	IDXGISwapChain* m_pSwapChain;

	D3D_FEATURE_LEVEL _FeatureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;

	D3D_FEATURE_LEVEL _FeatureLevelOut;

	DXGI_SWAP_CHAIN_DESC _Desc;
	ZeroMemory(&_Desc, sizeof(_Desc));

	_Desc.BufferCount = 1;
	_Desc.BufferDesc.Format = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
	_Desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	_Desc.OutputWindow = _Window;
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

	ID3D11RenderTargetView* m_pBackBuffer;

	ID3D11Texture2D* m_pBackbufferTexture = nullptr;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&m_pBackbufferTexture);
	
	m_pDevice->CreateRenderTargetView(m_pBackbufferTexture, nullptr, &m_pBackBuffer);

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

		m_pSwapChain->Present(0, 0);

	}
}