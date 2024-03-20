#pragma once
#include "Cameras.h"
#include "Model.h"

class Scene
{
public:
	Scene() = default;
	~Scene() = default;

	void Init();
	void Awake();
	void Update();

	void AddModel(std::shared_ptr<Model> aModel);

	std::shared_ptr<Camera> GetCamera();
	std::vector<std::shared_ptr<Model>> GetModels();

private:
	bool myUsingFreeLookCamera = false;
	std::shared_ptr<FreeLookCamera> myFreeLookCamera;
	std::shared_ptr<TopDownCamera> myTopDownCamera;
	std::vector<std::shared_ptr<Model>> myModels;
};

