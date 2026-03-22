#include "pch.h"
#include "NavigationSpawner.h"
#include "NavigationAgent.h"
#include "Scene/Components/Components.h"

void NavigationSpawner::Start()
{
	int rows = myAgentCount / sqrt(myAgentCount);
	for (size_t i = 0; i < myAgentCount; i++)
	{
		Entity agent = myEntity.GetScene().CreateEntity("Agent " + std::to_string(i));
		agent.AddComponent<ModelComponent>("ShipSmooth.fbx", "ShipMaterial");
		NavigationAgent* script = agent.AddComponent<ScriptComponent>().Bind<NavigationAgent>(agent);
		agent.GetComponent<TransformComponent>().myTransform.myPosition = { float(50 + 40 * (i / rows)), 0, float(35 * (i % rows)) };
	}
}
