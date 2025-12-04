#pragma once
#include <cassert>
#include "DirectXMath.h"

namespace math
{
	template<class T>
	class vector4
	{
	public:
		T x;
		T y;
		T z;
		T w;

		vector4<T>();
		vector4<T>(T aX, T aY, T aZ, T aW);
		~vector4<T>() = default;

		T Length() const;
		T LengthSqr() const;
		T Dot(const vector4<T>& aOtherVector) const;
		void Normalize();
		vector4<T> GetNormalized() const;
		vector3<T> Rotate(const vector3<T>& aOtherVector) const;
		vector3<T> ToEuler() const;
		static vector4<T> RotateAngleAxis(const float aAngle, const vector3<T>& aAxis);
		static vector4<T> FromToRotation(const vector3<T>& aFromDir, const vector3<T>& aToDir);
		static vector4<T> LookRotation(const vector3<T>& aForward, const vector3<T>& aUp);

#pragma region StaticMethods
		static constexpr vector4<T> zero()
		{
			return { 0, 0, 0, 0 };
		}

		static constexpr vector4<T> one()
		{
			return { 1, 1, 1, 1 };
		}

		static constexpr vector4<T> identity()
		{
			return { 0, 0, 0, 1 };
		}
#pragma endregion

		vector4<T>& operator=(const vector4<T>& aOtherVector) = default;
	};

#pragma region Constructors
	template<class T>
	vector4<T>::vector4()
	{
		x = {};
		y = {};
		z = {};
		w = {};
	}

	template<class T>
	vector4<T>::vector4(T aX, T aY, T aZ, T aW)
	{
		x = aX;
		y = aY;
		z = aZ;
		w = aW;
	}
#pragma endregion

#pragma region Operators
	template <class T>
	vector4<T> operator+(const vector4<T>& aVector1, const vector4<T>& aVector2)
	{
		return vector4<T>(aVector1.x + aVector2.x, aVector1.y + aVector2.y, aVector1.z + aVector2.z, aVector1.w + aVector2.w);
	}

	template<class T>
	vector4<T> operator-(const vector4<T>& aVector1, const vector4<T>& aVector2)
	{
		return vector4<T>(aVector1.x - aVector2.x, aVector1.y - aVector2.y, aVector1.z - aVector2.z, aVector1.w - aVector2.w);
	}

	template<class T>
	vector4<T> operator*(const vector4<T>& aVector, const T& aScalar)
	{
		return vector4<T>(aVector.x * aScalar, aVector.y * aScalar, aVector.z * aScalar, aVector.w * aScalar);
	}

	template<class T>
	vector4<T> operator/(const vector4<T>& aVector, const T& aScalar)
	{
		assert(aScalar > 0 && "Can't divide vector with 0.");
		T multiplicationValue = 1 / aScalar;
		return vector4<T>(aVector.x * multiplicationValue, aVector.y * multiplicationValue, aVector.z * multiplicationValue, aVector.w * multiplicationValue);
	}

	template<class T>
	void operator+=(vector4<T>& aVector1, const vector4<T>& aVector2)
	{
		aVector1.x = aVector1.x + aVector2.x;
		aVector1.y = aVector1.y + aVector2.y;
		aVector1.z = aVector1.z + aVector2.z;
		aVector1.w = aVector1.w + aVector2.w;
	}

	template<class T>
	void operator-=(vector4<T>& aVector1, const vector4<T>& aVector2)
	{
		aVector1.x = aVector1.x - aVector2.x;
		aVector1.y = aVector1.y - aVector2.y;
		aVector1.z = aVector1.z - aVector2.z;
		aVector1.w = aVector1.w - aVector2.w;
	}

	template<class T>
	void operator*=(vector4<T>& aVector, const T& aScalar)
	{
		aVector.x = aVector.x * aScalar;
		aVector.y = aVector.y * aScalar;
		aVector.z = aVector.z * aScalar;
		aVector.w = aVector.w * aScalar;
	}

	template<class T>
	void operator/=(vector4<T>& aVector, const T& aScalar)
	{
		assert(aScalar > 0 && "Can't divide vector with 0.");
		T multiplicationValue = 1 / aScalar;
		aVector.x = aVector.x * multiplicationValue;
		aVector.y = aVector.y * multiplicationValue;
		aVector.z = aVector.z * multiplicationValue;
		aVector.w = aVector.w * multiplicationValue;
	}

	template<class T>
	bool operator==(const vector4<T>& aVector1, const vector4<T>& aVector2)
	{
		return (aVector1.x == aVector2.x && aVector1.y == aVector2.y && aVector1.z == aVector2.z && aVector1.w == aVector2.w);
	}

	template<class T>
	bool operator!=(const vector4<T>& aVector1, const vector4<T>& aVector2)
	{
		return (aVector1.x != aVector2.x || aVector1.y != aVector2.y || aVector1.z != aVector2.z || aVector1.w != aVector2.w);
	}
#pragma endregion

#pragma region Methods
	template<class T>
	T vector4<T>::Length() const
	{
		return sqrt(x * x + y * y + z * z + w * w);
	}

	template<class T>
	T vector4<T>::LengthSqr() const
	{
		return (x * x + y * y + z * z + w * w);
	}

	template<class T>
	T vector4<T>::Dot(const vector4<T>& aOtherVector) const
	{
		return (x * aOtherVector.x + y * aOtherVector.y + z * aOtherVector.z + w * aOtherVector.w);
	}

