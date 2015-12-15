#pragma once

#include "IScene.h"

#include "Cube.h"
#include "Camera.h"
#include "Input.h"

#include "SpriteBatch.h"
#include "SpriteFont.h"

#include <list>
#include "GameObject.h"

#include "TexturedMaterial.h"
#include "TexturedNormMaterial.h"

#include "ObjMesh.h"

#include "ParticleSystem.h"

#include "Chunk.h"

#include "Plane.h"

#include "ShadingDemo.h"

#include "DirectionalLight.h"

class Scene3D : public IScene
{
public:
	Scene3D();
	~Scene3D();

	virtual void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, ID3D11RenderTargetView* p_pBackBuffer, ID3D11DepthStencilView* p_pDSV) override;

	virtual void Update(float p_DeltaTime) override;
	virtual void Render() override;

	virtual void Shutdown() override;

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;

	ID3D11RenderTargetView* m_pBackBuffer;
	ID3D11DepthStencilView* m_pDSV;

private:
	Cube* m_pCube;
	ObjMesh* m_pObjMesh;
	Camera* m_pCamera;
	Input* m_pInput;

	SpriteBatch* m_pSpriteBatch;
	SpriteFont* m_pCalibriFont;

	int FramesSinceLastSecond;
	float TimeSinceLastSecond;

	int CurrentFPS;

	ParticleSystem* m_pParticleSystem;

	Chunk* m_pChunk;

	Plane* m_pPlane;

	ShadingDemo* m_pShadingDemo;

	DirectionalLight* m_pDirectionalLight;

	float TimeSinceStart;

private:
	TexturedMaterial* m_pTextureMaterial;
	TexturedNormMaterial* m_pTextureNormMaterial;

	std::list<GameObject*> m_Objects;
};