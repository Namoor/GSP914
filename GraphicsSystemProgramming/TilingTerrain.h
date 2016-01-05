#pragma once


#include "TilingTerrain_Layer.h"
#include "TilingTerrain_Data.h"
#include "Camera.h"

class TilingTerrain
{
public:
	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, int SizeX, int SizeY, TilingTerrain_Data* p_pData);

	void Update(float p_DeltaTime);
	void Render(Camera* p_pCamera);

private:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	ID3D11InputLayout* m_pInputLayout;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11Buffer* m_pMatrixConstantBuffer;


	TilingTerrain_Layer* m_pGrass;
	TilingTerrain_Layer* m_pStone;
	TilingTerrain_Layer* m_pSnow;
	TilingTerrain_Layer* m_pCity;


	ID3D11ShaderResourceView* m_pTexture;


	int m_IndexCount;
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;

};