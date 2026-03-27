#pragma once
#include "Bindables/CBuffer.hpp"
#include "Bindables/StructBuffer.hpp"
#include "Bindables/BlendState.h"
#include "Bindables/VertexShader.h"
#include "Bindables/RenderStates.hpp"
#include "Skybox/CubeTexture.h"
#include "Skybox/SkyboxCube.h"

struct ModelData;
struct Animation;
struct Transform;
class Camera;
class Material;

class Renderer
{
public:
	Renderer();
	~Renderer() = default;

	void Render(const ModelData& aModelData, const Material& aMaterial, const Transform& aTransform, const Animation& aAnimation, double aTimeSeconds, const Camera& aCamera);
	void Render(const ModelData& aModelData, const Material& aMaterial, const Transform& aTransform, const Camera& aCamera);
	void RenderSkybox(const SkyboxCube& aCube, const CubeTexture& aCubeTexture, const Camera& aCamera);

private:
	void RenderInternal(const ModelData& aModelData, const Material& aMaterial, const Transform& aTransform, const Camera& aCamera);

	CBuffer<TransformBuffer> myCBufferTransform;
	StructBuffer<DirectX::XMMATRIX> myStructAnimation;
	CBuffer<CameraBuffer> myCBufferCamera;
	InputLayout myInputLayout;
	InputLayout mySkinnedInputLayout;
	VertexShader myVertexShader;
	VertexShader mySkinnedShader;
	inline constexpr BlendState& GetBlendState(eBlendState aBlendState);
	std::array<BlendState, static_cast<size_t>(eBlendState::Count)> myBlendStates;
};

