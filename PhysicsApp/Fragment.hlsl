Texture2D txDiffuse : register(t0);
SamplerState sampAni : register(s0);

struct VS_OUT
{
	float4 Pos		: SV_POSITION;
	float3 Normal	: NORMAL;
	float2 UV		: TEXCOORD;
	float4 worldPos : POSITION;
};

struct pointLight
{
	float3 Pos;
	float3 Color;
	float range;
};

float4 PS_main(VS_OUT input) : SV_Target
{
	pointLight light;

	light.Pos = float3(0.0f, 0.0f, 0.0f);
	light.Color = float3(1.0f, 1.0f, 1.0f);
	light.range = 1.0f;

	float4 color = float4(txDiffuse.Sample(sampAni, input.UV).xyz, 0.0f);


	float4 lightVector = normalize(float4(light.Pos, 1)- input.worldPos);
	float angle = dot(input.Normal, lightVector);
	angle = saturate(angle);
	if (length(float4(light.Pos, 1) - input.worldPos) < light.range)
	{
		angle = 1;
	}

	color = (color * angle) + color * 0.8f;


	return color;
};