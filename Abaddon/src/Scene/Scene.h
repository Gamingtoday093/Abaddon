#pragma once
#include "Cameras.h"
#include "EnTT/entt.hpp"

class Renderer;
class Script;
class Entity;
struct ScriptComponent;

class Scene
{
public:
	Scene(std::shared_ptr<Renderer> aRenderer, HWND& aHWND);
	~Scene();

	void Init();
	void Update();

	Entity CreateEmptyEntity(std::string aName = "New Entity");
	Entity CreateEntity(std::string aName = "Unnamed");
	std::vector<Entity> GetAllEntities();

	std::shared_ptr<Renderer> GetRenderer();
	HWND& myHWND;
	
	std::shared_ptr<Camera> GetCamera();

private:
	friend class Entity;

	std::shared_ptr<Renderer> myRenderer;
	bool myUsingFreeLookCamera = false;
	std::shared_ptr<FreeLookCamera> myFreeLookCamera;
	std::shared_ptr<TopDownCamera> myTopDownCamera;
	entt::registry myRegistry;
};
