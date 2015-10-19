struct VertexShaderOut
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL0;
};

cbuffer Matrix
{
	float4x4 WVPMatrix;
};

VertexShaderOut VShader(float3 pos : POSITION0, float4 col : COLOR0, float2 uv : TEXCOORD0, float3 Normal : NORMAL0)
{
	VertexShaderOut _Out;

	_Out.pos = mul(WVPMatrix, float4(pos, 1));
	_Out.col = col;
	_Out.uv = uv;
	_Out.normal = Normal;

	return _Out;
}

Texture2D gTexture;
SamplerState gSampler;

float4 PShader(VertexShaderOut _In) : SV_TARGET
{
	float LightIntensity = 0.3 + 0.7 * saturate(dot(_In.normal, normalize(float3(-2, 3, -1))));
	float4 _Tex = gTexture.Sample(gSampler, _In.uv) * _In.col;

		return float4(_Tex.rgb * LightIntensity, _Tex.a);
}