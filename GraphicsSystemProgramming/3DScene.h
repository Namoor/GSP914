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

class Scene3D : public IScene
{
public:
	Scene3D();
	~Scene3D();

	virtual void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon) override;

	virtual void Update(float p_DeltaTime) override;
	virtual void Render() override;

	virtual void Shutdown() override;

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;


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
private:
	TexturedMaterial* m_pTextureMaterial;
	TexturedNormMaterial* m_pTextureNormMaterial;

	std::list<GameObject*> m_Objects;
};