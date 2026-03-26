struct VS_INPUT 
{
	float3 position : POSITION;
	float4 color : COLOR;
};

struct VS_OUTPUT 
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

cbuffer projectionBuffer: register(b0) 
{
    float4x4 myProjectionViewMatrix;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
    output.position = mul(myProjectionViewMatrix, float4(input.position, 1.f));
    output.color = input.color;
	
	return output;
}