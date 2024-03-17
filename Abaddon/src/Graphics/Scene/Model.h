#pragma once
#include "Graphics/Vertex.h"
#include "Graphics/Bindables/Bindables.h"

#include "Camera.h"
#include "Math/vector3.hpp"

struct ModelData
{
	// Mesh
	VertexBuffer myVertexBuffer;
	IndexBuffer myIndexBuffer;
	InputLayout myInputLayout;

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

	void LoadModel(std::string aFilePath, std::shared_ptr<Camera> aCamera);
	void Update(std::shared_ptr<Camera> aCamera);
	const math::vector3<float>& GetPosition();
	const math::vector3<float>& GetRotation();
	void Transform(math::vector3<float> aPosition, math::vector3<float> aRotation);
	ModelData& GetModelData();

private:
	ModelData myModelData;

	math::vector3<float> myPositon = { 0, 0, 0 };
	math::vector3<float> myRotation = { 0, 0, 0 };
};

