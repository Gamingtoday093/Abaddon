#include "pch.h"
#include "Engine.h"

#include "Graphics/DX11.h"
#include "Graphics/Renderer.h"
#include "Scene/Scene.h"
#include "Tools/Input.h"

#include "Graphics/RenderPass.h"
#include "Graphics/Skybox/CubeTexture.h"
#include "Graphics/Skybox/SkyboxCube.h"

#ifdef enableImGui
#include "ImGuiManager/ImGuiManager.h"
#endif

Engine::Engine(HWND aHWND) : myHWND(aHWND)
{
	myFramework = std::make_unique<DX11>(myHWND);
	myFramework->Initialize(
#ifdef DEBUG
		true
#else
		false
#endif
	);

	myRenderer = std::make_shared<Renderer>();

	myScene = std::make_shared<Scene>(myRenderer, myHWND);
	myScene->Init();

	// Create Render Passes
	myDefaultPass = std::make_unique<RenderPass>();
	myDefaultPass->Init("VertexShader_vs.cso", D3D11_CULL_BACK, "PixelShader_ps.cso");

	mySkyboxPass = std::make_unique<RenderPass>();
	mySkyboxPass->Init("Skybox_vs.cso", D3D11_CULL_NONE, "Skybox_ps.cso");

	// Skybox
	mySkyboxTexture = std::make_unique<CubeTexture>();
	mySkyboxTexture->Init("Skybox");

	mySkyboxMesh = std::make_unique<SkyboxCube>();

#ifdef enableImGui
	// ImGui
	myImGui = std::make_unique<ImGuiManager>(myHWND, myScene);
#endif
}

Engine::~Engine() = default;

void Engine::Update()
{
	BeginFrame();
	//-----------------------

	if (!myIsMinimized)
	{
		mySkyboxPass->Bind();
		myRenderer->RenderSkybox(*mySkyboxMesh, *mySkyboxTexture, myScene->GetCamera());

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
	Input::GetInstance().Update();
	if (myIsMinimized) return;
	myFramework->BindRenderTarget();
	myFramework->BeginFrame(myClearColor);

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

void Engine::Resize(int aWidth, int aHeight)
{
	myIsMinimized = aWidth == 0 || aHeight == 0;
	myFramework->Resize(aWidth, aHeight);
}
