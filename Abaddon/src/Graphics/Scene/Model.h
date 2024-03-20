#pragma once
#include "Graphics/Vertex.h"
#include "Graphics/AABB.h"
#include "Graphics/Bindables/Bindables.h"

#include "Camera.h"
#include "Math/vector3.hpp"

struct ModelData
{
	// Mesh
	VertexBuffer myVertexBuffer;
	IndexBuffer myIndexBuffer;
	InputLayout myInputLayout;

	AABB myAABB;

	// Texture
	SRV mySRV;
	Sampler mySampler;

	// CBuffers
	CBuffer<TransformBuffer> myTransformCBuffer;
};

class Model
{
public:
	Model() = default;
	~Model() = default;

	void LoadModel(std::string aFilePath, std::string aTextureName, std::shared_ptr<Camera> aCamera);
	void UpdateRender(std::shared_ptr<Camera> aCamera);
	const math::vector3<float>& GetPosition();
	const math::vector3<float>& GetRotation();
	void Transform(math::vector3<float> aPosition, math::vector3<float> aRotation);
	void SetScale(math::vector3<float> aScale);
	void SetPosition(math::vector3<float> aPosition);
	void SetRotation(math::vector3<float> aRotation);
	ModelData& GetModelData();
	AABB GetTransformedAABB();

	virtual void Awake() {}
	virtual void Update() {}

private:
	ModelData myModelData;

protected:
	math::vector3<float> myPosition;
	XMVECTOR myQuaternionRotation = XMVectorSet(0, 0, 0, 1);
	math::vector3<float> myScale = math::vector3<float>::one();
};

