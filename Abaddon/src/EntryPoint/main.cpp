#include "pch.h"
#include "Window.h"
#include "Engine.h"
#include <format>

#include "ImGui/ImGui.h"

int main()
{
	std::unique_ptr<Window> window = std::make_unique<Window>();
	Engine* engine = new Engine(window->GetHWND());

	window->OnResize = [engine](int width, int height)
	{
		engine->Resize(width, height);
		engine->Update();
	};

	while (window->ProcessMessages())
	{
		engine->Update();
	}

#ifdef enableImGui
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#endif

	delete engine;
	engine = nullptr;

	return 0;
}