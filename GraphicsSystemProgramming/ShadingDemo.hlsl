struct VertexShaderOut
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
};

cbuffer Matrix
{
	float4x4 WVPMatrix;
};

cbuffer LightData
{
	float4 LightDir;

	float4 RGBLightColor_ALightIntensity;
};


VertexShaderOut VShader(float3 pos : POSITION0, float2 uv : TEXCOORD0, float3 Normal : NORMAL0)
{
	VertexShaderOut _Out;

	_Out.pos = mul(WVPMatrix, float4(pos, 1));
	_Out.uv = uv;
	_Out.normal = Normal;

	return _Out;
}

Texture2D gTexture;
SamplerState gSampler;

float4 PShader(VertexShaderOut _In) : SV_TARGET
{
	// _In.normal: N

	float3 LightIntensity = saturate(dot(-LightDir, _In.normal)) * RGBLightColor_ALightIntensity.rgb; // * C * I

	float3 AmbientLight = float3(1, 1, 1);


	LightIntensity = LightIntensity * 0.7 + AmbientLight * 0.3;


	float4 _Tex = gTexture.Sample(gSampler, _In.uv);

		return float4(_Tex.rgb * LightIntensity.rgb, _Tex.a);
}