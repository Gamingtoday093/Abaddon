#include "pch.h"
#include "NavigationManager.h"
#include "Scene/Scripts/NavigationAgent.h"
#include "Scene/Components/Components.h"
#include "Tools/Stopwatch.h"
#include "Graphics/GizmoRenderer.h"

void NavigationManager::Awake()
{
    Assert(!myInstance);
    myInstance = this;
}

void NavigationManager::Start()
{
    myNavGrid.Resize(256, 256);
    myNavGrid.StampCircle(myNavGrid.GetNearest({ 50, 10 }), 0xff, 8);
    myNavGrid.StampSquare(myNavGrid.GetNearest({ 50, 0 }), 0xff, 2);
    myNavGrid.myNodeSize = 8;

    Stopwatch sw = Stopwatch::StartNew();
    myNavGrid.Pathfind({ -20.5f, 0.5f }, { 250.f * 4, 100.f}, myResultPath);
    sw.Stop();

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
    auto gridCenter = myNavGrid.GetWorldPosition(((myNavGrid.GetWidth() / 2) * myNavGrid.GetHeight()) + myNavGrid.GetHeight() / 2);
    myEntity.GetScene().GetGizmoRenderer()->RenderPlane({ gridCenter.x, 0, gridCenter.y }, 
        { (myNavGrid.myNodeSize * myNavGrid.GetWidth() / 2) + myNavGrid.myNodeSize / 2, (myNavGrid.myNodeSize * myNavGrid.GetHeight() / 2) + myNavGrid.myNodeSize / 2 },
        { 0, 1, 0, 0.3f });

    gridCenter = myNavGrid.GetWorldPosition(0);
    myEntity.GetScene().GetGizmoRenderer()->RenderPlane({ gridCenter.x, 0, gridCenter.y }, { myNavGrid.myNodeSize / 2, myNavGrid.myNodeSize / 2 });
    
    for (size_t i = 0; i < myResultPath.size() - 1; i++)
    {
        auto fromPosition = myResultPath[i];
        auto toPosition = myResultPath[i + 1];
        myEntity.GetScene().GetGizmoRenderer()->RenderLine({ fromPosition.x, 0, fromPosition.y }, { toPosition.x, 0, toPosition.y }, { 1, 1, 0, 1 });
        myEntity.GetScene().GetGizmoRenderer()->RenderCube({ fromPosition.x, 0, fromPosition.y }, { 1, 1, 1 }, { 1, 1, 0, 1 });
    }
    if (!myResultPath.empty()) myEntity.GetScene().GetGizmoRenderer()->RenderCube({ myResultPath.back().x, 0, myResultPath.back().y}, {1, 1, 1}, {1, 1, 0, 1});

    for (size_t i = 0; i < myNavGrid.myNodes.size(); i++)
    {
        if (myNavGrid.myNodes[i] == 0) continue;
        auto worldPosition = myNavGrid.GetWorldPosition(i);
        myEntity.GetScene().GetGizmoRenderer()->RenderPlane({ worldPosition.x, 0, worldPosition.y }, { myNavGrid.myNodeSize / 2, myNavGrid.myNodeSize / 2 }, { 1, 0, 0, 1 });
    }

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
