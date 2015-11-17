#pragma once


#include "d3dinclude.h"
#include "Camera.h"

class ShadingDemo
{
public:
	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon);

	void Update(float p_DeltaTime);
	void Render(Camera* p_pCamera);

private:
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11InputLayout* m_pInputLayout;

	ID3D11Buffer* m_pMatrixConstantBuffer;
	ID3D11Buffer* m_pLightConstantBuffer;

	ID3D11ShaderResourceView* m_pTexture;

	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;

	float GetHeightAt(float x, float z);
};