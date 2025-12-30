#pragma once
#include "AnimationDefines.h"

struct Animation
{
	struct Keyframe
	{
		Keyframe()
		{
			myPositionKey = math::vector3<float>::zero();
			myRotationKey = math::vector4<float>::identity();
			myScaleKey = math::vector3<float>::one();
		}

		DirectX::XMMATRIX GetMatrix() const
		{
			return DirectX::XMMatrixTranspose(
				DirectX::XMMatrixScaling(myScaleKey.x, myScaleKey.y, myScaleKey.z) *
				DirectX::XMMatrixRotationQuaternion(DirectX::XMVectorSet(myRotationKey.x, myRotationKey.y, myRotationKey.z, myRotationKey.w)) *
				DirectX::XMMatrixTranslation(myPositionKey.x, myPositionKey.y, myPositionKey.z)
			);
		}

		math::vector3<float> myPositionKey;
		math::vector4<float> myRotationKey;
		math::vector3<float> myScaleKey;
	};

	struct PositionKeyframe
	{
		PositionKeyframe(double aTime, float aX, float aY, float aZ)
		{
			myTime = aTime;
			myPositionKey = { aX, aY, aZ };
		}

		double myTime;
		math::vector3<float> myPositionKey;
	};

	struct RotationKeyframe
	{
		RotationKeyframe(double aTime, float aX, float aY, float aZ, float aW)
		{
			myTime = aTime;
			myRotationKey = { aX, aY, aZ, aW };
		}

		double myTime;
		math::vector4<float> myRotationKey = math::vector4<float>::identity();
	};

	struct ScaleKeyframe
	{
		ScaleKeyframe(double aTime, float aX, float aY, float aZ)
		{
			myTime = aTime;
			myScaleKey = { aX, aY, aZ };
		}

		double myTime;
		math::vector3<float> myScaleKey = math::vector3<float>::one();
	};

	struct Channel
	{
		Channel(const std::string& aName)
		{
			myName = aName;
		}

		std::string myName;

		std::vector<PositionKeyframe> myPositionKeyframes{};
		std::vector<RotationKeyframe> myRotationKeyframes{};
		std::vector<ScaleKeyframe> myScaleKeyframes{};

		/// <summary>
		/// Binary searches through <see cref="aKeyframes" /> for the exact or closest lowest time Keyframe. 
		/// Example:
		/// <code>
		/// (1.8) => { 0.5, 2.25, 4.5 } = 0.5
		/// </code>
		/// </summary>
		/// <param name="aTime">Target time</param>
		/// <returns>Index of the exact or closest lowest time Keyframe</returns>
		template<typename T>
		size_t BinarySearchKeyframe(double aTime, const std::vector<T>& aKeyframes) const
		{
			size_t depth = 2;
			size_t pivot = aKeyframes.size() / depth;
			while (pivot > 0 && pivot < aKeyframes.size())
			{
				depth *= 2;
				double pivotTime = aKeyframes[pivot].myTime;
				if (pivotTime == aTime || 
					(pivotTime < aTime && aTime < aKeyframes[pivot + 1].myTime)) return pivot;

				size_t offset = aKeyframes.size() / depth;
				if (offset == 0) offset = 1;
				if (aTime < pivotTime)
					pivot -= offset;
				else
					pivot += offset;
			}

			return pivot;
		}

		Keyframe GetInterpolated(double aTime) const
		{
			Keyframe keyframe;

			if (myPositionKeyframes.size() > 0)
				SetPositionKey(aTime, keyframe);

			if (myRotationKeyframes.size() > 0)
				SetRotationKey(aTime, keyframe);

			if (myScaleKeyframes.size() > 0)
				SetScaleKey(aTime, keyframe);

			return keyframe;
		}

		void SetPositionKey(double aTime, Keyframe& aKeyframe) const
		{
			size_t positionIndex = BinarySearchKeyframe(aTime, myPositionKeyframes);
			if (positionIndex == myPositionKeyframes.size() - 1)
			{
				aKeyframe.myPositionKey = myPositionKeyframes.at(positionIndex).myPositionKey;
				return;
			}

			const PositionKeyframe& positionKeyframe = myPositionKeyframes.at(positionIndex);
			if (positionKeyframe.myTime == aTime)
			{
				aKeyframe.myPositionKey = positionKeyframe.myPositionKey;
				return;
			}

			const PositionKeyframe& nextPositionKeyframe = myPositionKeyframes.at(positionIndex + 1);
			float t = float((aTime - positionKeyframe.myTime) / (nextPositionKeyframe.myTime - positionKeyframe.myTime));
			aKeyframe.myPositionKey = math::vector3<float>::lerp(positionKeyframe.myPositionKey, nextPositionKeyframe.myPositionKey, t);
		}

		void SetRotationKey(double aTime, Keyframe& aKeyframe) const
		{
			size_t rotationIndex = BinarySearchKeyframe(aTime, myRotationKeyframes);
			if (rotationIndex == myRotationKeyframes.size() - 1)
			{
				aKeyframe.myRotationKey = myRotationKeyframes.at(rotationIndex).myRotationKey;
				return;
			}

			const RotationKeyframe& rotationKeyframe = myRotationKeyframes.at(rotationIndex);
			if (rotationKeyframe.myTime == aTime)
			{
				aKeyframe.myRotationKey = rotationKeyframe.myRotationKey;
				return;
			}

			const RotationKeyframe& nextRotationKeyframe = myRotationKeyframes.at(rotationIndex + 1);
			float t = float((aTime - rotationKeyframe.myTime) / (nextRotationKeyframe.myTime - rotationKeyframe.myTime));
			aKeyframe.myRotationKey = math::vector4<float>::slerp(rotationKeyframe.myRotationKey, nextRotationKeyframe.myRotationKey, t);
		}

		void SetScaleKey(double aTime, Keyframe& aKeyframe) const
		{
			size_t scaleIndex = BinarySearchKeyframe(aTime, myScaleKeyframes);
			if (scaleIndex == myScaleKeyframes.size() - 1)
			{
				aKeyframe.myScaleKey = myScaleKeyframes.at(scaleIndex).myScaleKey;
				return;
			}

			const ScaleKeyframe& scaleKeyframe = myScaleKeyframes.at(scaleIndex);
			if (scaleKeyframe.myTime == aTime)
			{
				aKeyframe.myScaleKey = scaleKeyframe.myScaleKey;
				return;
			}

			const ScaleKeyframe& nextScaleKeyframe = myScaleKeyframes.at(scaleIndex + 1);
			float t = float((aTime - scaleKeyframe.myTime) / (nextScaleKeyframe.myTime - scaleKeyframe.myTime));
			aKeyframe.myScaleKey = math::vector3<float>::lerp(scaleKeyframe.myScaleKey, nextScaleKeyframe.myScaleKey, t);
		}
	};

	double myFramerate{};
	double myDurationSeconds{};
	std::vector<Channel> myChannels{};
};