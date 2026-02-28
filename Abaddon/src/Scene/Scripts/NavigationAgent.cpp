#include "pch.h"
#include "NavigationAgent.h"
#include "Managers/NavigationManager.h"
#include "Scene/Components/Components.h"

void NavigationAgent::Awake()
{
    myTransform = &GetComponent<TransformComponent>();
    NavigationManager::GetInstance().myAgents.push_back(this);
}

void NavigationAgent::Start()
{
    myStartPosition = myTransform->myTransform.myPosition;
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
    myTransform->myTransform.myPosition += myVelocity * 0.3f;
    //if (myVelocity.LengthSqr() > 0)
    //    transform.myTransform.myRotation = math::vector4<float>::LookRotation(myVelocity, math::vector3<float>::up()).ToEuler();
}
