#pragma once

#include "d3dinclude.h"

struct ShadingDemo_Vertex
{
	D3DXVECTOR3 Position;	// 3 * 4b = 12 bytes
	D3DXVECTOR2 UV;			// 2 * 4b = 08 bytes
	D3DXVECTOR3 Normal;		// 3 * 4b = 12 bytes
	D3DXVECTOR3 Tangent;	// 3 * 4b = 12 bytes
	D3DXVECTOR3 BiTangent;
};


struct ShadingDemo_LightConstantBuffer
{
	// Worldmatrix
	D3DXMATRIX M;


	// Global
	D3DXVECTOR4 CameraPosition;

	// Directional Light
	D3DXVECTOR4 DirectionalLightDirection;
	D3DXVECTOR4 DirectionalLightColor;

	// Point Light
	D3DXVECTOR4 PointLightPosition;
	D3DXVECTOR4 PointLightColor;

	// Material Properties
	D3DXVECTOR4 R_SpecRoughness_G_SpecIntensity;
};

struct ShadingDemo_MatrixConstantBuffer
{
	D3DXMATRIX MVP;
	D3DXMATRIX M;
	D3DXMATRIX M_TransInv;
	D3DXMATRIX LightViewProj;
};