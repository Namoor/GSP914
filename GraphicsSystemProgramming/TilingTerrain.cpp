#include "TilingTerrain.h"


void TilingTerrain::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, int SizeX, int SizeY)
{
	m_pDevice = p_pDevice;
	m_pDeviceContext = p_pDevCon;

	D3DX11CreateShaderResourceViewFromFile(m_pDevice, TEXT("wc3_atlas.png"), nullptr, nullptr, &m_pTexture, nullptr);

	m_pGrass = new TilingTerrain_Layer(0, 0.0f, 1.0f, 0.25f, true, TilingTerrainType::Grass);
	m_pSnow = new TilingTerrain_Layer(0, 0.25f, 1.0f, 0.25f, true, TilingTerrainType::Snow);
	m_pStone = new TilingTerrain_Layer(0, 0.5f, 1.0f, 0.25f, true, TilingTerrainType::Stone);
	m_pCity = new TilingTerrain_Layer(0, 0.75f, 1.0f, 0.25f, false, TilingTerrainType::City);
}









void TilingTerrain::Update(float p_DeltaTime)
{

}

void TilingTerrain::Render(Camera* p_pCamera)
{

}