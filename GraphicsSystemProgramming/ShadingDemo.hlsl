struct VertexShaderOut
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
};

cbuffer Matrix
{
	float4x4 WVPMatrix;
	float4x4 WMatrix;
	float4x4 WNormalMatrix;
};

cbuffer LightData
{
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


VertexShaderOut VShader(float3 pos : POSITION0, float2 uv : TEXCOORD0, float3 Normal : NORMAL0)
{
	VertexShaderOut _Out;

	_Out.pos = mul(WVPMatrix, float4(pos, 1));
	_Out.worldPos = mul(WMatrix, float4(pos, 1));
	_Out.uv = uv;
	_Out.normal = mul(WNormalMatrix, float4(Normal, 0));

	return _Out;
}



Texture2D gTexture[3];
SamplerState gSampler;

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



float4 PShader(VertexShaderOut _In) : SV_TARGET
{
	_In.normal = normalize(_In.normal);

	float3 PointToCamera = normalize(RGB_CameraPosition.xyz - _In.worldPos.xyz);

		// Texture
		float4 _Tex = gTexture[0].Sample(gSampler, _In.uv);
		float4 _SpecValues = gTexture[1].Sample(gSampler, _In.uv);
		float3 _Nrm = (gTexture[2].Sample(gSampler, _In.uv).xzy - 0.5) * 2;

		_Nrm.z *= -1;



	// -- Directional Light --

	// Diffuse Anteil
	float3 DirectionalLight = saturate(dot(-DL_LightDir, _Nrm)) * DL_RGBLightColor.rgb; // * C * I

		// Specular Anteil
		float3 DirLight_Spec = BlinnPhongShading(DL_LightDir, _Nrm, PointToCamera, DL_RGBLightColor.rgb, R_SpecRoughness_G_SpecIntensity.g * _SpecValues.r, R_SpecRoughness_G_SpecIntensity.r * _SpecValues.g);

		// -- Point Light --

		// Diffuse Anteil
		float3 PL_LightDir = _In.worldPos.xyz - PL_LightPos.xyz;
		float PL_LightDir_Length = length(PL_LightDir);

	float PL_LightStrength = saturate(1 - PL_LightDir_Length / PL_RGBLightColor_ALightRange.a);

	float3 PointLight = saturate(dot(-PL_LightDir / PL_LightDir_Length, _Nrm)) * saturate(PL_RGBLightColor_ALightRange.rgb) * PL_LightStrength* PL_LightStrength * PL_LightStrength;

		// Specaluar Anteil
		float3 PointLight_Spec = BlinnPhongShading(PL_LightDir / PL_LightDir_Length, _Nrm, PointToCamera, PL_RGBLightColor_ALightRange, R_SpecRoughness_G_SpecIntensity.g  * _SpecValues.r, R_SpecRoughness_G_SpecIntensity.r  * _SpecValues.g);

		// -- Ambient Light --
		float3 AmbientLight = float3(1, 1, 1);

		// Composition
		float3 LightIntensity = (DirectionalLight + PointLight)* 0.7 + AmbientLight * 0.3 + DirLight_Spec + PointLight_Spec;



		//return float4(_Nrm.xyz / 2 + 0.5, 1);

		// Final Result
		return float4(_Tex.rgb * LightIntensity.rgb, _Tex.a);
}