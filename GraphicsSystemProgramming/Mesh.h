#pragma once

#include "TexturedMaterial.h"

#include "d3dinclude.h"

class Mesh
{
public:
	Mesh();

	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, int VertexCount, int IndexCount, int VertexByteSize);

	void Render(Material* p_pMaterial);

protected:
	void SetVertexData(void* p_pVertexData);
	void SetIndexData(void* p_pIndexData);


private:
	int m_VertexCount;
	int m_IndexCount;
	int m_VertexByteSize;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
};