#include "GameObject.h"


GameObject::GameObject(Mesh* p_pMesh, Material* p_pMaterial)
{
	m_pMesh = p_pMesh;
	m_pMaterial = p_pMaterial;
	m_pTransform = new Transform();
}

void GameObject::Update(float Delta)
{

}

void GameObject::Render(Camera* p_pCamera)
{
	m_pMaterial->SetMatrixData(m_pTransform, p_pCamera);
	m_pMesh->Render(m_pMaterial);
}