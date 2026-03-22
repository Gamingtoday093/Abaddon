#include "pch.h"
#include "NavigationManager.h"
#include "Scene/Scripts/NavigationAgent.h"
#include "Scene/Components/Components.h"
#include "Tools/Stopwatch.h"

void NavigationManager::Awake()
{
    Assert(!myInstance);
    myInstance = this;
}

void NavigationManager::Update()
{
    TransformComponent& transform = GetComponent<TransformComponent>();
    bool useSpatialHash = myAgents.size() > 200;
    if (useSpatialHash) mySpatialHash.Rebuild(myAgents);
    
    for (size_t i = 0; i < myAgents.size(); i++)
    {
        NavigationAgent& currentAgent = *myAgents[i];
        TransformComponent& currentTransform = *currentAgent.myTransform;

        if (i == 0)
        {
            currentAgent.myTargetVelocity = (transform.myTransform.myPosition - currentTransform.myTransform.myPosition);
        }
        else
        {
            currentAgent.myTargetVelocity = (currentAgent.myStartPosition - currentTransform.myTransform.myPosition);
        }
        if (currentAgent.myTargetVelocity.LengthSqr() > 0) currentAgent.myTargetVelocity.Normalize();

        math::vector3<float> totalAvoidance = CalculateTotalAvoidanceVelocity(currentAgent, useSpatialHash ? mySpatialHash.Query(currentAgent.myTransform->myTransform.myPosition, currentAgent.myRadius) : myAgents);
        totalAvoidance.y = 0.f;
        currentAgent.myVelocity = currentAgent.myTargetVelocity - totalAvoidance;
        if (currentAgent.myVelocity.LengthSqr() > 0) currentAgent.myVelocity.Normalize();
    }
}

math::vector3<float> NavigationManager::CalculateTotalAvoidanceVelocity(NavigationAgent& aAgent, const std::vector<NavigationAgent*>& aAgents)
{
    math::vector3<float> totalAvoidanceVelocity;

    for (size_t i = 0; i < aAgents.size(); i++)
    {
        NavigationAgent& otherAgent = *aAgents[i];
        if (&otherAgent == &aAgent) continue;
        if (otherAgent.myPriority < aAgent.myPriority) continue;
        TransformComponent& otherTransform = *otherAgent.myTransform;
    
        math::vector3<float> relativePosition = otherTransform.myTransform.myPosition - aAgent.myTransform->myTransform.myPosition;
        math::vector3<float> relativeVelocity = aAgent.myTargetVelocity - otherAgent.myVelocity;
        if (relativeVelocity.LengthSqr() == 0) continue;
    
        // Project relativePosition onto relativeVelocity
        float t = std::clamp(relativePosition.Dot(relativeVelocity) / relativeVelocity.LengthSqr(), 0.f, 1.5f);
        math::vector3<float> projectedClosest = relativePosition - (relativeVelocity * t);
    
        if (projectedClosest.LengthSqr() == 0) continue;
        float radius = aAgent.myRadius + otherAgent.myRadius;
        if (projectedClosest.LengthSqr() > radius * radius) continue;
    
        float avoidanceFactor = otherAgent.GetInfluence() / (aAgent.GetInfluence() + otherAgent.GetInfluence());
        avoidanceFactor = std::clamp(avoidanceFactor, 0.0f, 1.0f);
    
        float penetration = radius - projectedClosest.Length();
        float pushSpeed = penetration / std::max(t, 0.1f);
    
        math::vector3<float> avoidanceVelocity = projectedClosest.GetNormalized() * (pushSpeed * avoidanceFactor);
    
        totalAvoidanceVelocity += avoidanceVelocity;
    }

    return totalAvoidanceVelocity;
}

NavigationManager* NavigationManager::myInstance = nullptr;

NavigationManager& NavigationManager::GetInstance()
{
    Assert(myInstance);
    return *myInstance;
}
