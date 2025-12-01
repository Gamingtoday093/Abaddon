#include "pch.h"
#include "Window.h"
#include "Engine.h"
#include <format>

#include "ImGui/ImGui.h"

int main()
{
	Window* window = new Window();
	Engine* engine = new Engine(window->GetHWND());

	window->OnResize = [engine](int width, int height)
	{
		engine->Resize(width, height);
	};

	engine->Init();

	bool running = true;
	while (running)
	{
		if (!window->ProcessMessages())
		{
#ifdef enableImGui
			ImGui_ImplDX11_Shutdown();
			ImGui_ImplWin32_Shutdown();
			ImGui::DestroyContext();
#endif 
			running = false;
			break;
		}

		engine->Update();
	}

	delete engine;
	engine = nullptr;

	delete window;
	engine = nullptr;

	return 0;
}