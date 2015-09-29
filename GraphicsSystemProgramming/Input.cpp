#include "Input.h"

#include <Windows.h>

Input::Input()
{
	for (int x = 0; x < 256; x++)
	{
		m_KeyWasDown[x] = false;
		m_KeyIsDown[x] = false;
	}
}

void Input::Update()
{
	for (int x = 0; x < 256; x++)
	{
		m_KeyWasDown[x] = m_KeyIsDown[x];
		m_KeyIsDown[x] = GetAsyncKeyState(x);
	}

	for (int x = 0; x < 4; x++)
	{
		m_LastControllerState[x] = m_ControllerState[x];
		m_ControllerConnected[x] = XInputGetState(x, &(m_ControllerState[x])) == ERROR_SUCCESS;
	}
}


bool Input::GetKey(KeyCode::Key p_Key)
{
	return m_KeyIsDown[p_Key];
}

bool Input::GetKeyLastFrame(KeyCode::Key p_Key)
{
	return m_KeyWasDown[p_Key];
}

bool Input::GetKeyDown(KeyCode::Key p_Key)
{
	return m_KeyIsDown[p_Key] && !m_KeyWasDown[p_Key];
}

bool Input::GetKeyUp(KeyCode::Key p_Key)
{
	return !m_KeyIsDown[p_Key] && m_KeyWasDown[p_Key];
}


D3DXVECTOR2 Input::GetMousePosition()
{
	POINT _CursorPos;

	GetCursorPos(&_CursorPos);

	return D3DXVECTOR2(_CursorPos.x, _CursorPos.y);
}

void Input::SetMousePosition(D3DXVECTOR2 p_Position)
{
	SetCursorPos(p_Position.x, p_Position.y);
}

// Controller


float Input::GetAxis(int ControllerID, GamePadAxis::Axis p_Axis)
{
	if (!m_ControllerConnected[ControllerID])
		return 0.0f;

	switch (p_Axis)
	{
	case GamePadAxis::LStickX:
		return m_ControllerState[ControllerID].Gamepad.sThumbLX / 32767.0f;
	case GamePadAxis::LStickY:
		return m_ControllerState[ControllerID].Gamepad.sThumbLY / 32767.0f;
	case GamePadAxis::RStickX:
		return m_ControllerState[ControllerID].Gamepad.sThumbRX / 32767.0f;
	case GamePadAxis::RStickY:
		return m_ControllerState[ControllerID].Gamepad.sThumbRY / 32767.0f;

	case GamePadAxis::LTrigger:
		return m_ControllerState[ControllerID].Gamepad.bLeftTrigger / 256.0f;
	case GamePadAxis::RTrigger:
		return m_ControllerState[ControllerID].Gamepad.bRightTrigger / 256.0f;
	}

	return 0;
}


bool Input::GetButton(int p_ControllerID, GamePadButton::Button p_Button)
{
	if (!m_ControllerConnected[p_ControllerID])
		return false;

	return (m_ControllerState[p_ControllerID].Gamepad.wButtons & p_Button);
}

bool Input::GetButtonLastFrame(int p_ControllerID, GamePadButton::Button p_Button)
{
	if (!m_ControllerConnected[p_ControllerID])
		return false;

	return (m_LastControllerState[p_ControllerID].Gamepad.wButtons & p_Button);
}

bool Input::GetButtonDown(int p_ControllerID, GamePadButton::Button p_Button)
{
	if (!m_ControllerConnected[p_ControllerID])
		return false;

	return (m_ControllerState[p_ControllerID].Gamepad.wButtons & p_Button) && !(m_LastControllerState[p_ControllerID].Gamepad.wButtons & p_Button);
}

void Input::SetVibrationLevel(int p_ControllerID, float LRotationAmmount, float RRotationAmmount)
{
	XINPUT_VIBRATION _Data;
	_Data.wLeftMotorSpeed = LRotationAmmount * 65535;
	_Data.wRightMotorSpeed = RRotationAmmount * 65535;

	XInputSetState(p_ControllerID, &_Data);
}