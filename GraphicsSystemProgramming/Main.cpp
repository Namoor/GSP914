#include <Windows.h>

#include "System.h"

#include "2DScene.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	System* _pSystem = new System();

	_pSystem->Run(new Scene2D());
}