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
	Scene(std::shared_ptr<Renderer> aRenderer, HWND aHWND);
	~Scene();

	void Init();
	void Update();

	Entity CreateEmptyEntity(const std::string& aName = "New Entity");
	Entity CreateEntity(const std::string& aName = "Unnamed");
	std::vector<Entity> GetAllEntities();

	std::shared_ptr<Renderer> GetRenderer();
	HWND myHWND;
	
	Camera& GetCamera() const;

private:
	friend class Entity;

	std::shared_ptr<Renderer> myRenderer;
	bool myUsingFreeLookCamera = false;
	std::unique_ptr<FreeLookCamera> myFreeLookCamera;
	std::unique_ptr<TopDownCamera> myTopDownCamera;
	entt::registry myRegistry;
};
