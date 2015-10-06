#pragma once

#include "IScene.h"

#include "Cube.h"
#include "Camera.h"
#include "Input.h"

class Scene3D : public IScene
{
public:
	Scene3D();
	~Scene3D();

	virtual void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon) override;

	virtual void Update(float p_DeltaTime) override;
	virtual void Render() override;

	virtual void Shutdown() override;

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;


private:
	Cube* m_pCube;
	Camera* m_pCamera;
	Input* m_pInput;
};