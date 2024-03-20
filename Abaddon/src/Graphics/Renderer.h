#pragma once
#include "Scene/Model.h"
#include "Skybox/CubeTexture.h"
#include "Skybox/Cube.h"

class Renderer
{
public:
	Renderer() = default;
	~Renderer() = default;

	void Render(std::vector<std::shared_ptr<Model>> aModelList);
	void RenderSkybox(std::shared_ptr <Cube> aCube, std::shared_ptr<CubeTexture> aCubeTexture);

private:

};

