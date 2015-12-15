#pragma once

#include "IScene.h"

#include "SpriteBatch.h"

#include "SpriteFont.h"

#include "Input.h"

class Scene2D : public IScene
{
public:
	Scene2D();
	~Scene2D();

	virtual void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon, ID3D11RenderTargetView* p_pBackBuffer, ID3D11DepthStencilView* p_pDSV) override;

	virtual void Update(float p_DeltaTime) override;
	virtual void Render() override;

	virtual void Shutdown() override;


private:
	SpriteBatch* m_pSpriteBatch;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDeviceContext;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11ShaderResourceView* m_pTexture;
	ID3D11ShaderResourceView* m_pTexture2;

	ID3D11InputLayout* m_pInputLayout;


	SpriteFont* m_pFont;

	Input* m_pInput;

	float x;
};