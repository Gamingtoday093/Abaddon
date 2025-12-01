struct PS_INPUT 
{
	float4 position : SV_POSITION;
    float4 worldPosition : POSITION;
    float4 normal : NORMAL;
	float2 texcoord : TEXCOORD;
};

cbuffer CameraBuffer : register(b0)
{
    float4 cameraPosition;
};

cbuffer MaterialBuffer : register(b1)
{
    float4 color;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return color;
}