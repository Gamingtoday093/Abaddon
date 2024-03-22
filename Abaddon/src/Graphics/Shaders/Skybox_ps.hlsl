TextureCube tex : register(t0);
SamplerState sam : register(s0);

struct PS_INPUT
{
    float3 worldPos : Position;
    float4 pos : SV_Position;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return tex.Sample(sam, input.worldPos);
}