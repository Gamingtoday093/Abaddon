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
    int circlePositionsCount;
    float4 color;
    float4 circlePositions[128];
};

float4 main(PS_INPUT input) : SV_TARGET
{    
    for (int i = 0; i < circlePositionsCount; i++)
    {
        float dist = distance(input.worldPosition, circlePositions[i]);
        if (dist > 13)
        {
            for (int j = 0; j < circlePositionsCount; j++)
            {
                if (i == j) continue;
                
                dist = distance(input.worldPosition, circlePositions[j]);
                if (dist <= 13)
                    return float4(0, 0, 0, 0);
            }
            
            return color;
        }
    }
    
    return float4(0, 0, 0, 0);
}