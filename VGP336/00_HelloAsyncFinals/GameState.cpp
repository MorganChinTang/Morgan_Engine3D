#include "GameState.h"
#include "math.h"

#include "CustomDebugDrawComponent.h"
#include "CustomDebugDrawService.h"

using namespace Engine3D;
using namespace Engine3D::Graphics;
using namespace Engine3D::Input;
using namespace Engine3D::Physics;

Service* MakeCustomService(const std::string& serviceName, GameWorld& gameWorld)
{
    if (serviceName == "CustomDebugDrawService")
    {
        return gameWorld.AddService<CustomDebugDrawService>();
    }

    // add another 
    // add next..
    return nullptr;

}

Component* MakeCustomComponent(const std::string& componentName, GameObject& gameObject)
{
    if (componentName == "CustomDebugDrawComponent")
    {
        return gameObject.AddComponent<CustomDebugDrawComponent>();
    }
    return nullptr;
}
Component* GetCustomComponent(const std::string& componentName, GameObject& gameObject)
{
    if (componentName == "CustomDebugDrawComponent")
    {
        return gameObject.GetComponent<CustomDebugDrawComponent>();
    }
    return nullptr;
}
void GameState::Initialize()
{
    mLevelFile = L"../../Assets/Templates/Levels/async_level.json";

    // set a callback to try make a custom service (any serice that is NOT part of the engine and unique to the project)
    GameWorld::SetCustomService(MakeCustomService);
    //sets callbacks to try make/get a custom component (any component that is NOT part of the engine)
    GameObjectFactory::SetCustomMake(MakeCustomComponent);
    GameObjectFactory::SetCustomGet(GetCustomComponent);

    mGameWorld.LoadLevel(mLevelFile);
    mGridPlacementController.Initialize(mGameWorld);
    if (PhysicsService* physicsService = mGameWorld.GetService<PhysicsService>())
    {
        physicsService->SetEnabled(false);
    }
    mAsyncLoadController.Initialize(mGameWorld, mGridPlacementController);

}

void GameState::Terminate()
{
    mAsyncLoadController.Terminate();
    mGridPlacementController.Terminate();
    mGameWorld.Terminate();
}

void GameState::Update(float deltaTime)
{
    mGameWorld.Update(deltaTime);
    mGridPlacementController.Update(deltaTime);
    mAsyncLoadController.Update(deltaTime);
}

void GameState::Render()
{
    mGameWorld.Render();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    mGridPlacementController.DebugUI();
    mAsyncLoadController.DebugUI();
    mGameWorld.DebugUI();

    if (ImGui::Button("ReloadLevel"))
    {
        mAsyncLoadController.Terminate();
        mGridPlacementController.Terminate();
        mGameWorld.Terminate();
        mGameWorld.LoadLevel(mLevelFile);
        mGridPlacementController.Initialize(mGameWorld);
        if (PhysicsService* physicsService = mGameWorld.GetService<PhysicsService>())
        {
            physicsService->SetEnabled(false);
        }
        mAsyncLoadController.Initialize(mGameWorld, mGridPlacementController);
    }

    ImGui::End();
}