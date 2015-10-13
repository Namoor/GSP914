#include "2DScene.h"

#include "VertexStruct.h"


Scene2D::Scene2D()
{
	m_pDevice = nullptr;
	m_pDeviceContext = nullptr;

	x = 0;
}

Scene2D::~Scene2D()
{

}

void Scene2D::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pDevice = p_pDevice;
	m_pDeviceContext = p_pDevCon;

	m_pInput = new Input();

	D3DX11CreateShaderResourceViewFromFile(m_pDevice, TEXT("Dirt.jpg"),
		nullptr, nullptr, &m_pTexture, nullptr);

	D3DX11CreateShaderResourceViewFromFile(m_pDevice, TEXT("Grass.png"),
		nullptr, nullptr, &m_pTexture2, nullptr);

	m_pSpriteBatch = new SpriteBatch();
	m_pSpriteBatch->Init(m_pDevice, m_pDeviceContext);

	m_pFont = new SpriteFont();

	m_pFont->Load("Calibri.fnt", m_pDevice);

	
}

void Scene2D::Update(float p_DeltaTime)
{
	m_pInput->Update();

	x += m_pInput->GetAxis(0, GamePadAxis::LStickX) * 100 * p_DeltaTime;
}

void Scene2D::Render()
{
	

	m_pSpriteBatch->Begin();
	if (m_pInput->GetKey(KeyCode::LeftMouse))
		m_pSpriteBatch->DrawString(m_pFont, "the quick brown fox jumps over the lazy dog", 100, 100, 32, D3DXVECTOR4(1, 1, 1, 1));
	m_pSpriteBatch->DrawString(m_pFont, "THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG", 100, 200, 32, D3DXVECTOR4(1, 1, 1, 1));
	m_pSpriteBatch->DrawString(m_pFont, "! ? ü Ü ö Ö ä Ä _ : ß .", x, 300, 32, D3DXVECTOR4(1, 1, 1, 1));


	//for (int i = 0; i < 200000; i++)
	//{
	//	int x = rand() % 1000;
	//	int y = rand() % 1000;
	//
	//	m_pSpriteBatch->DrawTexture(m_pTexture2, Rect(x, y, 10, 10), D3DXVECTOR4(1, 1, 1, 1));
	//
	//	//m_pSpriteBatch->DrawTexture(m_pTexture, Rect(x +10, y, 10, 10), Rect(0, 0, 0, 0), D3DXVECTOR4(1, 1, 1, 1));
	//}

	m_pSpriteBatch->End();

}

void Scene2D::Shutdown()
{

}

