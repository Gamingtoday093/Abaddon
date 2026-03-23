#include "pch.h"
#include "NavigationManager.h"
#include "Scene/Scripts/NavigationAgent.h"
#include "Scene/Components/Components.h"

void NavigationManager::Awake()
{
    Assert(!myInstance);
    myInstance = this;
}

void NavigationManager::Start()
{
    myRVOSimulator = std::make_unique<RVO::RVOSimulator>();
    myRVOSimulator->setTimeStep(0.25f); // TODO: Replace with Deltatime
    myRVOSimulator->setAgentDefaults(25.0f, 12, 16.0f, 16.0f, 8.0f, 2.0f);

    for (auto agent : myAgents)
    {
        auto& position = agent->myTransform->myTransform.myPosition;
        agent->myRVOIndex = myRVOSimulator->addAgent({ position.x, position.z });
    }
}

void NavigationManager::Update()
{
    for (size_t i = 0; i < myAgents.size(); i++)
    {
        NavigationAgent& currentAgent = *myAgents[i];
        TransformComponent& currentTransform = *currentAgent.myTransform;

        myRVOSimulator->setAgentPosition(currentAgent.myRVOIndex, { currentTransform.myTransform.myPosition.x, currentTransform.myTransform.myPosition.z });

        if (i == 0)
        {
            currentAgent.myTargetVelocity = (GetComponent<TransformComponent>().myTransform.myPosition - currentTransform.myTransform.myPosition);
        }
        else
        {
            currentAgent.myTargetVelocity = (currentAgent.myStartPosition - currentTransform.myTransform.myPosition);
        }
        if (currentAgent.myTargetVelocity.LengthSqr() > 0.1f)
        {
            currentAgent.myTargetVelocity.Normalize();
        }
        else
        {
            currentAgent.myTargetVelocity = math::vector3<float>::zero();
        }
        myRVOSimulator->setAgentPrefVelocity(currentAgent.myRVOIndex, { currentAgent.myTargetVelocity.x, currentAgent.myTargetVelocity.z });

        //if (i == 0)
        //{
        auto& velocity = myRVOSimulator->getAgentVelocity(currentAgent.myRVOIndex);
        currentAgent.myVelocity = { velocity.x(), 0, velocity.y() };
        //}
        //else
        //{
        //    myRVOSimulator->setAgentVelocity(currentAgent.myRVOIndex, { 0, 0 });
        //}
    }
    myRVOSimulator->doStep();
}

NavigationManager* NavigationManager::myInstance = nullptr;

NavigationManager& NavigationManager::GetInstance()
{
    Assert(myInstance);
    return *myInstance;
}
