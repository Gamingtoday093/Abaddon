#pragma once

// Includes
#include "Log.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <memory>
#include <chrono>
#include "Math/vectors.h"
#include "EnTT/entt.hpp"
#include <d3d11.h>
#include <wrl.h>

// Functions
inline std::wstring AddStringsReturnWStr(const std::string aString1, const std::string aString2)
{
	std::string newString(aString1 + aString2);
	std::wstring newWString(newString.begin(), newString.end());

	return newWString;
}

// Defines
#define enableImGui
#define enableAssert