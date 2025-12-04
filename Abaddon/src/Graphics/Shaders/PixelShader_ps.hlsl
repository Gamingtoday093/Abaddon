Texture2D Texture : register(t0);
Texture2D Emission : register(t1);
sampler TextureSampler : register(s0);
sampler EmissionSampler : register(s1);

struct PS_INPUT 
{
	float4 position : SV_Position;
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
    float roughness;
};

cbuffer LightingBufffer : register(b2)
{
    float4 lightColor;
    float lightStrength;
    float4 lightDirection;
};

float cal_specular(float4 position, float4 normal, float shininess)
{
    const float4 halfWayVector = normalize(-lightDirection + (cameraPosition - position));
    return saturate(pow(dot(normal, halfWayVector), shininess));
}

float4 main(PS_INPUT input) : SV_TARGET
{
    const float4 baseColor = Texture.Sample(TextureSampler, input.texcoord);
    const float4 emission = Emission.Sample(EmissionSampler, input.texcoord);
    
    const float NdotL = dot(input.normal, -lightDirection);
    float light = saturate(NdotL);
    
    light += 0.5f;
    light *= 0.75f;
    
    return ((baseColor + (cal_specular(input.worldPosition, input.normal, 4) * roughness) + (lightColor * saturate(NdotL) * lightStrength)) * light) + emission;
}