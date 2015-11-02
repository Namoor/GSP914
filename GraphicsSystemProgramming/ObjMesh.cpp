#include "ObjMesh.h"

#include "ObjMesh_VertexStruct.h"

#include <fstream>
#include <vector>

#include "d3dinclude.h"

void ObjMesh::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, char* p_pFileName)
{
	// File Lesen und VertexData und IndexData erstellen
	int VertexCount = 0;
	int IndexCount = 0;

	std::vector<D3DXVECTOR3> _V;
	std::vector<D3DXVECTOR2> _VT;
	std::vector<D3DXVECTOR3> _VN;

	std::vector<ObjMesh_Vertice> _Vertices;
	std::vector<UINT32> _Indices;

	char _Trash[512];

	std::ifstream _File(p_pFileName);

	while (!_File.eof())
	{
		char MajorType;
		MajorType = _File.get();

		if (MajorType == 'v')
		{
			char MinorType;
			MinorType = _File.get();

			float _X;
			float _Y;
			float _Z;

			// VertexInformation
			if (MinorType == ' ')
			{
				// VertexInformation	
				_File >> _X;
				_File >> _Y;
				_File >> _Z;
				_V.push_back(D3DXVECTOR3(_X, _Y, _Z));

				_File.getline(_Trash, 512);
			}
			else if (MinorType == 't')
			{
				// Textur Eintrag
				_File >> _X;
				_File >> _Y;
				_VT.push_back(D3DXVECTOR2(_X, _Y));
				_File.getline(_Trash, 512);
			}
			else if (MinorType == 'n')
			{
				// Normal Eintrag
				_File >> _X;
				_File >> _Y;
				_File >> _Z;
				_VN.push_back(D3DXVECTOR3(_X, _Y, _Z));
				_File.getline(_Trash, 512);
			}
			else
			{
				int asd = 0;
			}
		}
		else if(MajorType == 'f')
		{
			// Face
			ObjMesh_Vertice _Vertex;
			char Blank; // '/'

			int Index1 = -1;
			int Index2 = -1;
			int Index3 = -1;

			_File >> _Vertex.V;
			Blank = _File.get();
			_File >> _Vertex.VT;
			Blank = _File.get();
			_File >> _Vertex.VN;

			Index1 = AddVertice(_Vertex, &_Vertices);

			_File >> _Vertex.V;
			Blank = _File.get();
			_File >> _Vertex.VT;
			Blank = _File.get();
			_File >> _Vertex.VN;

			Index2 = AddVertice(_Vertex, &_Vertices);

			_File >> _Vertex.V;
			Blank = _File.get();
			_File >> _Vertex.VT;
			Blank = _File.get();
			_File >> _Vertex.VN;

			Index3 = AddVertice(_Vertex, &_Vertices);

			_Indices.push_back(Index1);
			_Indices.push_back(Index2);
			_Indices.push_back(Index3);

			_File.getline(_Trash, 512);

		}
		else
		{
			int asd = 0;
		}
	}




	// VertexBuffer und IndexBuffer Initialisieren
	Mesh::Init(p_pDevice, p_pDevCon, _Vertices.size(), _Indices.size(), sizeof(ObjMesh_Vertex));


	// Daten in VertexBuffer und IndexBuffer übertragen

	ObjMesh_Vertex* _D3DVertices = new ObjMesh_Vertex[_Vertices.size()];

	for (int x = 0; x < _Vertices.size(); x++)
	{
		_D3DVertices[x].Color = D3DXVECTOR4(1, 1, 1, 1);
		int VT = _Vertices[x].VT - 1;
		if (VT >= _VT.size())
			VT = _VT.size() - 1;
		
		int VN = _Vertices[x].VN - 1;
		if (VN >= _VN.size())
			VN = _VN.size() - 1;
		
		int V = _Vertices[x].V - 1;
		if (V >= _V.size())
			V = _V.size() - 1;

		_D3DVertices[x].TexCoord = _VT[VT];
		_D3DVertices[x].Normal = _VN[VN];
		_D3DVertices[x].Position= _V[V];
	}

	SetIndexData(&(_Indices[0]));
	SetVertexData(_D3DVertices);


	delete[] _D3DVertices;
}

int ObjMesh::AddVertice(ObjMesh_Vertice Vertex, std::vector<ObjMesh_Vertice>* p_pVertices)
{
	// Suchen Ob bereits vorhanden
	int _Count = p_pVertices->size();
	for (int x = 0; x < _Count; x++)
	{
		ObjMesh_Vertice _Current = (*p_pVertices)[x];

		if (_Current.V == Vertex.V && _Current.VT == Vertex.VT && _Current.VN == Vertex.VN)
			return x;
	}

	// Ansonsten Hinten Anhängen und zurückgeben
	p_pVertices->push_back(Vertex);
	return _Count;
}