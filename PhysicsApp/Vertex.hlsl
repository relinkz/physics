cbuffer matrixData
{
	float4x4 worldMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};

struct VS_IN
{
	float3 Pos		: POSITION;
	float3 Normal	: NORMAL;
	float2 UV		: TEXCOORD;
};

struct VS_OUT
{
	float4 Pos		: SV_POSITION;
	float3 Normal	: NORMAL;
	float2 UV		: TEXCOORD;
};
//-----------------------------------------------------------------------------------------
// VertexShader: VSScene
//-----------------------------------------------------------------------------------------
VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;

	output.Pos = float4(input.Pos, 1);
	output.Normal = input.Normal;
	output.UV = input.UV;

	//transform to world
	output.Pos = mul(output.Pos, worldMatrix);
	//relative to camera
	output.Pos = mul(output.Pos, viewMatrix);
	//add perspective
	output.Pos = mul(output.Pos, projectionMatrix);

	return output;
}