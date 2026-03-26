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

void NavigationManager::Start()
{
    myNavGrid.Resize(512, 512);
    myNavGrid.StampCircle(myNavGrid.GetNearest({ 50, 10 }), 0xff, 8);
    myNavGrid.myNodeSize = 8;

    std::vector<math::vector2<float>> resultPath;
    Stopwatch sw = Stopwatch::StartNew();
    myNavGrid.Pathfind({ -20.5f, 0.5f }, { 250.f * 4, 100.f}, resultPath);
    sw.Stop();

    for (size_t i = 0; i < myNavGrid.myNodes.size(); i++)
    {
        if (myNavGrid.myNodes[i] == 0) continue;
        Entity line = myEntity.GetScene().CreateEntity("Block");
        line.AddComponent<ModelComponent>("Sphere2.fbx", "ShipMaterial");
        TransformComponent& transform = line.GetComponent<TransformComponent>();
        auto pos = myNavGrid.GetWorldPosition(i);
        transform.myTransform.myPosition = { pos.x, 0, pos.y };
        float scale = float(myNavGrid.myNodes[i]) / 255.f;
        scale *= 2;
        transform.myTransform.myScale = { scale, scale, scale };
    }

    for (size_t i = 0; i < resultPath.size(); i++)
    {
        auto node = resultPath[i];

        Entity line = myEntity.GetScene().CreateEntity("Line");
        line.AddComponent<ModelComponent>("Sphere2.fbx", "SandMaterial");
        TransformComponent& transform = line.GetComponent<TransformComponent>();
        transform.myTransform.myPosition = { node.x, 0, node.y };
    }
    LOG("Pathfinding took: " + std::to_string(sw.GetElapsedMilliseconds()) + "ms");

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
