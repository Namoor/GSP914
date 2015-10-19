#pragma once

<<<<<<< HEAD
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

=======
#include "d3dinclude.h"

#include "Camera.h"

class Material
{
public:
	Material();

	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon);

	void Update(float DeltaTime, Camera* p_pCamera);

	void Bind();

private:
	ID3D11Device* m_pDevice;
	ID3D11DeviceContext* m_pDevCon;

private:
>>>>>>> 942773305c7e7f345f8eb51ce652ba998b8abf90
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;

	ID3D11InputLayout* m_pInputLayout;

<<<<<<< HEAD
	ID3D11Buffer* m_pMatrixConstantBuffer;
=======

	ID3D11Buffer* m_pMatrixConstantBuffer;
	ID3D11ShaderResourceView* m_pTexture;

	D3DXMATRIX WorldMatrix;
>>>>>>> 942773305c7e7f345f8eb51ce652ba998b8abf90
};