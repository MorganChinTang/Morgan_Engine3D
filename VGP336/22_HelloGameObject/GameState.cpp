#include "GameState.h"

using namespace Engine3D;
using namespace Engine3D::Graphics;
using namespace Engine3D::Input;
using namespace Engine3D::Physics;
void GameState::Initialize()
{
    mGameWorld.Initialize();
    GameObject* transformGO = mGameWorld.CreateGameObject("Transform");
    transformGO->AddComponent<TransformComponent>();
    transformGO->Initialize();

    GameObject* cameraGO = mGameWorld.CreateGameObject("Camera");
    mCameraComponent = cameraGO->AddComponent<CameraComponent>();
    cameraGO->AddComponent<FPSCameraComponent>();
    cameraGO->Initialize();
    
    GameObject* playerGO = mGameWorld.CreateGameObject("Player");
    TransformComponent* playerTransform= playerGO->AddComponent<TransformComponent>();
    playerTransform->position.x = 1.0f;
    playerGO->Initialize();

    GameObject* planeGO = mGameWorld.CreateGameObject("Plane");
    TransformComponent* planeTransform = planeGO->AddComponent<TransformComponent>();
    planeTransform->position.x = 2.0f;
    planeGO->Initialize();

    GameObject* player2GO = mGameWorld.CreateGameObject("Player2");
    TransformComponent* player2Transform = player2GO->AddComponent<TransformComponent>();
    player2Transform->position.x = 3.0f;
    player2GO->Initialize();

    GameObject* ballGO = mGameWorld.CreateGameObject("Ball");
    TransformComponent* ballTransform = ballGO->AddComponent<TransformComponent>();
    ballTransform->position.x = 4.0f;
    ballGO->Initialize();

    GameObject* dogGO = mGameWorld.CreateGameObject("Dog");
    TransformComponent* dogTransform = dogGO->AddComponent<TransformComponent>();
    dogTransform->position.x = 5.0f;
    dogGO->Initialize();
}
void GameState::Terminate()
{
    mGameWorld.Terminate();
}
void GameState::Update(float deltaTime)
{
    mGameWorld.Update(deltaTime);
}

void GameState::Render()
{
    mGameWorld.Render();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    mGameWorld.DebugUI();
    ImGui::End();

    SimpleDraw::Render(mCameraComponent->GetCamera());
}
