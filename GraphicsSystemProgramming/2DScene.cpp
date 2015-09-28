#include "2DScene.h"

#include "VertexStruct.h"


Scene2D::Scene2D()
{
	m_pDevice = nullptr;
	m_pDeviceContext = nullptr;
}

Scene2D::~Scene2D()
{

}

void Scene2D::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pDevice = p_pDevice;
	m_pDeviceContext = p_pDevCon;

	D3DX11CreateShaderResourceViewFromFile(m_pDevice, TEXT("Dirt.jpg"),
		nullptr, nullptr, &m_pTexture, nullptr);

	D3DX11CreateShaderResourceViewFromFile(m_pDevice, TEXT("Grass.png"),
		nullptr, nullptr, &m_pTexture2, nullptr);

	m_pSpriteBatch = new SpriteBatch();
	m_pSpriteBatch->Init(m_pDevice, m_pDeviceContext);

	SpriteFont* _pFont = new SpriteFont();

	_pFont->Load("Calibri.fnt", m_pDevice);
}

void Scene2D::Update()
{

}

void Scene2D::Render()
{
	

	m_pSpriteBatch->Begin();

	for (int i = 0; i < 200000; i++)
	{
		int x = rand() % 1000;
		int y = rand() % 1000;
	
		m_pSpriteBatch->DrawTexture(m_pTexture2, Rect(x, y, 10, 10), D3DXVECTOR4(1, 1, 1, 1));

		//m_pSpriteBatch->DrawTexture(m_pTexture, Rect(x +10, y, 10, 10), Rect(0, 0, 0, 0), D3DXVECTOR4(1, 1, 1, 1));
	}

	m_pSpriteBatch->End();

}

void Scene2D::Shutdown()
{

}

