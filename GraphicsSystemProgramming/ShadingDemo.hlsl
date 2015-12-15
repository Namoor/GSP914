struct VertexShaderOut
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
	float3 tangent : NORMAL1;
	float3 bitangent : NORMAL2;
	float4 pos_Light : POSITION1;
};

cbuffer Matrix
{
	float4x4 WVPMatrix;
	float4x4 WMatrix;
	float4x4 WNormalMatrix;
	float4x4 WVPLight;
};

cbuffer LightData
{
	// WorldMatrix
	float4x4 WMatrix_PS;

	// Global
	float4 RGB_CameraPosition;

	// DirectionalLight
	float4 DL_LightDir;
	float4 DL_RGBLightColor;

	// Point Light
	float4 PL_LightPos;
	float4 PL_RGBLightColor_ALightRange;

	// MaterialProperties
	float4 R_SpecRoughness_G_SpecIntensity;
};


Texture2D gTexture[4];
SamplerState gSampler;

VertexShaderOut VShader(float3 pos : POSITION0, float2 uv : TEXCOORD0, float3 Normal : NORMAL0, float3 Tangent : NORMAL1, float3 BiTangent : NORMAL2)
{
	VertexShaderOut _Out;

	float4 _RGBNrm_ADisplacement = gTexture[2].SampleLevel(gSampler, uv, 0);

		pos += Normal * _RGBNrm_ADisplacement.a * 0.1;

	_Out.pos = mul(WVPMatrix, float4(pos, 1));
	_Out.worldPos = mul(WMatrix, float4(pos, 1));
	_Out.uv = uv;
	_Out.normal = mul(WNormalMatrix, float4(Normal, 0));
	_Out.tangent = mul(WNormalMatrix, float4(Tangent, 0));
	_Out.bitangent = mul(WNormalMatrix, float4(BiTangent, 0));

	_Out.pos_Light = _Out.worldPos - DL_LightDir * 0.1f;
	_Out.pos_Light = mul(WVPLight, float4(_Out.pos_Light.xyz, 1));
	//_Out.pos_Light /= _Out.pos_Light.w;


	return _Out;
}




float3 PhongShading(float3 LightInDir, float3 Normal, float3 ViewOutDir, float3 SpecularColor, float SpecularIntensity, float SpecularExponent)
{
	float3 OutDir = normalize(LightInDir - 2 * Normal * dot(Normal, LightInDir));
		// float3 OutDir = reflect(LightInDir, Normal);

		float SpecPower = saturate(dot(OutDir, ViewOutDir));

	SpecPower = pow(SpecPower, SpecularExponent);

	return SpecularColor * SpecPower * SpecularIntensity;
}

float3 BlinnPhongShading(float3 LightInDir, float3 Normal, float3 ViewOutDir, float3 SpecularColor, float SpecularIntensity, float SpecularExponent)
{
	float3 HalfVec = normalize((-1 * LightInDir + ViewOutDir) * 0.5);

		float SpecPower = saturate(dot(Normal, HalfVec));

	SpecPower = pow(SpecPower, SpecularExponent);

	return SpecularColor * SpecPower * SpecularIntensity;
}

float GetLightIntensity(float2 uvs, float CompareDepth)
{
	float LightDepth = gTexture[3].Sample(gSampler, uvs).r;


	if (CompareDepth < LightDepth)
	{
		return 1;
	}

	return 0;
}

