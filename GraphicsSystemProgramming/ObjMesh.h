#pragma once

#include "Mesh.h"
#include "ObjMesh_VertexStruct.h"
#include <vector>

class ObjMesh : public Mesh
{
public:
	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, char* p_pFileName);


private:
	int AddVertice(ObjMesh_Vertice Vertex, std::vector<ObjMesh_Vertice>* p_pVertices);
};