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
	float4 RGB_CameraPosition_ARoughness;

	// DirectionalLight
	float4 DL_LightDir;
	float4 DL_RGBLightColor;

	// Point Light
	float4 PL_LightPos;
	float4 PL_RGBLightColor_ALightRange;

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



Texture2D gTexture;
SamplerState gSampler;

float3 PhongShading(float3 LightInDir, float3 Normal, float3 ViewOutDir, float3 SpecularColor, float SpecularIntensity)
{
	float3 OutDir = LightInDir - 2 * Normal * dot(Normal, LightInDir);
		// float3 OutDir = reflect(LightInDir, Normal);

		float SpecPower = saturate(dot(OutDir, ViewOutDir));

	SpecPower = pow(SpecPower, RGB_CameraPosition_ARoughness.a);

	return SpecularColor * SpecPower * SpecularIntensity;
}

float4 PShader(VertexShaderOut _In) : SV_TARGET
{
	_In.normal = normalize(_In.normal);

	float3 PointToCamera = normalize(RGB_CameraPosition_ARoughness.rgb - _In.worldPos.xyz);

	// Texture
	float4 _Tex = gTexture.Sample(gSampler, _In.uv);



	// -- Directional Light --

	// Diffuse Anteil
	float3 DirectionalLight = saturate(dot(-DL_LightDir, _In.normal)) * DL_RGBLightColor.rgb; // * C * I

		// Specular Anteil
		float3 DirLight_Spec = PhongShading(DL_LightDir, _In.normal, PointToCamera, DL_RGBLightColor.rgb, _Tex.r);

		// -- Point Light --

		// Diffuse Anteil
		float3 PL_LightDir = _In.worldPos.xyz - PL_LightPos.xyz;
		float PL_LightDir_Length = length(PL_LightDir);

	float PL_LightStrength = saturate(1 - PL_LightDir_Length / PL_RGBLightColor_ALightRange.a);

	float3 PointLight = saturate(dot(-PL_LightDir / PL_LightDir_Length, _In.normal)) * saturate(PL_RGBLightColor_ALightRange.rgb) * PL_LightStrength* PL_LightStrength * PL_LightStrength;

		// Specaluar Anteil
		float3 PointLight_Spec = PhongShading(PL_LightDir / PL_LightDir_Length, _In.normal, PointToCamera, PL_RGBLightColor_ALightRange, _Tex.r);

		// -- Ambient Light --
		float3 AmbientLight = float3(1, 1, 1);

		// Composition
		float3 LightIntensity = (DirectionalLight + PointLight)* 0.7 + AmbientLight * 0.3 + DirLight_Spec + PointLight_Spec;



		// Final Result
		return float4(_Tex.rgb * LightIntensity.rgb, _Tex.a);
}