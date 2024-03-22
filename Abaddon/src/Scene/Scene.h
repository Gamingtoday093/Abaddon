#pragma once
#include "Cameras.h"
#include "EnTT/entt.hpp"
#include "Graphics/Renderer.h"

class Script;
class Entity;

class Scene
{
public:
	Scene(Renderer& aRenderer, HWND& aHWND);
	~Scene();

	void Init();
	void Update();

	Entity CreateEmptyEntity(std::string aName = "New Entity");
	Entity CreateEntity(std::string aName = "Unnamed");

	entt::registry* FuckENTT();
	HWND& myHWND;

	std::shared_ptr<Camera> GetCamera();

private:
	friend class Entity;

	Renderer& myRenderer;
	bool myUsingFreeLookCamera = false;
	std::shared_ptr<FreeLookCamera> myFreeLookCamera;
	std::shared_ptr<TopDownCamera> myTopDownCamera;
	entt::registry myRegistry;
};
