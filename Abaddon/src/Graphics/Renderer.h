#pragma once
#include "Bindables/CBuffer.hpp"
#include "Bindables/StructBuffer.hpp"
#include "Bindables/BlendState.h"
#include "Bindables/VertexShader.h"
#include "Bindables/RenderStates.hpp"
#include "Skybox/CubeTexture.h"
#include "Skybox/Cube.h"

struct ModelData;
struct Animation;
struct TextureData;
struct Transform;
class Camera;
class Material;

class Renderer
{
public:
	Renderer() = default;
	~Renderer() = default;

	void Init();
	void Render(ModelData& aModelData, Material& aMaterial, const Transform& aTransform, const Animation& aAnimation, double aTimeSeconds, std::shared_ptr<Camera> aCamera);
	void Render(ModelData& aModelData, Material& aMaterial, const Transform& aTransform, std::shared_ptr<Camera> aCamera);
	void RenderSkybox(std::shared_ptr<Cube> aCube, std::shared_ptr<CubeTexture> aCubeTexture, std::shared_ptr<Camera> aCamera);

private:
	void RenderInternal(ModelData& aModelData, Material& aMaterial, const Transform& aTransform, const Camera& aCamera);

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

