#include "pch.h"
#include "Scene.h"
#include "Graphics/Renderer.h"
#include "ModelAssetHandler.h"

#include "Entity.h"

#include "Components/Components.h"
#include "Scene/Scripts/PlayerMovement.h"
#include "Scene/Scripts/Unit.h"
#include "Scene/Scripts/Managers/UnitManager.h"
#include "Scene/Scripts/LightingVisualizer.h"
#include "Scene/Scripts/NavigationAgent.h"
#include "Scene/Scripts/NavigationSpawner.h"
#include "Scene/Scripts/Managers/NavigationManager.h"
#include "Graphics/Bindables/Materials/Materials.h"

struct TextureData;

Scene::Scene(std::shared_ptr<Renderer> aRenderer, HWND aHWND) : myRenderer(std::move(aRenderer)), myHWND(aHWND)
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
	myTopDownCamera->Init(1.2f, 0.005f, 6, 80, 50, 160, { 0, 0, 0 }, { -0.9f, 0 }, -0.1f, -1.3f);

	ModelAssetHandler::LoadModel("Ship.fbx");
	ModelAssetHandler::LoadModel("ShipSmooth.fbx");
	ModelAssetHandler::LoadModel("Sphere2.fbx");
	ModelAssetHandler::LoadModel("AnimationTest2.fbx");
	ModelAssetHandler::LoadAnimations("AnimationTest2.fbx");
	ModelAssetHandler::LoadTexture("ShipTexture.png");
	ModelAssetHandler::LoadTexture("ShipEmission.png");
	ModelAssetHandler::LoadModel("gremlin.fbx");
	ModelAssetHandler::LoadTexture("sand.jpg");
	StandardMaterial& material = ModelAssetHandler::CreateMaterial<StandardMaterial>("ShipMaterial");
	material.Init({ 1,0,0,0 }, 0.4f, ModelAssetHandler::GetTextureData("ShipTexture.png"), ModelAssetHandler::GetTextureData("ShipEmission.png"));
	material.UpdateLighting({ 0.85f,0.25f,0.125f,1 }, 0.1f, { -0.5,-0.75,-0.25 });

	StandardMaterial& sandMaterial = ModelAssetHandler::CreateMaterial<StandardMaterial>("SandMaterial");
	sandMaterial.Init({ 1,0,0,0 }, 0, ModelAssetHandler::GetTextureData("sand.jpg"), TextureData());
	sandMaterial.UpdateLighting({ 0.85f,0.25f,0.125f,1 }, 0.1f, { -0.5,-0.75,-0.25 });

	//Entity unitManager = CreateEmptyEntity("UnitManager");
	//unitManager.AddComponent<ScriptComponent>().Bind<UnitManager>(unitManager);

	Entity navManager = CreateEntity("NavigationManager");
	navManager.AddComponent<ModelComponent>("Sphere2.fbx", "SandMaterial");
	navManager.AddComponent<ScriptComponent>().Bind<NavigationManager>(navManager);
	navManager.GetComponent<TransformComponent>().myTransform.myPosition = { 30, 0, 40 };

	Entity ship = CreateEntity("Ship");
	ship.AddComponent<ModelComponent>("ShipSmooth.fbx", "ShipMaterial");
	NavigationAgent* agent = ship.AddComponent<ScriptComponent>().Bind<NavigationAgent>(ship);
	agent->myMass = 10;
	agent->myPriority = 1;
	ship.GetComponent<TransformComponent>().myTransform.myPosition = { -30, 0, -40 };
	//Unit* unit = UnitManager::CreateUnit(ship);
	//unit->Init(myRenderer);

	Entity ship2 = CreateEntity("Ship2");
	ship2.AddComponent<ModelComponent>("ShipSmooth.fbx", "ShipMaterial");
	NavigationAgent* agent2 = ship2.AddComponent<ScriptComponent>().Bind<NavigationAgent>(ship2);
	agent2->myMass = 10;
	agent2->myPriority = 2;
	//ship2.GetComponent<TransformComponent>().myTransform.myPosition = { 30, 0, 20 };
	//Unit* unit2 = UnitManager::CreateUnit(ship2);
	//unit2->Init(myRenderer);

	Entity navSpawner = CreateEmptyEntity("NavigationSpawner");
	NavigationSpawner* spawner = navSpawner.AddComponent<ScriptComponent>().Bind<NavigationSpawner>(navSpawner);
	spawner->myAgentRowCount = 7;

	//Entity sphere = CreateEntity("Sphere");
	//sphere.AddComponent<ModelComponent>("Sphere2.fbx", "ShipMaterial");
	//sphere.GetComponent<TransformComponent>().myTransform.myPosition = { 0, 10, 0 };
	//LightingVisualizer* lightingVisualizer = sphere.AddComponent<ScriptComponent>().Bind<LightingVisualizer>(sphere);
	//lightingVisualizer->Init("ShipMaterial");
	
	//Entity animated = CreateEntity("Animated");
	//animated.AddComponent<ModelComponent>("AnimationTest2.fbx", "ShipMaterial");
	//animated.GetComponent<TransformComponent>().myTransform.myPosition = { 10, 10, 0 };
	//animated.AddComponent<AnimatorComponent>("Armature|Move2", ModelAssetHandler::GetAnimation("Armature|Move2").myDurationSeconds);
	
	//Entity gremlin = CreateEntity("Gremlin");
	//gremlin.AddComponent<ModelComponent>("gremlin.fbx", "SandMaterial");
	//gremlin.GetComponent<TransformComponent>().myTransform.myPosition = { -20, 0, -20 };
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

	if (Input::GetInstance().IsKeyPressed((int)eKeys::UP))
	{
		StandardMaterial& material = ModelAssetHandler::GetMaterial<StandardMaterial>("ShipMaterial");
		math::vector3<float> lightDirection = material.GetLightingDirection();
		lightDirection.x -= 0.25f;
		material.UpdateLightingDirection(lightDirection);
	}
	else if (Input::GetInstance().IsKeyPressed((int)eKeys::LEFT))
	{
		StandardMaterial& material = ModelAssetHandler::GetMaterial<StandardMaterial>("ShipMaterial");
		math::vector3<float> lightDirection = material.GetLightingDirection();
		lightDirection.y -= 0.25f;
		material.UpdateLightingDirection(lightDirection);
	}
	else if (Input::GetInstance().IsKeyPressed((int)eKeys::RIGHT))
	{
		StandardMaterial& material = ModelAssetHandler::GetMaterial<StandardMaterial>("ShipMaterial");
		math::vector3<float> lightDirection = material.GetLightingDirection();
		lightDirection.z -= 0.25f;
		material.UpdateLightingDirection(lightDirection);
	}

	auto group = myRegistry.group<TransformComponent>(entt::get<ModelComponent>, entt::exclude<AnimatorComponent>);
	for (auto entity : group)
	{
		std::tuple<TransformComponent, ModelComponent> object = group.get<TransformComponent, ModelComponent>(entity);
		auto x = std::get<1>(object).myModelName;
		ModelData& modelData = ModelAssetHandler::GetModelData(std::get<1>(object).myModelName);
		Material& material = ModelAssetHandler::GetMaterial(std::get<1>(object).myMaterialName);
		myRenderer->Render(modelData, material, std::get<0>(object).myTransform, GetCamera());
	}

	auto skinned = myRegistry.view<AnimatorComponent, TransformComponent, ModelComponent>();
	for (auto skinnedEntity : skinned)
	{
		std::tuple<AnimatorComponent, TransformComponent, ModelComponent> object = skinned.get<AnimatorComponent, TransformComponent, ModelComponent>(skinnedEntity);
	
		ModelData& modelData = ModelAssetHandler::GetModelData(std::get<2>(object).myModelName);
		Material& material = ModelAssetHandler::GetMaterial(std::get<2>(object).myMaterialName);
		Animation& animation = ModelAssetHandler::GetAnimation(std::get<0>(object).myAnimationName);
		myRenderer->Render(modelData, material, std::get<1>(object).myTransform, animation, std::get<0>(object).myTimeSeconds, GetCamera());
	}

	myRegistry.view<AnimatorComponent>().each([=](AnimatorComponent& aAnimatorComponent)
		{
			aAnimatorComponent.Update();
		});

	myRegistry.view<ScriptComponent>().each([=](entt::entity aEntity, ScriptComponent& aScriptComponent)
		{
			if (!aScriptComponent.myIsEnabled) return;
			if (!aScriptComponent.myHasStarted) 
			{
				aScriptComponent.myInstance->Start();
				aScriptComponent.myHasStarted = true;
			}

			aScriptComponent.myInstance->Update();
		});
}

Entity Scene::CreateEmptyEntity(const std::string& aName)
{
	Entity entity = { myRegistry.create(), this };
	entity.AddComponent<TagComponent>(aName);
	return entity;
}

Entity Scene::CreateEntity(const std::string& aName)
{
	Entity entity = { myRegistry.create(), this };
	entity.AddComponent<TransformComponent>();
	entity.AddComponent<TagComponent>(aName);

	return entity;
}

std::vector<Entity> Scene::GetAllEntities()
{
	auto view = myRegistry.view<entt::entity>();
	std::vector<Entity> entities;
	entities.reserve(view.size_hint());

	for (const auto& entity : view)
		entities.emplace_back(entity, this);
	
	return entities;
}

std::shared_ptr<Renderer> Scene::GetRenderer()
{
	return myRenderer;
}

std::shared_ptr<Camera> Scene::GetCamera()
{
	if (myUsingFreeLookCamera)
		return myFreeLookCamera;
	return myTopDownCamera;
}

