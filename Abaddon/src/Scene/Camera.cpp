#include "pch.h"
#include "Camera.h"

math::vector2<float> Camera::WorldSpaceToCameraSpace(math::vector3<float> aPosition) const
{
	XMMATRIX matrix =
		GetMatrix() * // View Matrix
		XMMatrixPerspectiveFovLH(1.0f, 16.0f / 9.0f, 0.1f, 1000.0f); // Projection Matrix

	XMVECTOR newVector = XMVector3TransformCoord(XMLoadFloat3(reinterpret_cast<XMFLOAT3*>(&aPosition)), matrix);

	return { XMVectorGetX(newVector), XMVectorGetY(newVector) };
}

math::vector3<float> Camera::CameraSpaceToWorldSpace(math::vector2<float> aPosition) const
{
	XMMATRIX matrix =
		GetMatrix() * // View Matrix
		XMMatrixPerspectiveFovLH(1.0f, 16.0f / 9.0f, 0.1f, 1000.0f); // Projection Matrix

	matrix = XMMatrixInverse(NULL, matrix); // Inverse Matrix

	XMVECTOR newVector = XMVector3TransformCoord(XMLoadFloat2(reinterpret_cast<XMFLOAT2*>(&aPosition)), matrix);

	return { XMVectorGetX(newVector), XMVectorGetY(newVector), XMVectorGetZ(newVector) };
}

/// <summary>
/// Center of the Screen is (0, 0).
/// Left is -1 and Right is 1.
/// Bottom is -1 and Top is 1.
/// Top Right is (1, 1).
/// Bottom Left is (-1, -1).
/// </summary>
/// <param name="hwnd"></param>
/// <returns></returns>
math::vector2<float> Camera::MousePositionToCameraSpace(HWND hwnd)
{
	POINT mPos = Input::GetInstance().GetMousePosition();
	math::vector2<float> mousePos = math::vector2<float>(float(mPos.x), float(mPos.y));

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
