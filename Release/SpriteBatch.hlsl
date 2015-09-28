struct VertexShaderOut
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
	float2 uv : TEXCOORD0;
};

VertexShaderOut VShader(float3 pos : POSITION0, float4 col : COLOR0, float2 uv : TEXCOORD0)
{
	VertexShaderOut _Out;

	_Out.pos = float4(pos, 1);
	_Out.col = col;
	_Out.uv = uv;

	return _Out;
}

Texture2D gTexture;
SamplerState gSampler;

float4 PShader(VertexShaderOut _In) : SV_TARGET
{
	return gTexture.Sample(gSampler, _In.uv) * _In.col;
}

