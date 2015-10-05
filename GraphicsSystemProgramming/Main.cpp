#include <Windows.h>

#include "System.h"

#include "2DScene.h"

#include "3DScene.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	System* _pSystem = new System();

	//_pSystem->Run(new Scene2D());
	_pSystem->Run(new Scene3D());
}