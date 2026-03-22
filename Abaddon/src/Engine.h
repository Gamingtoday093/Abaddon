#pragma once

class DX11;
class Renderer;
class Scene;
class RenderPass;
class CubeTexture;
class SkyboxCube;

#ifdef enableImGui
class ImGuiManager;
#endif

class Engine
{
public:
	Engine(HWND aHWND);
	~Engine();

	void Update();
	void Resize(int aWidth, int aHeight);

private:
	void BeginFrame();
	void EndFrame();

	HWND myHWND;

	std::unique_ptr<DX11> myFramework;
	std::shared_ptr<Renderer> myRenderer;
	std::shared_ptr<Scene> myScene;

	std::unique_ptr<RenderPass> myDefaultPass;
	std::unique_ptr<RenderPass> mySkyboxPass;

	std::unique_ptr<CubeTexture> mySkyboxTexture;
	std::unique_ptr<SkyboxCube> mySkyboxMesh;

#ifdef enableImGui
	std::unique_ptr<ImGuiManager> myImGui;
#endif

	bool myIsMinimized = false;
	float myClearColor[4] = { 0.16f, 0.16f, 0.16f, 0.16f };
};