	template<class T>
	void vector4<T>::Normalize()
	{
		assert(Length() > 0 && "Can't Normalize vector with Length 0.");
		T multiplicationValue = 1 / Length();
		x *= multiplicationValue;
		y *= multiplicationValue;
		z *= multiplicationValue;
		w *= multiplicationValue;
	}

	template<class T>
	vector4<T> vector4<T>::GetNormalized() const
	{
		assert(Length() > 0 && "Can't Normalize vector with Length 0.");
		T multiplicationValue = 1 / Length();
		return vector4<T>(x * multiplicationValue, y * multiplicationValue, z * multiplicationValue, w * multiplicationValue);
	}

	template<class T>
	inline vector3<T> vector4<T>::Rotate(const vector3<T>& aOtherVector) const
	{
		const vector3<T> u = { x, y, z };
		return (u * (2 * u.Dot(aOtherVector))) + (aOtherVector * (w * w - u.Dot(u))) + (u.Cross(aOtherVector) * (2 * w));
		//return aOtherVector + (u.Cross(aOtherVector) * w) + u.Cross(u.Cross(aOtherVector)) * 2.0f; // Doesnt work
	}

	template<class T>
	vector3<T> vector4<T>::ToEuler() const
	{
		const float xx = x * x;
		const float yy = y * y;
		const float zz = z * z;

		const float m31 = 2.f * x * z + 2.f * y * w;
		const float m32 = 2.f * y * z - 2.f * x * w;
		const float m33 = 1.f - 2.f * xx - 2.f * yy;

		const float cy = sqrtf(m33 * m33 + m31 * m31);
		const float cx = atan2f(-m32, cy);
		if (cy > 16.f * FLT_EPSILON)
		{
			const float m12 = 2.f * x * y + 2.f * z * w;
			const float m22 = 1.f - 2.f * xx - 2.f * zz;

			return { cx, atan2f(m31, m33), atan2f(m12, m22) };
		}
		else
		{
			const float m11 = 1.f - 2.f * yy - 2.f * zz;
			const float m21 = 2.f * x * y - 2.f * z * w;

			return { cx, 0.f, atan2f(-m21, m11) };
		}
	}

	template<class T>
	inline vector4<T> vector4<T>::RotateAngleAxis(const float aAngle, const vector3<T>& aAxis)
	{
		const float sn = sin(aAngle * 0.5f);
		const float cs = cos(aAngle * 0.5f);
		const math::vector3<T> asn = aAxis * sn;
		return { asn.x, asn.y, asn.z, cs };
	}

	template<class T>
	vector4<T> vector4<T>::FromToRotation(const vector3<T>& aFromDir, const vector3<T>& aToDir)
	{
		const vector3<T> normalizedFromDir = aFromDir.GetNormalized();
		const vector3<T> normalizedToDir = aToDir.GetNormalized();

		const float dot = normalizedFromDir.Dot(normalizedToDir);

		if (dot >= 1.f)
		{
			return vector4<T>::identity();
		}
		else if (dot <= -1.f)
		{
			vector3<T> axis = normalizedFromDir.Cross(vector3<T>::right());
			if (axis.x <= FLT_EPSILON && axis.x >= -FLT_EPSILON && axis.y <= FLT_EPSILON && axis.y >= -FLT_EPSILON && axis.z <= FLT_EPSILON && axis.z >= -FLT_EPSILON)
			{
				axis = normalizedFromDir.Cross(vector3<T>::up());
			}

			return vector4<T>::RotateAngleAxis(DirectX::XM_PI, axis);
			//const DirectX::XMVECTOR quat = DirectX::XMQuaternionRotationAxis(DirectX::XMVectorSet(axis.x, axis.y, axis.z, 0), DirectX::XM_PI);
			//return vector4<T>(DirectX::XMVectorGetX(quat), DirectX::XMVectorGetY(quat), DirectX::XMVectorGetZ(quat), DirectX::XMVectorGetW(quat));
		}
		else
		{
			vector3<T> cross = normalizedFromDir.Cross(normalizedToDir);
			const float s = sqrtf((1.f + dot) * 2.f);
			cross /= s;
			return vector4<T>(cross.x, cross.y, cross.z, s * 0.5f);
		}
	}

	template<class T>
	vector4<T> vector4<T>::LookRotation(const vector3<T>& aForward, const vector3<T>& aUp)
	{
		vector4<T> quat = vector4<T>::FromToRotation(vector3<T>::forward(), aForward);

		vector3<T> cross = aForward.Cross(aUp);
		if (cross.x <= FLT_EPSILON && cross.x >= -FLT_EPSILON && cross.y <= FLT_EPSILON && cross.y >= -FLT_EPSILON && cross.z <= FLT_EPSILON && cross.z >= -FLT_EPSILON)
		{
			return quat;
		}

		const DirectX::XMVECTOR U = DirectX::XMQuaternionMultiply(DirectX::XMVectorSet(quat.x, quat.y, quat.z, quat.w), DirectX::XMVectorSet(0, 1, 0, 0));
		
		vector4<T> quat2 = FromToRotation({ DirectX::XMVectorGetX(U), DirectX::XMVectorGetY(U), DirectX::XMVectorGetZ(U) }, aUp);

		const DirectX::XMVECTOR result = DirectX::XMQuaternionMultiply(DirectX::XMVectorSet(quat2.x, quat2.y, quat2.z, quat2.w), DirectX::XMVectorSet(quat.x, quat.y, quat.z, quat.w));
		return vector4<T>(DirectX::XMVectorGetX(result), DirectX::XMVectorGetY(result), DirectX::XMVectorGetZ(result), DirectX::XMVectorGetW(result));
	}
#pragma endregion
}