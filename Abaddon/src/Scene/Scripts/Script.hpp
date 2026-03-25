#pragma once
#include "Scene/Entity.h"
#include <typeinfo>

struct PropertyDescriptor
{
	std::type_info Type;
	std::string PropertyName;
	void* PropertyValue;
};

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
		std::array<char, N> myData{};

		constexpr const char* c_str() const { return myData.data(); }
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

		scriptName.myData[nextBufferIndex++] = nameView[0];
		for (size_t i = 1; i < nameView.size(); i++)
		{
			char c = nameView[i];

			if (IsUpper(c) && !IsUpper(nameView[i - 1]))
				scriptName.myData[nextBufferIndex++] = ' ';

			scriptName.myData[nextBufferIndex++] = c;
		}

		if (!nameView.ends_with("Script"))
		{
			scriptName.myData.at(nextBufferIndex++) = ' ';
			scriptName.myData.at(nextBufferIndex++) = 'S';
			scriptName.myData.at(nextBufferIndex++) = 'c';
			scriptName.myData.at(nextBufferIndex++) = 'r';
			scriptName.myData.at(nextBufferIndex++) = 'i';
			scriptName.myData.at(nextBufferIndex++) = 'p';
			scriptName.myData.at(nextBufferIndex++) = 't';
		}

		scriptName.myData.at(nextBufferIndex++) = '\0';
		return scriptName;
	}

private:
	bool myHasStarted = false;
	bool myIsEnabled = true;

	friend class Scene;
	friend struct ScriptComponent;
};

