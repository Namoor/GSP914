


struct VSIn
{
	float3 Pos : POSITION0;
	float2 PosInQuad : POSITION1;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
};

struct VSOut
{
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float2 UV : TEXCOORD0;
};

cbuffer FrameData
{
	float4x4 MVPMatrix;
	float4 Right;
	float4 Up;
};

Texture2D Tex0;
SamplerState gSampler;

VSOut VShader(VSIn In)
{
	VSOut _Out;

//	float3 ViewDir = normalize(CameraPosition.rgb - In.Pos);

//	float3 Right =  normalize(cross(ViewDir, float3(0,1,0)));
//	float3 Up = normalize(cross(Right, ViewDir));




	_Out.Pos = mul(MVPMatrix, float4(In.Pos + Right * In.PosInQuad.x + Up * In.PosInQuad.y,1));

	_Out.UV = In.UV;
	_Out.Color = In.Color;

	return _Out;
}

float4 PShader(VSOut In) : SV_TARGET
{
	return Tex0.Sample(gSampler, In.UV) * In.Color;
}