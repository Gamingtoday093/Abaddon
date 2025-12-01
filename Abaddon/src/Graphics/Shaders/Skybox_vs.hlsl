cbuffer TransformCBuf : register(b0)
{
    matrix viewProj; // float4x4
};

struct VS_INPUT
{
    float4 position : POSITION;
    float4 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct VSOut
{
    float3 worldPosition : Position;
    float4 pixelPosition : SV_Position;
};

VSOut main(VS_INPUT input)
{
    VSOut vso;
    vso.worldPosition = float3(input.position.xyz);
    input.position.w = 0;
    vso.pixelPosition = mul(input.position, viewProj);
    // make sure that the depth after w divide will be 1.0 (so that the z-buffering will work)
    vso.pixelPosition.z = vso.pixelPosition.w;
    return vso;
}