float4 PShader(VertexShaderOut _In) : SV_TARGET
{
	_In.normal = normalize(_In.normal);

	_In.pos_Light /= _In.pos_Light.w;

	float2 _LightMapUVs = _In.pos_Light.xy * 0.5f + 0.5f;
		_LightMapUVs.y = 1 - _LightMapUVs.y;

	float VertexDepth = _In.pos_Light.z;
	//float LightDepth = gTexture[3].Sample(gSampler, _LightMapUVs).r;
	//
	//float DirLightIntensity = 0;
	//
	//if (VertexDepth < LightDepth)
	//{
	//	DirLightIntensity = 1;
	//}

	const float _PixelOffsetX = 1.0f / 800.0f;
	const float _PixelOffsetY = 1.0f / 600.0f;

	const float2 Offsets[9] =
	{
		float2(-_PixelOffsetX, -_PixelOffsetY), float2(-_PixelOffsetX, -0), float2(-_PixelOffsetX, +_PixelOffsetY),
		float2(0, -_PixelOffsetY), float2(0, -0), float2(0, +_PixelOffsetY),
		float2(_PixelOffsetX, -_PixelOffsetY), float2(_PixelOffsetX, -0), float2(_PixelOffsetX, +_PixelOffsetY)
	};

	

	float DirLightIntensity = 0;

	for (int x = 0; x < 9; x++)
	{
		DirLightIntensity += GetLightIntensity(_LightMapUVs + Offsets[x], VertexDepth);
	}

	DirLightIntensity /= 9;


	float3 PointToCamera = normalize(RGB_CameraPosition.xyz - _In.worldPos.xyz);

		// Texture
		float4 _Tex = gTexture[0].Sample(gSampler, _In.uv);

		//return float4(_Tex.rgb, 1);

		float4 _SpecValues = gTexture[1].Sample(gSampler, _In.uv);
		float4 _NrmTex = gTexture[2].Sample(gSampler, _In.uv);

		float3 _Nrm = (_NrmTex.xzy - 0.5f) * 2;

		_Nrm.z *= -1;



	float3 _Normal = _In.tangent * _Nrm.x +
		_In.normal *_Nrm.y +
		_In.bitangent * _Nrm.z;

	//_Normal = _In.normal;


	// -- Directional Light --

	// Diffuse Anteil
	float3 DirectionalLight = saturate(dot(-DL_LightDir, _Normal)) * DL_RGBLightColor.rgb; // * C * I

		// Specular Anteil
		float3 DirLight_Spec = BlinnPhongShading(DL_LightDir, _Normal, PointToCamera, DL_RGBLightColor.rgb, R_SpecRoughness_G_SpecIntensity.g * _SpecValues.r, R_SpecRoughness_G_SpecIntensity.r * _SpecValues.g);

		// -- Point Light --

		// Diffuse Anteil
		float3 PL_LightDir = _In.worldPos.xyz - PL_LightPos.xyz;
		float PL_LightDir_Length = length(PL_LightDir);

	float PL_LightStrength = saturate(1 - PL_LightDir_Length / PL_RGBLightColor_ALightRange.a);

	float3 PointLight = saturate(dot(-PL_LightDir / PL_LightDir_Length, _Normal)) * saturate(PL_RGBLightColor_ALightRange.rgb) * PL_LightStrength* PL_LightStrength * PL_LightStrength;

		// Specaluar Anteil
		float3 PointLight_Spec = BlinnPhongShading(PL_LightDir / PL_LightDir_Length, _Normal, PointToCamera, PL_RGBLightColor_ALightRange, R_SpecRoughness_G_SpecIntensity.g  * _SpecValues.r, R_SpecRoughness_G_SpecIntensity.r  * _SpecValues.g);

		// -- Ambient Light --
		float3 AmbientLight = float3(1, 1, 1);

		// Composition
		float3 LightIntensity = (DirectionalLight * DirLightIntensity + PointLight * 0)* 0.7 + AmbientLight * 0.3 + DirLight_Spec + PointLight_Spec;



		//return float4(_In.bitangent.xyz / 2 + 0.5, 1);

		//return float4(_NrmTex.a, _NrmTex.a, _NrmTex.a, 1);

		//return float4(LightDepth, 0, 0, 1);

		// Final Result
		return float4(_Tex.rgb * LightIntensity.rgb, _Tex.a);
}