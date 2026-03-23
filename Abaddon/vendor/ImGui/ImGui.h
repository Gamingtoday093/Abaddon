#pragma once
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

namespace ImGui {
	void SeparatorTextCheckbox(const char* label, const char* checkboxId, bool* v);
}