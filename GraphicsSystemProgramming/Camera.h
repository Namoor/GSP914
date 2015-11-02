#pragma once


#include "d3dinclude.h"

#include "Input.h"


class Camera
{
public:
	Camera();
	~Camera();

public:
	D3DXVECTOR4 GetPosition4();
	D3DXMATRIX GetViewMatrix();
	D3DXMATRIX GetProjMatrix();

	void Update(float Delta, Input* p_pInput);

	void Move(D3DXVECTOR3 p_Movement, bool IsInWorldSpace);
	void RotateX(float p_Degree, bool IsInWorldSpace);
	void RotateY(float p_Degree, bool IsInWorldSpace);
	void RotateZ(float p_Degree, bool IsInWorldSpace);


	D3DXVECTOR4 GetRight4();
	D3DXVECTOR4 GetUp4();
private:
	void UpdateViewMatrix();
	
	D3DXMATRIX m_Projection;
	D3DXMATRIX m_View;

	D3DXVECTOR3 m_Target;
	D3DXVECTOR3 m_Direction;
	D3DXVECTOR3 m_Up;
	D3DXVECTOR3 m_Right;
	float m_Distance;

	D3DXVECTOR2 SavedMousePos;
};