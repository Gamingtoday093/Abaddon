#pragma once
#include "Bindables/CBuffer.hpp"
#include <DirectXMath.h>

__declspec(align(64))
struct TransformBuffer
{
	DirectX::XMMATRIX myProjectionModelMatrix;
	DirectX::XMMATRIX myModelMatrix;
};

__declspec(align(16))
struct CameraBuffer
{
	DirectX::XMVECTOR CameraPosition;
};

__declspec(align(16))
struct MaterialBuffer
{
	DirectX::XMVECTOR Color;
	float Roughness;
};

__declspec(align(16))
struct LightingBuffer
{
	DirectX::XMVECTOR LightColor;
	float LightStrength;
	DirectX::XMVECTOR LightDirection = DirectX::XMVectorSet(0, -1, 0, 0);
};

__declspec(align(16))
struct ColorBuffer
{
	DirectX::XMVECTOR Color;
};

__declspec(align(16))
struct SelectCircleBuffer
{
	int CirclePositionsCount;
	DirectX::XMVECTOR Color;
	DirectX::XMVECTOR CirclePositions[128];
};
