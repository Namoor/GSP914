#pragma once

#include "d3dinclude.h"

#include "Camera.h"

#include "Material.h"

class Cube
{
public:

	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon);

	void Update(float DeltaTime, Camera* p_pCamera);

	void Render();


private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;

private:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;





	Material* m_pMaterial;

};