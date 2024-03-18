#pragma once
#include <DirectXMath.h>
#include "DirectXTex/DirectXTex.h"
#include "Tools/Input.h"
#include "Math/vectors.h"

using namespace DirectX;

class Camera
{
public:
	Camera(Input& aInput) : myInput(aInput) { }
	~Camera() = default;

	virtual void Update() = 0;
	virtual XMMATRIX GetMatrix() = 0;

protected:
	virtual void CalculateMatrix() = 0;
	virtual void UpdateInput() { }

	Input& myInput;

	XMMATRIX myCameraMatrix;
};

