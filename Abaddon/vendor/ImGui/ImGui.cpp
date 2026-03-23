#include "pch.h"
#include "ImGui.h"

void ImGui::SeparatorTextCheckbox(const char* label, const char* checkboxId, bool* v)
{
	ImGui::SeparatorText(label);
	auto& vtxBuffer = ImGui::GetWindowDrawList()->VtxBuffer;
	float lastX = vtxBuffer.back().pos.x + 30.f;
	for (int i = vtxBuffer.size() - 2; i >= 0; i--)
	{
		if (vtxBuffer[i].pos.x < lastX) continue;

		vtxBuffer[i].pos.x -= 30;
		vtxBuffer[i - 1].pos.x -= 30;
		break;
	}
	ImGui::SameLine(ImGui::GetWindowWidth() - 30);
	ImGui::Checkbox(checkboxId, v);
}
