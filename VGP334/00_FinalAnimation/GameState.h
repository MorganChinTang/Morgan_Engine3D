#pragma once

#include <3DEngine/Inc/3DEngine.h>

#include <array>

class GameState : public Engine3D::AppState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;
    void DebugUI() override;

private:
    struct Scene
    {
        float startTime = 0.0f;
        float endTime = 2.0f;
        float duration = 2.0f;
        bool useCameraRotation = false;
        int clipIndex4 = 0;
        int clipIndex5 = -1;
        int clipIndex6 = -1;
        Engine3D::Math::Vector3 cameraPosition = Engine3D::Math::Vector3::Zero;
        Engine3D::Math::Vector3 cameraLookAt = Engine3D::Math::Vector3::Zero;
        Engine3D::Math::Vector3 cameraRotation = Engine3D::Math::Vector3::Zero;
        Engine3D::Math::Vector3 character4Position = Engine3D::Math::Vector3::Zero;
        Engine3D::Math::Quaternion character4Rotation = Engine3D::Math::Quaternion::Identity;
        float character4Scale = 1.0f;
        Engine3D::Math::Vector3 character5Position = Engine3D::Math::Vector3::Zero;
        Engine3D::Math::Quaternion character5Rotation = Engine3D::Math::Quaternion::Identity;
        float character5Scale = 1.0f;
        Engine3D::Math::Vector3 character6Position = Engine3D::Math::Vector3::Zero;
        Engine3D::Math::Quaternion character6Rotation = Engine3D::Math::Quaternion::Identity;
        float character6Scale = 1.0f;
        Engine3D::Math::Vector3 bombPosition = Engine3D::Math::Vector3::Zero;
        Engine3D::Math::Vector3 bombEndPosition = Engine3D::Math::Vector3::Zero;
        float bombScale = 1.0f;
        Engine3D::Math::Vector3 earthPosition = Engine3D::Math::Vector3::Zero;
        float earthScale = 1.0f;
        Engine3D::Math::Vector3 cactusPosition = Engine3D::Math::Vector3::Zero;
        Engine3D::Math::Quaternion cactusRotation = Engine3D::Math::Quaternion::Identity;
        float cactusScale = 1.0f;
        Engine3D::Math::Vector3 saladPosition = Engine3D::Math::Vector3::Zero;
        Engine3D::Math::Quaternion saladRotation = Engine3D::Math::Quaternion::Identity;
        float saladScale = 1.0f;
    };

    static constexpr int kSceneCount = 8;

    void InitializeScenes();
    void ApplyScene(int sceneIndex);
    void OnPlaySequenceEvent(const Engine3D::Core::Event& e);
    void OnSceneSelectedEvent(const Engine3D::Core::Event& e);

    void UpdateCamera(float deltaTime);

    Engine3D::Graphics::Camera mCamera;
    Engine3D::Graphics::DirectionalLight mDirectionalLight;
    Engine3D::Math::Vector3 mCameraLookAt = Engine3D::Math::Vector3::Zero;

    Engine3D::Graphics::RenderGroup mCharacter4;
    Engine3D::Graphics::RenderGroup mCharacter5;
    Engine3D::Graphics::RenderGroup mCharacter6;
    Engine3D::Graphics::RenderGroup mBomb;
    Engine3D::Graphics::RenderGroup mCactus;
    Engine3D::Graphics::RenderGroup mSalad;
    Engine3D::Graphics::RenderGroup mEarth;

    Engine3D::Graphics::RenderObject mRoomFloor;
    Engine3D::Graphics::RenderObject mRoomWallFront;
    Engine3D::Graphics::RenderObject mRoomWallLeft;
    Engine3D::Graphics::RenderObject mRoomWallRight;
    Engine3D::Graphics::RenderObject mBombSphere;
    Engine3D::Graphics::RenderObject mEarthSphere;
    float mBombScale = 1.0f;
    float mEarthScale = 1.0f;
    float mCactusScale = 1.0f;
    float mSaladScale = 1.0f;

    Engine3D::Graphics::ParticleSystemEffect mBombParticleEffect;
    Engine3D::Physics::ParticleSystem mBombParticleSystem;
    Engine3D::Graphics::ParticleSystemEffect mFireParticleEffect;
    Engine3D::Physics::ParticleSystem mFireParticleSystem;

    Engine3D::Graphics::Animator mAnimator4;
    Engine3D::Graphics::Animator mAnimator5;
    Engine3D::Graphics::Animator mAnimator6;

    Engine3D::Graphics::StandardEffect mStandardEffect;
    std::array<Scene, kSceneCount> mScenes{};
    Engine3D::Core::EventListenerId mPlaySequenceListenerId = 0;
    Engine3D::Core::EventListenerId mSceneSelectedListenerId = 0;

    std::array<Engine3D::Audio::SoundId, kSceneCount> mSceneAudioIds{};

    bool mSequencePlaying = false;
    float mSceneTimer = 0.0f;
    float mPlaybackTimer = 0.0f;
    float mSequenceSpeed = 1.0f;
    int mCurrentSceneIndex = 0;

    bool mDrawSkeleton = false;
    float mAnimationSpeed4 = 1.0f;
    float mAnimationSpeed5 = 1.0f;
    float mAnimationSpeed6 = 1.0f;

    int mClipIndex4 = -1;
    int mClipIndex5 = -1;
    int mClipIndex6 = -1;

};