cbuffer TransformCBuf : register(b0)
{
    matrix viewProj; // float4x4
};

struct VS_INPUT
{
    float4 position : POSITION;
    float2 texcoord : TEXCOORD;
};

struct VSOut
{
    float3 worldPos : Position;
    float4 pos : SV_Position;
};

VSOut main(VS_INPUT input)
{
    VSOut vso;
    vso.worldPos = float3(input.position.xyz);
    input.position.w = 0;
    vso.pos = mul(input.position, viewProj);
    // make sure that the depth after w divide will be 1.0 (so that the z-buffering will work)
    vso.pos.z = vso.pos.w;
    return vso;
}