#include "3DScene.h"

#include <string>

#include "TexturedMaterial.h"

Scene3D::Scene3D()
{
	TimeSinceStart = 0;
}

Scene3D::~Scene3D()
{

}

void Scene3D::Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, ID3D11RenderTargetView* p_pBackBuffer, ID3D11DepthStencilView* p_pDSV)
{
	m_pDevice = p_pDevice;
	m_pDevCon = p_pDevCon;

	m_pDSV = p_pDSV;
	m_pBackBuffer = p_pBackBuffer;

	m_pCube = new Cube();
	m_pCube->Init(p_pDevice, p_pDevCon);

	m_pParticleSystem = new ParticleSystem();
	m_pParticleSystem->Init(m_pDevice, m_pDevCon);

	m_pObjMesh = new ObjMesh();
	//m_pObjMesh->Init(p_pDevice, p_pDevCon, "duck_triangulated_10k.obj");

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


	int max = 32;

	for (int x = 0; x < max; x++)
		for (int y = 0; y < max; y++)
			for (int z = 0; z < max; z++)
			{
				if (rand() % 2 > 0)
				{
					GameObject* _pGameObject = new GameObject(m_pCube, m_pTextureMaterial);

					_pGameObject->m_pTransform->SetPosition(D3DXVECTOR3(x, y, z) * 1.0f);
					//_pGameObject->m_pTransform->SetScale(D3DXVECTOR3(0.02f, 0.02f, 0.02f));

					m_Objects.push_back(_pGameObject);
				}
			}

	m_pChunk = new Chunk();
	m_pChunk->Init(m_pDevice, m_pDevCon);

	m_pPlane = new Plane();
	m_pPlane->Init(m_pDevice, m_pDevCon);

	m_pShadingDemo = new ShadingDemo();
	m_pShadingDemo->Init(m_pDevice, m_pDevCon);

	m_pDirectionalLight = new DirectionalLight();
	m_pDirectionalLight->Init(m_pDevice, m_pDevCon);

	
	TilingTerrain_Data* _pData = new TilingTerrain_Data();
	_pData->Init(33, 33, TilingTerrainType::Grass);

	//_pData->SetData(1, 1, TilingTerrainType::Snow);
	_pData->SetData(1, 2, TilingTerrainType::Snow);
	_pData->SetData(2, 1, TilingTerrainType::Snow);
	//_pData->SetData(2, 2, TilingTerrainType::Snow);
	
	m_pTilingTerrain = new TilingTerrain();
	m_pTilingTerrain->Init(m_pDevice, m_pDevCon, 32, 32, _pData);
}

void Scene3D::Update(float p_DeltaTime)
{
	TimeSinceStart += p_DeltaTime;

	D3DXMATRIX _Rotation;
	D3DXMatrixRotationY(&_Rotation, 1 * TimeSinceStart);


	D3DXVECTOR4 _LightOrig = D3DXVECTOR4(1, -1, 1, 0);
	D3DXVECTOR3 _LightRotated;
	D3DXVec4Transform(&_LightOrig, &_LightOrig, &_Rotation);

	_LightRotated.x = _LightOrig.x;
	_LightRotated.y = _LightOrig.y;
	_LightRotated.z = _LightOrig.z;


	m_pDirectionalLight->SetDirection(_LightRotated);



	m_pChunk->Update(p_DeltaTime);

	m_pInput->Update();
	m_pCamera->Update(p_DeltaTime, m_pInput);

	TimeSinceLastSecond += p_DeltaTime;
	FramesSinceLastSecond += 1;

	m_pParticleSystem->Update(p_DeltaTime);

	D3DXVECTOR3 _RndVec = D3DXVECTOR3(rand() % 2000 / 1000.0f - 1.0f, 1, rand() % 2000 / 1000.0f - 1.0f);
	D3DXVec3Normalize(&_RndVec, &_RndVec);

	D3DXVECTOR4 _RndVec4 = D3DXVECTOR4(_RndVec.x *0.5f + 0.5f, _RndVec.y *0.5f + 0.5f, _RndVec.z *0.5f + 0.5f, 0.1f);

	//m_pParticleSystem->SpawnParticle(_RndVec, _RndVec, _RndVec4);

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

	m_pShadingDemo->Update(p_DeltaTime);
}

void Scene3D::Render()
{
	// 1. ShadowMap Binden
	//m_pDirectionalLight->GetShadowMap()->Bind();

	// 2. Aus Sicht des Lichtes Rendern
	
	//m_pShadingDemo->RenderDepth(m_pDirectionalLight->GetViewProjectionMatrix(), D3DXVECTOR4(0, 0, 0, 0));



	// 3. Bildschirm binden
	//m_pDevCon->OMSetRenderTargets(1, &m_pBackBuffer, m_pDSV);
	//m_pDevCon->ClearDepthStencilView(m_pShadowMap->GetDSV(), D3D11_CLEAR_FLAG::D3D11_CLEAR_DEPTH, 1.0f, 0);


	D3D11_VIEWPORT _Viewport;

	_Viewport.TopLeftX = 0;
	_Viewport.TopLeftY = 0;

	_Viewport.Width = 800;
	_Viewport.Height = 600;

	_Viewport.MinDepth = 0;
	_Viewport.MaxDepth = 1;
	m_pDevCon->RSSetViewports(1, &_Viewport);


	// 4. Scene aus sicht der Kamera rendern

	//m_pShadingDemo->Render(m_pCamera, m_pDirectionalLight);

	m_pTilingTerrain->Render(m_pCamera);

	std::string _FPSString("FPS: ");
	_FPSString.append(std::to_string(CurrentFPS));

	char Buffer[200];
	sprintf_s(Buffer, "FPS: %d", CurrentFPS);

	//for (auto _It = m_Objects.begin(); _It != m_Objects.end(); _It++)
	//{
	//	//(*_It)->Render(m_pCamera);
	//	
	//}

	//m_pChunk->Render(m_pCamera);

	//m_pPlane->Render(m_pCamera);

	//m_pParticleSystem->Render(m_pCamera);

	m_pSpriteBatch->Begin();

	//m_pSpriteBatch->DrawTexture(m_pDirectionalLight->GetShadowMap()->GetTexture(), Rect(0, 0, 400, 300));

	m_pSpriteBatch->DrawString(m_pCalibriFont, _FPSString.c_str(), 0, 0, 40, D3DXVECTOR4(1, 1, 1, 1));

	m_pSpriteBatch->End();

}

void Scene3D::Shutdown()
{

}