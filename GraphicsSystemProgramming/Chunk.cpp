#include "Chunk.h"

#include "Chunk_Structs.h"

void Chunk::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 16; y++)
			for (int z = 0; z < 16; z++)
			{
				if (rand() % 2 > 0)
				{
					m_BlockData[x][y][z] = 1;
				}
				else
				{
					m_BlockData[x][y][z] = 0;
				}
			}


	// Faces Zählen
	int FaceCount = 0;

	for (int x = 0; x < 16; x++)
		for (int y = 0; y < 16; y++)
			for (int z = 0; z < 16; z++)
			{
				// Wenn der eigene Block Solide ist, dann Faces generieren
				if (m_BlockData[x][y][z] != 0)
				{

					// Ein Face für jeden anliegenden LuftBlock
					if (m_BlockData[x + 1][y][z] == 0)
						FaceCount += 1;

					if (m_BlockData[x - 1][y][z] == 0)
						FaceCount += 1;

					if (m_BlockData[x][y + 1][z] == 0)
						FaceCount += 1;

					if (m_BlockData[x][y - 1][z] == 0)
						FaceCount += 1;

					if (m_BlockData[x][y][z + 1] == 0)
						FaceCount += 1;

					if (m_BlockData[x][y][z - 1] == 0)
						FaceCount += 1;
				}
			}



	ChunkVertexStruct* _Vertices = new ChunkVertexStruct[FaceCount * 4];
	int* _Indices = new int[FaceCount * 6];



}