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
    
    if (myAgents.size() > 0)
    {
        int index = 0;
        NavigationAgent& currentAgent = *myAgents.at(index);
        TransformComponent& currentTransform = currentAgent.GetComponent<TransformComponent>();

        currentAgent.myTargetVelocity = (transform.myTransform.myPosition - currentTransform.myTransform.myPosition);
        if (currentAgent.myTargetVelocity.LengthSqr() > 0) currentAgent.myTargetVelocity.Normalize();

        math::vector3<float> totalAvoidanceVelocity = { 0, 0, 0 };

        for (size_t i = 0; i < myAgents.size(); i++)
        {
            if (i == index) continue;

            NavigationAgent& otherAgent = *myAgents.at(i);
            TransformComponent& otherTransform = otherAgent.GetComponent<TransformComponent>();

            math::vector3<float> relativePosition = otherTransform.myTransform.myPosition - currentTransform.myTransform.myPosition;
            math::vector3<float> relativeVelocity = currentAgent.myTargetVelocity - otherAgent.myVelocity;

            float t = std::clamp(relativePosition.Dot(relativeVelocity) / relativeVelocity.Dot(relativeVelocity), 0.0f, PREDICTION_HORIZON_SECONDS);

            math::vector3<float> closest = relativePosition - (relativeVelocity * t);

            if (closest.LengthSqr() == 0) continue;
            float radius = currentAgent.GetRadius() + otherAgent.GetRadius();
            if (closest.LengthSqr() > radius * radius) continue;

            float avoidanceFactor = otherAgent.GetInfluence() / (currentAgent.GetInfluence() + otherAgent.GetInfluence());
            avoidanceFactor = std::clamp(avoidanceFactor, 0.0f, 1.0f);

            float penetration = radius - closest.Length();
            float pushSpeed = penetration / std::max(t, 0.1f);

            math::vector3<float> avoidanceVelocity = closest.GetNormalized() * (pushSpeed * avoidanceFactor);

            totalAvoidanceVelocity += avoidanceVelocity;
        }

        currentAgent.myVelocity = currentAgent.myTargetVelocity - totalAvoidanceVelocity;
    }
}

NavigationManager* NavigationManager::myInstance = nullptr;

NavigationManager& NavigationManager::GetInstance()
{
    Assert(myInstance);
    return *myInstance;
}
