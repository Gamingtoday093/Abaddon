#pragma once
#include "Cameras.h"
#include "EnTT/entt.hpp"

class Renderer;
class GizmoRenderer;
class Script;
class Entity;
struct ScriptComponent;

class Scene
{
public:
	Scene(std::shared_ptr<Renderer> aRenderer, std::shared_ptr<GizmoRenderer> aGizmoRenderer, HWND aHWND);
	~Scene();

	void Init();
	void Update();

	Entity CreateEmptyEntity(const std::string& aName = "New Entity");
	Entity CreateEntity(const std::string& aName = "Unnamed");
	std::vector<Entity> GetAllEntities();

	std::shared_ptr<Renderer> GetRenderer();
	std::shared_ptr<GizmoRenderer> GetGizmoRenderer();
	HWND myHWND;
	
	const Camera& GetCamera() const;
	void FocusCamera(const math::vector3<float>& aPosition);

private:
	friend class Entity;

	std::shared_ptr<Renderer> myRenderer;
	std::shared_ptr<GizmoRenderer> myGizmoRenderer;
	bool myUsingFreeLookCamera = false;
	FreeLookCamera myFreeLookCamera;
	TopDownCamera myTopDownCamera;
	entt::registry myRegistry;
};
