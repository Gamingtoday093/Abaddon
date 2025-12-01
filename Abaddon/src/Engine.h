#pragma once

class DX11;
class Renderer;
class Scene;
class RenderPass;
class CubeTexture;
class Cube;

#ifdef enableImGui
class ImGuiManager;
#endif

class Engine
{
public:
	Engine(HWND& aHWND);
	~Engine();

	void Init();
	void Update();
	void BeginFrame();
	void EndFrame();
	void Resize(int width, int height);

private:
	HWND& myHWND;
	std::shared_ptr<DX11> myFramework;
	std::shared_ptr<Renderer> myRenderer;
	std::shared_ptr<Scene> myScene;

	std::unique_ptr<RenderPass> myDefaultPass;
	std::unique_ptr<RenderPass> mySkyboxPass;

	std::shared_ptr<CubeTexture> mySkyboxTexture;
	std::shared_ptr<Cube> mySkyboxMesh;

#ifdef enableImGui
	std::shared_ptr<ImGuiManager> myImGui;
#endif

	bool myIsMinimized = false;
	float myClearColor[4] = { 0.16f, 0.16f, 0.16f, 0.16f };
};

