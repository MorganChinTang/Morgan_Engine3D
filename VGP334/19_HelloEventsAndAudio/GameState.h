#pragma once

#include <3DEngine/Inc/3DEngine.h>

class GameState : public Engine3D::AppState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;
    void DebugUI() override;

private:

    void UpdateCamera(float deltaTime);

    void OnSpacePressedEvent(const Engine3D::Core::Event& e);

    Engine3D::Graphics::Camera mCamera;
    Engine3D::Graphics::ParticleSystemEffect mParticleSystemEffect;
    Engine3D::Physics::ParticleSystem mParticleSystem;
    Engine3D::Physics::ParticleSystem mFireworkParticle;

    Engine3D::Core::EventListenerId mSpacePressedListenerId = 0;
    Engine3D::Core::EventListenerId mEnterPressedListenerId = 0;

    Engine3D::Graphics::Animation mFireworkAnimation;
    float mFireworkAnimationTime = 0.0f;

    Engine3D::Audio::SoundId mLaunchSoundId = 0;
    Engine3D::Audio::SoundId mExplosionSoundId = 0;
};