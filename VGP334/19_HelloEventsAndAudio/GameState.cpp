#include "GameState.h"
#include "GameEvents.h"
#include "math.h"

using namespace Engine3D;
using namespace Engine3D::Graphics;
using namespace Engine3D::Input;
using namespace Engine3D::Physics;
using namespace Engine3D::Audio;

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
    info.timeBetweenEmit = { 0.1f, 0.2f };
    info.spawnAngle = { -30.0f, 30.0f };
    info.spawnSpeed = { 1.0f, 3.0f };
    info.particleLifeTime = { 0.5f, 2.0f };
    info.spawnDirection = -Math::Vector3::YAxis;
    info.spawnPosition = Math::Vector3::Zero;
    info.startScale = { Math::Vector3::One, Math::Vector3::One };
    info.endScale = { Math::Vector3::Zero, Math::Vector3::Zero };
    info.startColor = { Graphics::Colors::White, Graphics::Colors::White };
    info.endColor = { Graphics::Colors::White, Graphics::Colors::White };
    mParticleSystem.Initialize(info);


    info.textureId = TextureManager::Get()->LoadTexture("Images/bird1.png");
    info.particlesPerEmit = { 10, 30 };
    info.spawnSpeed = { 7.0f, 15.0f };
    info.delay = 0.0f;
    info.lifeTime = 0.0f;
    info.spawnAngle = { -180.0f, 180.0f };
    info.startColor = { Graphics::Colors::Red, Graphics::Colors::Yellow };
    info.endColor = { Graphics::Colors::LightGray, Graphics::Colors::White };
    mFireworkParticle.Initialize(info);

    EventManager* em = EventManager::Get();
    mSpacePressedListenerId = em->AddListener(PressSpaceEvent::StaticGetTypeId(),
        std::bind(&GameState::OnSpacePressedEvent, this, std::placeholders::_1));
    mEnterPressedListenerId = em->AddListener(PressEnterEvent::StaticGetTypeId(),
        [](const Event& e)
        {
            LOG("ENTER WAS PRESSED");
        });

    SoundEffectManager* sm = SoundEffectManager::Get();
    mLaunchSoundId = sm->Load("megamanx_blast.wav");
    mExplosionSoundId = sm->Load("explosion.wav");

    auto launchFirework = [&]()
        {
            SoundEffectManager::Get()->Play(mLaunchSoundId);
        };
    auto spawnFirworks = [&]()
        {
            mFireworkParticle.SpawnParticles();
            SoundEffectManager::Get()->Play(mExplosionSoundId);
        };
    mFireworkAnimation = AnimationBuilder()
        .AddPositionKey(Math::Vector3::Zero, 0.0f)
        .AddPositionKey(Math::Vector3::Zero, 2.0f)
        .AddPositionKey(Math::Vector3::YAxis * 5.0f, 3.0f)
        .AddEventKey(launchFirework, 2.0f)
        .AddEventKey(spawnFirworks, 3.0f)
        .Build();
}
void GameState::Terminate()
{
    SoundEffectManager* sm = SoundEffectManager::Get();
    sm->Stop(mLaunchSoundId);
    sm->Stop(mExplosionSoundId);

    EventManager* em = EventManager::Get();
    em->RemoveListener(PressSpaceEvent::StaticGetTypeId(), mSpacePressedListenerId);
    em->RemoveListener(PressEnterEvent::StaticGetTypeId(), mEnterPressedListenerId);
    mFireworkParticle.Terminate();
    mParticleSystem.Terminate();
    mParticleSystemEffect.Terminate();


}
void GameState::Update(float deltaTime)
{
    SoundEffectManager* sm = SoundEffectManager::Get();
    sm->Stop(mLaunchSoundId);
    sm->Stop(mExplosionSoundId);

    EventManager* em = EventManager::Get();
    em->RemoveListener(PressSpaceEvent::StaticGetTypeId(), mSpacePressedListenerId);
    em->RemoveListener(PressEnterEvent::StaticGetTypeId(), mEnterPressedListenerId);
    mFireworkParticle.Terminate();
    mParticleSystem.Terminate();
    mParticleSystemEffect.Terminate();
}
void GameState::Render()
{
    SimpleDraw::AddGroundPlane(10.0f, Colors::White);
    SimpleDraw::Render(mCamera);

    Transform particleTransform = mFireworkAnimation.GetTransform(mFireworkAnimationTime);
    mParticleSystem.SetPosition(particleTransform.position);
    mFireworkParticle.SetPosition(particleTransform.position);
    mParticleSystemEffect.Begin();
    mParticleSystem.Render(mParticleSystemEffect);
    mFireworkParticle.Render(mParticleSystemEffect);
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

void GameState::OnSpacePressedEvent(const Engine3D::Core::Event& e)
{
    LOG("SPACE WAS PRESSED");
    mFireworkParticle.SpawnParticles();
}
