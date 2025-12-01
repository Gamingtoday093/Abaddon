#pragma once
#include "Bindables/CBuffer.hpp"
#include "Bindables/BlendState.h"
#include "Bindables/RenderStates.hpp"
#include "Skybox/CubeTexture.h"
#include "Skybox/Cube.h"

struct ModelData;
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
	void Render(ModelData& aModelData, Material& aMaterial, Transform& aTransform, std::shared_ptr<Camera> aCamera);
	void RenderSkybox(std::shared_ptr<Cube> aCube, std::shared_ptr<CubeTexture> aCubeTexture, std::shared_ptr<Camera> aCamera);

private:
	CBuffer<TransformBuffer> myCBufferTransform;
	CBuffer<CameraBuffer> myCBufferCamera;
	inline constexpr BlendState& GetBlendState(eBlendState aBlendState);
	std::array<BlendState, static_cast<size_t>(eBlendState::Count)> myBlendStates;
};

