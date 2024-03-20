#pragma once
#include <DirectXMath.h>
#include "DirectXTex/DirectXTex.h"
#include "Tools/Input.h"
#include "Math/vectors.h"

using namespace DirectX;

class Camera
{
public:
	explicit Camera() = default;
	virtual ~Camera() = default;

	virtual void Update() = 0;
	virtual math::vector3<float> GetPosition() = 0;
	virtual math::vector4<float> GetRotation() = 0;
	virtual void SetTransformation(math::vector3<float> aPosition, math::vector4<float> aRotation) = 0;
	virtual XMMATRIX GetMatrix() = 0;

#pragma region Methods
	math::vector2<float> WorldSpaceToCameraSpace(math::vector3<float> aPosition)
	{
		XMMATRIX matrix =
			GetMatrix() * // View Matrix
			XMMatrixPerspectiveFovLH(1.0f, 16.0f / 9.0f, 0.1f, 1000.0f); // Projection Matrix

		XMVECTOR newVector = XMVector3TransformCoord(XMVectorSet(aPosition.x, aPosition.y, aPosition.z, 0), matrix);

		return { XMVectorGetX(newVector), XMVectorGetY(newVector) };
	}

	math::vector3<float> CameraSpaceToWorldSpace(math::vector2<float> aPosition)
	{
		XMMATRIX matrix =
			GetMatrix() * // View Matrix
			XMMatrixPerspectiveFovLH(1.0f, 16.0f / 9.0f, 0.1f, 1000.0f); // Projection Matrix

		matrix = XMMatrixInverse(NULL, matrix); // Inverse Matrix

		XMVECTOR newVector = XMVector3TransformCoord(XMVectorSet(aPosition.x, aPosition.y, 0, 0), matrix);

		return { XMVectorGetX(newVector), XMVectorGetY(newVector), XMVectorGetZ(newVector) };
	}

	static math::vector2<float> MousePositionToCameraSpace(HWND& hwnd)
	{
		POINT mPos = Input::GetInstance().GetMousePosition();

		math::vector2<float> mousePos = math::vector2<float>(mPos.x, mPos.y);

		RECT rect;
		GetWindowRect(hwnd, &rect);
		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		mousePos.x /= width;
		mousePos.y /= height;

		mousePos.x -= 0.5f;
		mousePos.y -= 0.5f;

		mousePos.x *= 2;
		mousePos.y *= -2;

		return mousePos;
	}
#pragma endregion

protected:
	virtual void CalculateMatrix() = 0;
	virtual void UpdateInput() { }

	XMMATRIX myCameraMatrix;
};
