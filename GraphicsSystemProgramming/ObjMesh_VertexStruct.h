#pragma once

#include "d3dinclude.h"

struct ObjMesh_Vertex
{
	D3DXVECTOR3 Position;
	D3DXVECTOR4 Color;
	D3DXVECTOR2 TexCoord;
	D3DXVECTOR3 Normal;
};

struct ObjMesh_Vertice
{
	int V;
	int VT;
	int VN;
};