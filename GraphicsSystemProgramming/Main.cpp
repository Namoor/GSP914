#include <Windows.h>

bool isRunning;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (Message == WM_QUIT)
	{
		//Programm beenden
		isRunning = false;
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

	isRunning = true;

	//GameLoop
	while (isRunning)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			//Message bearbeiten
			TranslateMessage(&msg);
			DispatchMessage(&msg);			
		}
	}
}