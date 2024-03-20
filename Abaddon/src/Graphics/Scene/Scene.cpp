#include "pch.h"
#include "Scene.h"

void Scene::Init()
{
	myFreeLookCamera = std::make_shared<FreeLookCamera>();
	myFreeLookCamera->Init(0.125f, 0.5f, 0.1f, 3, 0.005f);

	myTopDownCamera = std::make_shared<TopDownCamera>();
	myTopDownCamera->Init(0.5f, 0.005f, 4, 50, 25, 90, { 0, 0, 0 }, { -0.9f, 0 }, -0.1, -1.3);
}

void Scene::Awake()
{
	for (std::shared_ptr<Model> model : myModels)
	{
		model->Awake();
	}
}

void Scene::Update()
{
	if (Input::GetInstance().IsKeyPressed((int)eKeys::V))
	{
		myUsingFreeLookCamera = !myUsingFreeLookCamera;
		if (myUsingFreeLookCamera)
			myFreeLookCamera->SetTransformation(myTopDownCamera->GetPosition(), myTopDownCamera->GetRotation());
	}

	if (myUsingFreeLookCamera)
		myFreeLookCamera->Update();
	else
		myTopDownCamera->Update();

	for (std::shared_ptr<Model> model : myModels)
	{
		model->UpdateRender(GetCamera());
		model->Update();
	}
}

std::shared_ptr<Camera> Scene::GetCamera()
{
	if (myUsingFreeLookCamera)
		return myFreeLookCamera;
	return myTopDownCamera;
}

std::vector<std::shared_ptr<Model>> Scene::GetModels()
{
	return myModels;
}

void Scene::AddModel(std::shared_ptr<Model> aModel)
{
	myModels.push_back(aModel);
}
