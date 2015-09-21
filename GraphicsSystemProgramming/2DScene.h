#pragma once

#include "IScene.h"

class Scene2D : public IScene
{
public:
	Scene2D();
	~Scene2D();

	virtual void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon) override;

	virtual void Update() override;
	virtual void Render() override;

	virtual void Shutdown() override;


private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;

};