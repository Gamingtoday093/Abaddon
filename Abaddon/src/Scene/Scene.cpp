#include "pch.h"
#include "Scene.h"
#include "ModelAssetHandler.h"

#include "Entity.h"

#include "Scene/Scripts/PlayerMovement.h"
#include "Scene/Scripts/Unit.h"
#include "Managers/UnitManager.h"

Scene::Scene(Renderer& aRenderer, HWND& aHWND) : myRenderer(aRenderer), myHWND(aHWND)
{
}

Scene::~Scene()
{
	myRegistry.view<ScriptComponent>().each([=](entt::entity aEntity, ScriptComponent& aScriptComponent)
		{
			if (aScriptComponent.myInstance)
			{
				aScriptComponent.myInstance->OnDestroy();
			}
		});
}

void Scene::Init()
{
	myFreeLookCamera = std::make_shared<FreeLookCamera>();
	myFreeLookCamera->Init(0.125f, 0.5f, 0.1f, 3, 0.005f);

	myTopDownCamera = std::make_shared<TopDownCamera>();
	myTopDownCamera->Init(0.5f, 0.005f, 4, 50, 25, 90, { 0, 0, 0 }, { -0.9f, 0 }, -0.1, -1.3);

	ModelAssetHandler::LoadModel("Ship.fbx");
	ModelAssetHandler::LoadTexture("ShipTexture.png");
	ModelAssetHandler::LoadModel("gremlin.fbx");
	ModelAssetHandler::LoadTexture("sand.jpg");

	Entity unitManager = CreateEmptyEntity();
	unitManager.AddComponent<ScriptComponent>().Bind<UnitManager>(unitManager);

	Entity ship = CreateEntity();
	ship.GetComponent<ModelComponent>().SetModelAndTexture("Ship.fbx", "ShipTexture.png");
	Unit* unit = UnitManager::CreateUnit(ship);
	unit->Init(myRenderer);
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

	auto group = myRegistry.group<TransformComponent>(entt::get<ModelComponent>);
	for (auto entity : group)
	{
		std::tuple<TransformComponent, ModelComponent> object = group.get<TransformComponent, ModelComponent>(entity);

		ModelData& modelData = ModelAssetHandler::GetModelData(std::get<1>(object).myModelName);
		TextureData& textureData = ModelAssetHandler::GetTextureData(std::get<1>(object).myTextureName);
		myRenderer.Render(modelData, textureData, std::get<0>(object).myTransform, GetCamera());
	}

	myRegistry.view<ScriptComponent>().each([=](entt::entity aEntity, ScriptComponent& aScriptComponent)
		{
			if (!aScriptComponent.myHasStarted) 
			{
				aScriptComponent.myInstance->Start();
				aScriptComponent.myHasStarted = true;
			}

			aScriptComponent.myInstance->Update();
		});
}

Entity Scene::CreateEmptyEntity(std::string aName)
{
	return { myRegistry.create(), this };
}

Entity Scene::CreateEntity(std::string aName)
{
	Entity entity = { myRegistry.create(), this };
	entity.AddComponent<TransformComponent>();
	entity.AddComponent<TagComponent>(aName);
	entity.AddComponent<ModelComponent>("gremlin.fbx", "gremlin.jpg");

	return entity;
}

entt::registry* Scene::FuckENTT()
{
	return &myRegistry;
}

std::shared_ptr<Camera> Scene::GetCamera()
{
	if (myUsingFreeLookCamera)
		return myFreeLookCamera;
	return myTopDownCamera;
}

