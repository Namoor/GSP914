struct VertexShaderOut
{
	float4 pos : SV_POSITION;
	float2 uv0 : TEXCOORD0;
	float2 uv1 : TEXCOORD1;
	float2 uv2 : TEXCOORD2;
	float2 uv3 : TEXCOORD3;
};

cbuffer Matrix
{
	float4x4 WVPMatrix;
};

VertexShaderOut VShader(float3 pos : POSITION0, float2 uv0 : TEXCOORD0, float2 uv1 : TEXCOORD1, float2 uv2 : TEXCOORD2, float2 uv3 : TEXCOORD3)
{
	VertexShaderOut _Out;

	_Out.pos = mul(WVPMatrix, float4(pos, 1));
	_Out.uv0 = uv0;
	_Out.uv1 = uv1;
	_Out.uv2 = uv2;
	_Out.uv3 = uv3;

	return _Out;
}

Texture2D gTexture;
SamplerState gSampler;

float4 PShader(VertexShaderOut _In) : SV_TARGET
{
	float4 _Tex0 = gTexture.Sample(gSampler, _In.uv0);
	float4 _Tex1 = gTexture.Sample(gSampler, _In.uv1);
	float4 _Tex2 = gTexture.Sample(gSampler, _In.uv2);
	float4 _Tex3 = gTexture.Sample(gSampler, _In.uv3);


	float3 _TexColor = _Tex0 * _Tex0.a;
	float RemainingAlpha = 1 - _Tex0.a;
	_TexColor += RemainingAlpha * _Tex1.rgb * _Tex1.a;
	RemainingAlpha *= 1 - _Tex1.a;
	_TexColor += RemainingAlpha * _Tex2.rgb * _Tex2.a;
	RemainingAlpha *= 1 - _Tex2.a;
	_TexColor += RemainingAlpha * _Tex3.rgb * _Tex3.a;


	return float4(_TexColor, 1);
}