#pragma once

#include "d3dinclude.h"

#include "Camera.h"

<<<<<<< HEAD

#include "Mesh.h"

class Cube : public Mesh
=======
#include "Material.h"

class Cube
>>>>>>> 942773305c7e7f345f8eb51ce652ba998b8abf90
{
public:
	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon);
<<<<<<< HEAD
=======

	void Update(float DeltaTime, Camera* p_pCamera);

	void Render();


private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;

private:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;





	Material* m_pMaterial;

>>>>>>> 942773305c7e7f345f8eb51ce652ba998b8abf90
};