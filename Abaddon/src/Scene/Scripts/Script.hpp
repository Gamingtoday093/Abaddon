#pragma once
#include "Scene/Entity.h"

class Script
{
public:
	Script() = default;
	virtual ~Script() = default;

	inline bool GetEnabled() const
	{
		return myIsEnabled;
	}

	void SetEnabled(bool aNewEnabled)
	{
		if (myIsEnabled == aNewEnabled) return;
		myIsEnabled = aNewEnabled;
		myHasStarted = false;
	}

	template<typename T>
	T& GetComponent()
	{
		return myEntity.GetComponent<T>();
	}

#define SCRIPT_NAME(ScriptName)												\
	const char* GetScriptName() const override								\
	{																		\
		static constexpr auto scriptName = FormatScriptName(#ScriptName);	\
		return scriptName.c_str();											\
	};
	virtual const char* GetScriptName() const = 0;

protected:
	virtual void Awake() {};
	virtual void Start() {};
	virtual void OnDestroy() {};
	virtual void Update() {};

	Entity myEntity;

	template<size_t N>
	struct FixedString
	{
		std::array<char, N> data{};

		constexpr const char* c_str() const { return data.data(); }
	};

	static constexpr bool IsUpper(char ch)
	{
		return ch >= 'A' && ch <= 'Z';
	}

	template<size_t N>
	static constexpr auto FormatScriptName(const char (&aScriptName)[N])
	{
		std::string_view nameView = std::string_view(aScriptName);

		size_t nextBufferIndex = 0;
		FixedString<((N - 1) * 2) + 1 + 7> scriptName;

		scriptName.data[nextBufferIndex++] = nameView[0];
		for (size_t i = 1; i < nameView.size(); i++)
		{
			char c = nameView[i];

			if (IsUpper(c) && !IsUpper(nameView[i - 1]))
				scriptName.data[nextBufferIndex++] = ' ';

			scriptName.data[nextBufferIndex++] = c;
		}

		if (!nameView.ends_with("Script"))
		{
			scriptName.data.at(nextBufferIndex++) = ' ';
			scriptName.data.at(nextBufferIndex++) = 'S';
			scriptName.data.at(nextBufferIndex++) = 'c';
			scriptName.data.at(nextBufferIndex++) = 'r';
			scriptName.data.at(nextBufferIndex++) = 'i';
			scriptName.data.at(nextBufferIndex++) = 'p';
			scriptName.data.at(nextBufferIndex++) = 't';
		}

		scriptName.data.at(nextBufferIndex++) = '\0';
		return scriptName;
	}

private:
	bool myHasStarted = false;
	bool myIsEnabled = true;

	friend class Scene;
	friend struct ScriptComponent;
};

