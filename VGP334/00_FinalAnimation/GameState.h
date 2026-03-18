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
        float duration = 2.0f;
        int clipIndex = 0;
        Engine3D::Math::Vector3 cameraPosition = Engine3D::Math::Vector3::Zero;
        Engine3D::Math::Vector3 cameraLookAt = Engine3D::Math::Vector3::Zero;
        Engine3D::Math::Vector3 character4Position = Engine3D::Math::Vector3::Zero;
        Engine3D::Math::Quaternion character4Rotation = Engine3D::Math::Quaternion::Identity;
        Engine3D::Math::Vector3 character5Position = Engine3D::Math::Vector3::Zero;
        Engine3D::Math::Quaternion character5Rotation = Engine3D::Math::Quaternion::Identity;
        Engine3D::Math::Vector3 character6Position = Engine3D::Math::Vector3::Zero;
        Engine3D::Math::Quaternion character6Rotation = Engine3D::Math::Quaternion::Identity;
    };

    static constexpr int kSceneCount = 8;

    void InitializeScenes();
    void ApplyScene(int sceneIndex);
    void OnPlaySequenceEvent(const Engine3D::Core::Event& e);
    void OnSceneSelectedEvent(const Engine3D::Core::Event& e);

    void UpdateCamera(float deltaTime);

    Engine3D::Graphics::Camera mCamera;
    Engine3D::Graphics::DirectionalLight mDirectionalLight;

    Engine3D::Graphics::RenderGroup mCharacter4;
    Engine3D::Graphics::RenderGroup mCharacter5;
    Engine3D::Graphics::RenderGroup mCharacter6;

    Engine3D::Graphics::Animator mAnimator4;
    Engine3D::Graphics::Animator mAnimator5;
    Engine3D::Graphics::Animator mAnimator6;

    Engine3D::Graphics::StandardEffect mStandardEffect;
    std::array<Scene, kSceneCount> mScenes{};
    Engine3D::Core::EventListenerId mPlaySequenceListenerId = 0;
    Engine3D::Core::EventListenerId mSceneSelectedListenerId = 0;

    bool mSequencePlaying = false;
    float mSceneTimer = 0.0f;
    int mCurrentSceneIndex = 0;

    bool mDrawSkeleton = false;
    float mAnimationSpeed4 = 1.0f;
    float mAnimationSpeed5 = 1.0f;
    float mAnimationSpeed6 = 1.0f;

    int mClipIndex4 = -1;
    int mClipIndex5 = -1;
    int mClipIndex6 = -1;

};