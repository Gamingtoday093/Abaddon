#include "pch.h"
#include "NavigationAgent.h"
#include "Managers/NavigationManager.h"
#include "Scene/Components/Components.h"

void NavigationAgent::Awake()
{
    NavigationManager::GetInstance().myAgents.push_back(this);
}

void NavigationAgent::OnDestroy()
{
    auto& agents = NavigationManager::GetInstance().myAgents;
    auto result = std::find(agents.begin(), agents.end(), this);
    if (result == agents.end()) return;
    agents.erase(result);
}

void NavigationAgent::Update()
{
    TransformComponent& transform = GetComponent<TransformComponent>();
    transform.myTransform.myPosition += myVelocity * 0.3f;
}
