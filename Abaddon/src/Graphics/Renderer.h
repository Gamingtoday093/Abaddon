#pragma once
#include "Bindables/CBuffer.hpp"
#include "Skybox/CubeTexture.h"
#include "Skybox/Cube.h"

struct ModelData;
struct TextureData;
struct Transform;
class Camera;

class Renderer
{
public:
	Renderer() = default;
	~Renderer() = default;

	void Init();
	void Render(ModelData& aModelData, TextureData& aTextureData, Transform& aTransform, std::shared_ptr<Camera> aCamera);
	void RenderSkybox(std::shared_ptr <Cube> aCube, std::shared_ptr<CubeTexture> aCubeTexture, std::shared_ptr<Camera> aCamera);

private:
	CBuffer<TransformBuffer> myCBufferTransform;
};

