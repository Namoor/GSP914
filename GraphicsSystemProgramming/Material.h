#pragma once

#include "Transform.h"
#include "Camera.h"

#include "d3dinclude.h"

class Material
{
public:
	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon);
	virtual void SetMatrixData(Transform* p_pTransform, Camera* p_pCamera);

	virtual void Bind() = 0;

	ID3D10Blob* LoadShaders(char* VertexShaderSourceFile, char* VertexShaderFunctionName, char* PixelShaderSourceFile, char* PixelShaderFunctionName);

protected:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;

	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11InputLayout* m_pInputLayout;

	ID3D11Buffer* m_pMatrixConstantBuffer;
};