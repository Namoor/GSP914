#include "3DScene.h"

#include <string>

#include "TexturedMaterial.h"

Scene3D::Scene3D()
{

}

Scene3D::~Scene3D()
{

}

void Scene3D::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon)
{
	m_pDevice = p_pDevice;
	m_pDevCon = p_pDevCon;

	m_pCube = new Cube();
	m_pCube->Init(p_pDevice, p_pDevCon);

	m_pObjMesh = new ObjMesh();
	m_pObjMesh->Init(p_pDevice, p_pDevCon, "duck_triangulated_10k.obj");

	m_pCamera = new Camera();

	m_pInput = new Input();

	m_pTextureMaterial = new TexturedMaterial();
	m_pTextureMaterial->Init(p_pDevice, p_pDevCon);

	m_pTextureNormMaterial = new TexturedNormMaterial();
	m_pTextureNormMaterial->Init(p_pDevice, p_pDevCon);

	m_pSpriteBatch = new SpriteBatch();
	m_pCalibriFont = new SpriteFont();

	m_pSpriteBatch->Init(p_pDevice, p_pDevCon);
	m_pCalibriFont->Load("Calibri.fnt", p_pDevice);

	FramesSinceLastSecond = 0;
	TimeSinceLastSecond = 0;
	CurrentFPS = 0;


	int max = 10;

	for (int x = 0; x < max; x++)
		for (int y = 0; y < max; y++)
			for (int z = 0; z < max; z++)
			{
				if (rand() % 2 > 0)
				{
					GameObject* _pGameObject = new GameObject(m_pObjMesh, m_pTextureMaterial);

					_pGameObject->m_pTransform->SetPosition(D3DXVECTOR3(x, y, z) * 1.0f);

					m_Objects.push_back(_pGameObject);
				}
			}

}

void Scene3D::Update(float p_DeltaTime)
{
	m_pInput->Update();
	m_pCamera->Update(p_DeltaTime, m_pInput);

	TimeSinceLastSecond += p_DeltaTime;
	FramesSinceLastSecond += 1;

	if (TimeSinceLastSecond > 1)
	{
		TimeSinceLastSecond -= 1;

		CurrentFPS = FramesSinceLastSecond;
		FramesSinceLastSecond = 0;
	}

	if (m_pInput->GetKeyDown(KeyCode::K))
	{
		for (auto _It = m_Objects.begin(); _It != m_Objects.end(); _It++)
		{
			(*_It)->m_pMaterial = m_pTextureNormMaterial;
		}
	}

	if (m_pInput->GetKeyDown(KeyCode::L))
	{
		for (auto _It = m_Objects.begin(); _It != m_Objects.end(); _It++)
		{
			(*_It)->m_pMaterial = m_pTextureMaterial;
		}
	}
}

void Scene3D::Render()
{
	


	std::string _FPSString("FPS: ");
	_FPSString.append(std::to_string( CurrentFPS));

	char Buffer[200];
	sprintf_s(Buffer, "FPS: %d", CurrentFPS);

	for (auto _It = m_Objects.begin(); _It != m_Objects.end(); _It++)
	{
		break;
		(*_It)->Render(m_pCamera);
	}

	m_pSpriteBatch->Begin();

	m_pSpriteBatch->DrawString(m_pCalibriFont, _FPSString.c_str(), 0, 0, 40, D3DXVECTOR4(1, 1, 1, 1));

	m_pSpriteBatch->End();

}

void Scene3D::Shutdown()
{

}