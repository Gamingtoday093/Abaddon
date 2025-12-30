struct VS_INPUT 
{
	float3 position : POSITION;
    float3 normal : NORMAL;
	float2 texcoord : TEXCOORD;
	
    uint4 boneIDs : BONEIDS;
    float4 boneWeights : BONEWEIGHTS;
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
    float4x4 myProjectionViewMatrix;
    float4x4 myModelMatrix;
};

StructuredBuffer<float4x4> myAnimation : register(t0);

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
    
    float4x4 skinningMatrix = 0;
    skinningMatrix += mul(myAnimation[input.boneIDs.x], input.boneWeights.x);
    skinningMatrix += mul(myAnimation[input.boneIDs.y], input.boneWeights.y);
    skinningMatrix += mul(myAnimation[input.boneIDs.z], input.boneWeights.z);
    skinningMatrix += mul(myAnimation[input.boneIDs.w], input.boneWeights.w);
    
    output.worldPosition = mul(myModelMatrix, mul(skinningMatrix, float4(input.position, 1.f)));
    output.position = mul(myProjectionViewMatrix, output.worldPosition);
    output.normal = normalize(mul(myModelMatrix, float4(input.normal, 0.f)));
    output.texcoord = input.texcoord;
    
	return output;
}