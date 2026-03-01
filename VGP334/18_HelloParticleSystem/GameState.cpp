#include "GameState.h"
#include "math.h"

using namespace Engine3D;
using namespace Engine3D::Graphics;
using namespace Engine3D::Input;
using namespace Engine3D::Physics;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    mParticleSystemEffect.Initialize();
    mParticleSystemEffect.SetCamera(mCamera);

    ParticleSystemInfo info;
    info.textureId = TextureManager::Get()->LoadTexture("Images/pikachu.png");
    info.maxParticles = 1000;
    info.particlesPerEmit = { 1, 4 };
    info.delay = 1.0f;
    info.lifeTime = FLT_MAX;
    info.timeBetweenEmit = { 0.2f, 0.4f };
    info.spawnAngle = { -30.0f, 30.0f };
    info.spawnSpeed = { 1.0f, 3.0f };
    info.particleLifeTime = { 0.5f, 2.0f };
    info.spawnDirection = Math::Vector3::YAxis;
    info.spawnPosition = Math::Vector3::Zero;
    info.startScale = { Math::Vector3::One, Math::Vector3::One };
    info.endScale = { Math::Vector3::One, Math::Vector3::One };
    info.startColor = { Graphics::Colors::LightPink, Graphics::Colors::LightYellow };
    mParticleSystem.Initialize(info);
}
void GameState::Terminate()
{
    mParticleSystem.Terminate();
    mParticleSystemEffect.Terminate();

}
void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
    mParticleSystem.Update(deltaTime);

}
void GameState::Render()
{
    mParticleSystemEffect.Begin();
    mParticleSystem.Render(mParticleSystemEffect);
    mParticleSystemEffect.End();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    mParticleSystem.DebugUI();
    PhysicsWorld::Get()->DebugUI();
    ImGui::End();
    SimpleDraw::AddGroundPlane(10.0f, Colors::White);
    SimpleDraw::Render(mCamera);
}

void GameState::UpdateCamera(float deltaTime)
{
    InputSystem* input = InputSystem::Get();
    const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
    const float turnSpeed = 0.1f;

    if (input->IsKeyDown(KeyCode::W))
    {
        mCamera.Walk(moveSpeed * deltaTime);
    }
    else if (input->IsKeyDown(KeyCode::S))
    {
        mCamera.Walk(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::D))
    {
        mCamera.Strafe(moveSpeed * deltaTime);
    }
    else if (input->IsKeyDown(KeyCode::A))
    {
        mCamera.Strafe(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::E))
    {
        mCamera.Rise(moveSpeed * deltaTime);
    }
    else if (input->IsKeyDown(KeyCode::Q))
    {
        mCamera.Rise(-moveSpeed * deltaTime);
    }

    if (input->IsMouseDown(MouseButton::RBUTTON))
    {

        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
    }
}