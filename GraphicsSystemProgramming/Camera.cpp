#pragma once

#include "Camera.h"

Camera::Camera()
{
	m_Target = D3DXVECTOR3(0, 0, 0);
	m_Up = D3DXVECTOR3(0, 1, 0);
	m_Direction = D3DXVECTOR3(1, -1, 1);

	m_Distance = 4;

	D3DXVec3Cross(&m_Right, &m_Up, &m_Direction);
	D3DXVec3Cross(&m_Up, &m_Direction, &m_Right);

	D3DXVec3Normalize(&m_Direction, &m_Direction);
	D3DXVec3Normalize(&m_Up, &m_Up);
	D3DXVec3Normalize(&m_Right, &m_Right);

	UpdateViewMatrix();

	D3DXMatrixPerspectiveFovLH(&m_Projection, D3DXToRadian(50), 800.0f / 600.0f, 0.2f, 1000.0f);
}

Camera::~Camera()
{

}

D3DXMATRIX Camera::GetViewMatrix()
{
	return m_View;
}

D3DXMATRIX Camera::GetProjMatrix()
{
	return m_Projection;
}

void Camera::Update(float Delta, Input* p_pInput)
{
	float xMovement = 0;
	float yMovement = 0;
	float zMovement = 0;

	if (p_pInput->GetKey(KeyCode::A))
		xMovement -= 1;
	if (p_pInput->GetKey(KeyCode::D))
		xMovement += 1;

	if (p_pInput->GetKey(KeyCode::W))
		zMovement += 1;
	if (p_pInput->GetKey(KeyCode::S))
		zMovement -= 1;
	
	if (p_pInput->GetKey(KeyCode::Space))
		yMovement += 1;
	if (p_pInput->GetKey(KeyCode::LeftShift))
		yMovement -= 1;

	if (p_pInput->GetKeyDown(KeyCode::RightMouse))
	{
		SavedMousePos = p_pInput->GetMousePosition();
		p_pInput->SetMousePosition(D3DXVECTOR2(400, 400));
		ShowCursor(false);
	}


	if (p_pInput->GetKey(KeyCode::RightMouse))
	{
		D3DXVECTOR2 _Diff = p_pInput->GetMousePosition() - D3DXVECTOR2(400, 400);
		RotateX(_Diff.y * 0.2f, false);
		RotateY(_Diff.x * 0.2f, true);
		p_pInput->SetMousePosition(D3DXVECTOR2(400, 400));


	}

	if (p_pInput->GetKeyUp(KeyCode::RightMouse))
	{
		p_pInput->SetMousePosition(SavedMousePos);
		ShowCursor(true);
	}

#pragma region controller
	if (abs(p_pInput->GetAxis(0, GamePadAxis::LStickX)) > 0.2f)
	{
		xMovement += p_pInput->GetAxis(0, GamePadAxis::LStickX);
	}

	if (abs(p_pInput->GetAxis(0, GamePadAxis::LStickY)) > 0.2f)
	{
		zMovement += p_pInput->GetAxis(0, GamePadAxis::LStickY);
	}

	if (p_pInput->GetButton(0, GamePadButton::LShoulder))
		yMovement += 1;

	if (p_pInput->GetButton(0, GamePadButton::RShoulder))
		yMovement -= 1;


	if (abs(p_pInput->GetAxis(0, GamePadAxis::RStickX)) > 0.2f)
	{
		RotateY(p_pInput->GetAxis(0, GamePadAxis::RStickX) * Delta * 180, true);
	}

	if (abs(p_pInput->GetAxis(0, GamePadAxis::RStickY)) > 0.2f)
	{
		RotateX(p_pInput->GetAxis(0, GamePadAxis::RStickY) * Delta * -180, false);
	}

#pragma endregion

	D3DXVECTOR3 _Movement(xMovement, yMovement, zMovement);

	Move(_Movement * Delta, false);
}

void Camera::Move(D3DXVECTOR3 p_Movement, bool IsInWorldSpace)
{
	if (IsInWorldSpace)
	{
		m_Target += p_Movement;
	}
	else
	{
		m_Target += p_Movement.x * m_Right + p_Movement.y * m_Up + p_Movement.z * m_Direction;
	}

	UpdateViewMatrix();
}

void Camera::RotateX(float p_Degree, bool IsInWorldSpace)
{

	D3DXMATRIX _Rotation;

	if (IsInWorldSpace)
	{
		D3DXMatrixRotationX(&_Rotation, D3DXToRadian(p_Degree));
	}
	else
	{
		D3DXMatrixRotationAxis(&_Rotation, &m_Right, D3DXToRadian(p_Degree));
	}

	D3DXVECTOR4 _Output;

	D3DXVec3Transform(&_Output, &m_Direction, &_Rotation);
	m_Direction = D3DXVECTOR3(_Output.x, _Output.y, _Output.z);

	D3DXVec3Transform(&_Output, &m_Up, &_Rotation);
	m_Up = D3DXVECTOR3(_Output.x, _Output.y, _Output.z);

	D3DXVec3Transform(&_Output, &m_Right, &_Rotation);
	m_Right = D3DXVECTOR3(_Output.x, _Output.y, _Output.z);


	UpdateViewMatrix();

}

void Camera::RotateY(float p_Degree, bool IsInWorldSpace)
{
	D3DXMATRIX _Rotation;

	if (IsInWorldSpace)
	{
		D3DXMatrixRotationY(&_Rotation, D3DXToRadian( p_Degree));
	}
	else
	{
		D3DXMatrixRotationAxis(&_Rotation, &m_Up, D3DXToRadian(p_Degree));
	}

	D3DXVECTOR4 _Output;

	D3DXVec3Transform(&_Output, &m_Direction, &_Rotation);
	m_Direction = D3DXVECTOR3(_Output.x, _Output.y, _Output.z);

	D3DXVec3Transform(&_Output, &m_Up, &_Rotation);
	m_Up = D3DXVECTOR3(_Output.x, _Output.y, _Output.z);

	D3DXVec3Transform(&_Output, &m_Right, &_Rotation);
	m_Right= D3DXVECTOR3(_Output.x, _Output.y, _Output.z);


	UpdateViewMatrix();

}

void Camera::RotateZ(float p_Degree, bool IsInWorldSpace)
{

	D3DXMATRIX _Rotation;

	if (IsInWorldSpace)
	{
		D3DXMatrixRotationZ(&_Rotation, D3DXToRadian(p_Degree));
	}
	else
	{
		D3DXMatrixRotationAxis(&_Rotation, &m_Direction, D3DXToRadian(p_Degree));
	}

	D3DXVECTOR4 _Output;

	D3DXVec3Transform(&_Output, &m_Direction, &_Rotation);
	m_Direction = D3DXVECTOR3(_Output.x, _Output.y, _Output.z);

	D3DXVec3Transform(&_Output, &m_Up, &_Rotation);
	m_Up = D3DXVECTOR3(_Output.x, _Output.y, _Output.z);

	D3DXVec3Transform(&_Output, &m_Right, &_Rotation);
	m_Right = D3DXVECTOR3(_Output.x, _Output.y, _Output.z);


	UpdateViewMatrix();

}

void Camera::UpdateViewMatrix()
{
	D3DXVECTOR3 _CameraPosition = m_Target - m_Direction * m_Distance;

	D3DXMatrixLookAtLH(&m_View, &_CameraPosition, &m_Target, &m_Up);
}

