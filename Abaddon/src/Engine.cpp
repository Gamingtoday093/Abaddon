#include "pch.h"
#include "Engine.h"

#include "Graphics/DX11.h"
#include "Graphics/Renderer.h"
#include "Scene/Scene.h"
#include "Tools/Input.h"

#include "Graphics/RenderPass.h"
#include "Graphics/Skybox/CubeTexture.h"
#include "Graphics/Skybox/Cube.h"

#ifdef enableImGui
#include "ImGuiManager/ImGuiManager.h"
#endif

Engine::Engine(HWND& aHWND) : myHWND(aHWND)
{}

Engine::~Engine()
{}

void Engine::Init()
{
	myFramework = std::make_shared<DX11>(myHWND);
	myFramework->Initialize(true);

	myRenderer = std::make_shared<Renderer>();
	myRenderer->Init();

	myScene = std::make_shared<Scene>(myRenderer, myHWND);
	myScene->Init();

	// Create Render Passes
	myDefaultPass = std::make_unique<RenderPass>();
	myDefaultPass->Init("VertexShader_vs.cso", D3D11_CULL_BACK, "PixelShader_ps.cso");

	mySkyboxPass = std::make_unique<RenderPass>();
	mySkyboxPass->Init("Skybox_vs.cso", D3D11_CULL_NONE, "Skybox_ps.cso");

	// Skybox
	mySkyboxTexture = std::make_shared<CubeTexture>();
	mySkyboxTexture->Init("Skybox");

	mySkyboxMesh = std::make_shared<Cube>();
	mySkyboxMesh->Init();

#ifdef enableImGui
	// ImGui
	myImGui = std::make_shared<ImGuiManager>(myHWND, myScene);
	myImGui->Init();
#endif
}

void Engine::Update()
{
	BeginFrame();
	//-----------------------
	
	if (!myIsMinimized)
	{
		mySkyboxPass->Bind();
		myRenderer->RenderSkybox(mySkyboxMesh, mySkyboxTexture, myScene->GetCamera());

		myDefaultPass->Bind();
		myScene->Update();
	}

	//-----------------------
	EndFrame();
}

void Engine::BeginFrame()
{
#ifdef enableImGui
	myImGui->BeginFrame();
#endif
	if (myIsMinimized) return;
	myFramework->BindRenderTarget();
	myFramework->BeginFrame(myClearColor);
	Input::GetInstance().Update();

#ifdef enableImGui
	DX11::BindRenderTargetTexture();
#endif
}

void Engine::EndFrame()
{
#ifdef enableImGui
	if (!myIsMinimized)
	{
		DX11::BindRenderTarget();
	}
	myImGui->EndFrame();
#endif

	if (!myIsMinimized)
	{
		myFramework->EndFrame();
	}
}

void Engine::Resize(int width, int height)
{
	myIsMinimized = width == 0 || height == 0;
	if (myIsMinimized) return;
	myFramework->Resize();
}
