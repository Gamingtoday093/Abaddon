#pragma once
#include <DirectXMath.h>
#include "DirectXTex/DirectXTex.h"
#include "Tools/Input.h"
#include "Math/vectors.h"

using namespace DirectX;

class Camera abstract
{
public:
	explicit Camera() = default;
	virtual ~Camera() = default;

	virtual void Update() = 0;
	virtual math::vector3<float> GetPosition() const = 0;
	virtual math::vector4<float> GetRotation() const = 0;
	virtual void SetTransformation(const math::vector3<float>& aPosition, const math::vector4<float>& aRotation) = 0;
	virtual void Focus(const math::vector3<float>& aPosition) = 0;
	virtual XMMATRIX GetMatrix() const = 0;

#pragma region Methods
	math::vector2<float> WorldSpaceToCameraSpace(math::vector3<float> aPosition) const;
	math::vector3<float> CameraSpaceToWorldSpace(math::vector2<float> aPosition) const;

	/// <summary>
	/// Center of the Screen is (0, 0).
	/// Left is -1 and Right is 1.
	/// Bottom is -1 and Top is 1.
	/// Top Right is (1, 1).
	/// Bottom Left is (-1, -1).
	/// </summary>
	/// <param name="hwnd"></param>
	/// <returns></returns>
	static math::vector2<float> MousePositionToCameraSpace(HWND hwnd);
#pragma endregion

protected:
	virtual void CalculateMatrix() = 0;
	virtual void UpdateInput() { }
};
