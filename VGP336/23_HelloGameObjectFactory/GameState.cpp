#include "GameState.h"

using namespace Engine3D;
using namespace Engine3D::Graphics;
using namespace Engine3D::Input;
using namespace Engine3D::Physics;
void GameState::Initialize()
{
    mGameWorld.AddService<CameraService>;
    mGameWorld.Initialize();
    GameObject* transformGO = mGameWorld.CreateGameObject("Transform", "L../../Assets/Templates/Objects/transform_obj.json");
    transformGO->AddComponent<TransformComponent>();
    transformGO->Initialize();

    GameObject* cameraGO = mGameWorld.CreateGameObject("Camera", "L../../Assets/Templates/Objects/fps_camera.json");
    mCameraComponent = cameraGO->GetComponent<CameraComponent>();
    cameraGO->AddComponent<FPSCameraComponent>();
    cameraGO->Initialize();
    
    GameObject* playerGO = mGameWorld.CreateGameObject("Player", "L../../Assets/Templates/Objects/transform_obj.json");
    TransformComponent* playerTransform= playerGO->GetComponent<TransformComponent>();
    playerTransform->position.x = 1.0f;
    playerGO->Initialize();

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
