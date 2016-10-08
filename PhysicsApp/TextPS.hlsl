Texture2D txDiffuse : register(t0);
SamplerState sampAni : register(s0);

struct VS_OUT
{
	float4 Pos		: SV_POSITION;
	float2 UV		: TEXCOORD;
};



float4 main(VS_OUT input) : SV_Target
{
	float4 color = float4(0.0f,0.0f,0.0f,0.0f);
	color = float4(txDiffuse.Sample(sampAni, input.UV).xyz, 0.0f);

	return color;
};