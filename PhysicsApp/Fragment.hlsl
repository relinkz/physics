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
};

float4 PS_main(VS_OUT input) : SV_Target
{
	pointLight light;

	light.Pos = float3(0.0f, 4.0f, 0.0f);
	light.Color = float3(1.0f, 1.0f, 1.0f);

	float3 color = float3(1, 0, 0);


	float4 lightVector = normalize(float4(light.Pos, 1)- input.worldPos);
	float angle = dot(input.Normal, lightVector);
	angle = saturate(angle);

	color = color * angle;


	return float4(color, 0);
};