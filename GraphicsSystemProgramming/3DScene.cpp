#include "3DScene.h"


Scene3D::Scene3D()
{

}

Scene3D::~Scene3D()
{

}

void Scene3D::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pDevice = p_pDevice;
	m_pDevCon = p_pDevCon;

	m_pCube = new Cube();
	m_pCube->Init(p_pDevice, p_pDevCon);

	m_pCamera = new Camera();

	m_pInput = new Input();
}

void Scene3D::Update(float p_DeltaTime)
{
	m_pInput->Update();
	m_pCamera->Update(p_DeltaTime, m_pInput);

	m_pCube->Update(p_DeltaTime, m_pCamera);
}

void Scene3D::Render()
{
	m_pCube->Render();
}

void Scene3D::Shutdown()
{

}