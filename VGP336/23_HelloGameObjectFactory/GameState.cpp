#include "GameState.h"
#include "math.h"

using namespace Engine3D;
using namespace Engine3D::Graphics;
using namespace Engine3D::Input;
using namespace Engine3D::Physics;
void GameState::Initialize()
{
    mGameWorld.AddService<CameraService>();
    mGameWorld.AddService<RenderService>();
    mGameWorld.AddService<PhysicsService>();
    mGameWorld.Initialize();

    std::vector<GameObject*> gameObjects;
    gameObjects.push_back(mGameWorld.CreateGameObject("Transform", L"../../Assets/Templates/Objects/transform_obj.json"));
    gameObjects.push_back(mGameWorld.CreateGameObject("Camera", L"../../Assets/Templates/Objects/fps_camera.json"));

    gameObjects.push_back(mGameWorld.CreateGameObject("Player", L"../../Assets/Templates/Objects/transform_obj.json"));
    TransformComponent* playerTransform = gameObjects.back()->GetComponent<TransformComponent>();
    playerTransform->position.x = 2.0f;

    gameObjects.push_back(mGameWorld.CreateGameObject("SphereObj", L"../../Assets/Templates/Objects/mesh_obj.json"));
    TransformComponent* meshTransform = gameObjects.back()->GetComponent<TransformComponent>();
    meshTransform->position.x = 0.0f;
    meshTransform->position.y = 10.0f;


    gameObjects.push_back(mGameWorld.CreateGameObject("ModelObj", L"../../Assets/Templates/Objects/model_obj.json"));
    TransformComponent* modelTransform = gameObjects.back()->GetComponent<TransformComponent>();
    modelTransform->position.z = 2.0f;

    gameObjects.push_back(mGameWorld.CreateGameObject("Ground", L"../../Assets/Templates/Objects/ground.json"));

    for (GameObject* go : gameObjects)
    {
        go->Initialize();
    }

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
}