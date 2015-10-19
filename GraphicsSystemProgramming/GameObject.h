#pragma once

#include "Camera.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"


class GameObject
{
public:
	GameObject(Mesh* p_pMesh, Material* p_pMaterial);

public:
	void Update(float Delta);
	void Render(Camera* p_pCamera);

public:
	Mesh* m_pMesh;
	Material* m_pMaterial;
	Transform* m_pTransform;
};