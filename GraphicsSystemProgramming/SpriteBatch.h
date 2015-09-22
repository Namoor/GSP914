#pragma once
#include "SpriteBatch_Vertex.h"
#include "d3dinclude.h"

#include "Rect.h"

class SpriteBatch
{
public:
	SpriteBatch();


	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon);

	void Begin();
	void End();

	void DrawTexture(ID3D11ShaderResourceView* p_pTexture, Rect Destination, Rect Source, D3DXVECTOR4 p_Color);

private:
	SpriteBatch_Vertex* m_pVertices;
	int m_DrawCallCount;

	float m_ScreenWidth;
	float m_ScreenHeight;

private:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11InputLayout* m_pInputLayout;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;
};