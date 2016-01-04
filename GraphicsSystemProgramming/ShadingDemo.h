#pragma once


#include "d3dinclude.h"
#include "Camera.h"
#include "DirectionalLight.h"

#include "PerlinNoiseOctaves.h"

class ShadingDemo
{
public:
	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon);

	void Update(float p_DeltaTime);
	void Render(Camera* p_pCamera, DirectionalLight* p_pDirectionalLight);
	void RenderDepth(D3DXMATRIX p_ViewProj, D3DXVECTOR4 RenderingPosition);

private:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11InputLayout* m_pInputLayout;

	ID3D11Buffer* m_pMatrixConstantBuffer;
	ID3D11Buffer* m_pLightConstantBuffer;

	ID3D11ShaderResourceView* m_pTexture;
	ID3D11ShaderResourceView* m_pTexture_Spec;
	ID3D11ShaderResourceView* m_pTexture_Nrm;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;

	float GetHeightAt(float x, float z);

	int m_IndexCount;

	float TimeSinceStart;

	PerlinNoiseOctave* m_pPerlin;
};