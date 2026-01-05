#include "pch.h"
#include "NavigationManager.h"
#include "Scene/Scripts/NavigationAgent.h"
#include "Scene/Components/Components.h"

void NavigationManager::Awake()
{
    Assert(!myInstance);
    myInstance = this;
}

void NavigationManager::Update()
{
    TransformComponent& transform = GetComponent<TransformComponent>();
    
    for (size_t i = 0; i < myAgents.size(); i++)
    {
        NavigationAgent& currentAgent = *myAgents[i];
        TransformComponent& currentTransform = currentAgent.GetComponent<TransformComponent>();

        if (i == 0)
        {
            currentAgent.myTargetVelocity = (transform.myTransform.myPosition - currentTransform.myTransform.myPosition);
        }
        else
        {
            currentAgent.myTargetVelocity = (math::vector3<float>::zero() - currentTransform.myTransform.myPosition);
        }
        if (currentAgent.myTargetVelocity.LengthSqr() > 0) currentAgent.myTargetVelocity.Normalize();

        math::vector3<float> totalAvoidance = CalculateTotalAvoidanceVelocity(currentAgent);
        totalAvoidance.y = 0.f;
        currentAgent.myVelocity = currentAgent.myTargetVelocity - totalAvoidance;
        if (currentAgent.myVelocity.LengthSqr() > 0) currentAgent.myVelocity.Normalize();
    }
}

math::vector3<float> NavigationManager::CalculateTotalAvoidanceVelocity(NavigationAgent& aAgent)
{
    math::vector3<float> totalAvoidanceVelocity;

    for (size_t i = 0; i < myAgents.size(); i++)
    {
        NavigationAgent& otherAgent = *myAgents[i];
        if (&otherAgent == &aAgent) continue;
        if (otherAgent.myPriority < aAgent.myPriority) continue;
        TransformComponent& otherTransform = otherAgent.GetComponent<TransformComponent>();

        math::vector3<float> relativePosition = otherTransform.myTransform.myPosition - aAgent.GetComponent<TransformComponent>().myTransform.myPosition;
        math::vector3<float> relativeVelocity = aAgent.myTargetVelocity - otherAgent.myVelocity;
        if (relativeVelocity.LengthSqr() == 0) continue;

        // Project relativePosition onto relativeVelocity
        float t = std::clamp(relativePosition.Dot(relativeVelocity) / relativeVelocity.LengthSqr(), 0.f, 1.5f);
        math::vector3<float> projectedClosest = relativePosition - (relativeVelocity * t);

        if (projectedClosest.LengthSqr() == 0) continue;
        float radius = aAgent.GetRadius() + otherAgent.GetRadius();
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
