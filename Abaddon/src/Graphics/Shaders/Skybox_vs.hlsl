cbuffer TransformCBuf : register(b0)
{
    matrix viewProj; // float4x4
};

struct VS_INPUT
{
    float3 position : POSITION;
};

struct VSOut
{
    float3 worldPosition : Position;
    float4 pixelPosition : SV_Position;
};

VSOut main(VS_INPUT input)
{
    VSOut vso;
    vso.worldPosition = float3(input.position);
    vso.pixelPosition = mul(viewProj, float4(input.position, 0.f));
    // make sure that the depth after w divide will be 1.0 (so that the z-buffering will work)
    vso.pixelPosition.z = vso.pixelPosition.w;
    return vso;
}