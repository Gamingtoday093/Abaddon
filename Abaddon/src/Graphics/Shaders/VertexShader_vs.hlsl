struct VS_INPUT 
{
	float4 position : POSITION;
    float4 normal : NORMAL;
	float2 texcoord : TEXCOORD;
};

struct VS_OUTPUT 
{
	float4 position : SV_POSITION;
	float4 worldPosition : POSITION;
	float4 normal : NORMAL;
	float2 texcoord : TEXCOORD;
};

cbuffer transformBuffer: register(b0) 
{
    float4x4 myProjectionModelMatrix;
    float4x4 myModelMatrix;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
    output.position = mul(input.position, myProjectionModelMatrix);
    output.worldPosition = mul(input.position, myModelMatrix);
    output.normal = mul(input.normal, myModelMatrix);
    output.texcoord = input.texcoord;

	return output;
}