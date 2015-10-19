#pragma once

#include "d3dinclude.h"

#include "Camera.h"

#include "Transform.h"

#include "Material.h"

class TexturedMaterial : public Material
{
public:
	TexturedMaterial();

	void Init(ID3D11Device* p_pDevice, ID3D11DeviceContext* p_pDevCon);

	virtual void Bind() override;

private:
	ID3D11ShaderResourceView* m_pTexture;

};