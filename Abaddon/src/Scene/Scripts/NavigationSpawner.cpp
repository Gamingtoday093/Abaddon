#include "pch.h"
#include "NavigationSpawner.h"
#include "NavigationAgent.h"
#include "Scene/Components/Components.h"

void NavigationSpawner::Start()
{
	for (size_t i = 0; i < myAgentRowCount * myAgentRowCount; i++)
	{
		Entity agent = myEntity.GetScene().CreateEntity("Agent " + std::to_string(i));
		agent.AddComponent<ModelComponent>("ShipSmooth.fbx", "ShipMaterial");
		NavigationAgent* script = agent.AddComponent<ScriptComponent>().Bind<NavigationAgent>(agent);
		agent.GetComponent<TransformComponent>().myTransform.myPosition = { float(50 + 40 * (i / myAgentRowCount)), 0, float(35 * (i % myAgentRowCount)) };
	}
}
