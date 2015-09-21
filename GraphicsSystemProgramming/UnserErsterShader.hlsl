float4 VShader( float3 pos : POSITION0 ) : SV_POSITION
{
	return float4(pos, 1);
}


float4 PShader(float4 pos : SV_POSITION) : SV_TARGET
{
	return float4(1, 1, 1, 1);
}

