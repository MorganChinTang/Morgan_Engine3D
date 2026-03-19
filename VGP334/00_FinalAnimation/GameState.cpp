#include "GameState.h"
#include "GameEvents.h"

#include <cmath>
#include <cfloat>
using namespace Engine3D;
using namespace Engine3D::Graphics;
using namespace Engine3D::Input;
using namespace Engine3D::Physics;
using namespace Engine3D::Audio;

//--------------------------------------------------------------------------//
//                                                                          //
//                            IMPORTANT NOTE                                //
//       press SPACE or 1 to play the animation from the beginning          //
//              press 1 - 8 to play a specific scene/short clip             //
//                                                                          //
//                                                                          //
//--------------------------------------------------------------------------//
// This is a recreation of asdfmovie3 by TomSka
namespace
{
    Math::Vector3 QuaternionToEulerDegrees(const Math::Quaternion& q)
    {
        const float sinp = 2.0f * (q.w * q.x - q.z * q.y);
        float pitch = 0.0f;
        if (std::fabs(sinp) >= 1.0f)
        {
            pitch = std::copysign(Math::Constants::HalfPi, sinp);
        }
        else
        {
            pitch = std::asin(sinp);
        }

        const float siny = 2.0f * (q.w * q.y + q.x * q.z);
        const float cosy = 1.0f - 2.0f * (q.y * q.y + q.x * q.x);
        const float yaw = std::atan2(siny, cosy);

        const float sinr = 2.0f * (q.w * q.z + q.x * q.y);
        const float cosr = 1.0f - 2.0f * (q.z * q.z + q.x * q.x);
        const float roll = std::atan2(sinr, cosr);

        return {
            pitch * Math::Constants::RadToDeg,
            yaw * Math::Constants::RadToDeg,
            roll * Math::Constants::RadToDeg
        };
    }

    Math::Quaternion EulerDegreesToQuaternion(const Math::Vector3& degrees)
    {
        const float pitch = degrees.x * Math::Constants::DegToRad;
        const float yaw = degrees.y * Math::Constants::DegToRad;
        const float roll = degrees.z * Math::Constants::DegToRad;
        return Math::Quaternion::CreateFromYawPitchRoll(yaw, pitch, roll);
    }

    Math::Vector3 DirectionToEulerDegrees(const Math::Vector3& direction)
    {
        const Math::Vector3 dir = Math::Normalize(direction);
        const float yaw = std::atan2(dir.x, dir.z);
        const float pitch = std::atan2(-dir.y, std::sqrt(dir.x * dir.x + dir.z * dir.z));
        return {
            pitch * Math::Constants::RadToDeg,
            yaw * Math::Constants::RadToDeg,
            0.0f
        };
    }

    Math::Vector3 EulerDegreesToDirection(const Math::Vector3& degrees)
    {
        const Math::Quaternion rotation = EulerDegreesToQuaternion(degrees);
        const Math::Matrix4 rotationMatrix = Math::Matrix4::MatrixRotationQuaternion(rotation);
        return Math::TransformNormal(Math::Vector3::ZAxis, rotationMatrix);
    }

    void DrawTransformEditor(const char* label, Transform& transform, bool showScale, float* sharedScale = nullptr)
    {
        ImGui::Text("%s", label);
        ImGui::PushID(label);
        ImGui::DragFloat3("Position", &transform.position.x, 0.1f);
        Math::Vector3 rotation = QuaternionToEulerDegrees(transform.rotation);
        if (ImGui::DragFloat3("Rotation (deg)", &rotation.x, 1.0f))
        {
            transform.rotation = EulerDegreesToQuaternion(rotation);
        }
        if (showScale)
        {
            float scaleValue = (sharedScale != nullptr) ? *sharedScale : transform.scale.x;
            if (ImGui::DragFloat("Scale", &scaleValue, 0.05f, 0.001f, 1000.0f))
            {
                if (sharedScale != nullptr)
                {
                    *sharedScale = scaleValue;
                }
            }
            const float finalScale = (sharedScale != nullptr) ? *sharedScale : scaleValue;
            transform.scale = Math::Vector3(finalScale);
        }
        ImGui::PopID();
    }
}

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
    mCameraLookAt = { 0.0f, 0.0f, 0.0f };

    mDirectionalLight.direction = Math::Normalize({ 1.0f,-1.0f,1.0 });
    mDirectionalLight.ambient = { 0.4f,0.4f,0.4f,1.0f };
    mDirectionalLight.diffuse = { 0.7f,0.7f,0.7f,1.0f };
    mDirectionalLight.specular = { 0.9f,0.9f,0.9f,1.0f };

    mCharacter4.Initialize(L"../../Assets/Models/Character04/Character04.model");
    mCharacter5.Initialize(L"../../Assets/Models/Character05/Character05.model");
    mCharacter6.Initialize(L"../../Assets/Models/Character06/Character06.model");

    //mBomb.Initialize("bomb.model");
    mCactus.Initialize("cactus.model");
    mSalad.Initialize("salad.model");

    //mBomb.transform.position = { 1.0f, 0.0f, 1.0f };
    mCactus.transform.position = { 2.0f, 0.0f, 1.0f };
    mSalad.transform.position = { 3.0f, 0.0f, 1.0f };

    mCharacter5.transform.position = { 1.0f, 0.0f, 0.0f };
    mCharacter6.transform.position = { 2.0f, 0.0f, 0.0f };

    ModelManager* mm = ModelManager::Get();
    mCharacter4.Initialize("Character04/Character04.model");
    mCharacter5.Initialize("Character05/Character05.model");
    mCharacter6.Initialize("Character06/Character06.model");


    mCharacter4.animator = &mAnimator4;
    mCharacter5.animator = &mAnimator5;
    mCharacter6.animator = &mAnimator6;

    
    mm->AddAnimation(mCharacter4.modelId, L"../../Assets/Animations/Character04/C4Idle.animset");// animIndex 0
    mm->AddAnimation(mCharacter4.modelId, L"../../Assets/Animations/Character04/C4Talking.animset");// animIndex 1
    mm->AddAnimation(mCharacter4.modelId, L"../../Assets/Animations/Character04/C4Angry.animset");// animIndex 2
    mm->AddAnimation(mCharacter4.modelId, L"../../Assets/Animations/Character04/C4Walking.animset");// animIndex 3
    mm->AddAnimation(mCharacter4.modelId, L"../../Assets/Animations/Character04/C4HoldingSalad.animset");// animIndex 4
    mm->AddAnimation(mCharacter4.modelId, L"../../Assets/Animations/Character04/C4GivingSmt.animset");// animIndex 5
    mm->AddAnimation(mCharacter4.modelId, L"../../Assets/Animations/Character04/C4GettingPunched.animset");// animIndex 6
    mm->AddAnimation(mCharacter4.modelId, L"../../Assets/Animations/Character04/C4GoCrazy.animset");// animIndex 7
    mm->AddAnimation(mCharacter4.modelId, L"../../Assets/Animations/Character04/C4LookingStressed.animset");// animIndex 8
    mm->AddAnimation(mCharacter4.modelId, L"../../Assets/Animations/Character04/C4TakingSmt.animset");// animIndex 9
    mm->AddAnimation(mCharacter4.modelId, L"../../Assets/Animations/Character04/C4TPose.animset");// animIndex 10
    
    mm->AddAnimation(mCharacter5.modelId, L"../../Assets/Animations/Character05/C5HoldingSmt.animset");// animIndex 0
    mm->AddAnimation(mCharacter5.modelId, L"../../Assets/Animations/Character05/C5NeutralIdle.animset");// animIndex 1
    mm->AddAnimation(mCharacter5.modelId, L"../../Assets/Animations/Character05/C5OnTheFloor.animset");// animIndex 2
    mm->AddAnimation(mCharacter5.modelId, L"../../Assets/Animations/Character05/C5PunchingSmt.animset");// animIndex 3
    mm->AddAnimation(mCharacter5.modelId, L"../../Assets/Animations/Character05/C5Talking.animset");// animIndex 4
    mm->AddAnimation(mCharacter5.modelId, L"../../Assets/Animations/Character05/C5TalkingEvil.animset");// animIndex 5
    mm->AddAnimation(mCharacter5.modelId, L"../../Assets/Animations/Character05/C5Walking.animset");// animIndex 6
    mm->AddAnimation(mCharacter6.modelId, L"../../Assets/Animations/Character06/C6Crazy.animset");// animIndex 0

    mAnimator4.Initialize(mCharacter4.modelId);
    mAnimator5.Initialize(mCharacter5.modelId);
    mAnimator6.Initialize(mCharacter6.modelId);

    InitializeScenes();
    ApplyScene(0);
    mSequencePlaying = false;
    mSceneTimer = 0.0f;
    mSequenceSpeed = 1.0f;

    EventManager* em = EventManager::Get();
    mPlaySequenceListenerId = em->AddListener(PlaySequenceEvent::StaticGetTypeId(),
        std::bind(&GameState::OnPlaySequenceEvent, this, std::placeholders::_1));
    mSceneSelectedListenerId = em->AddListener(SceneSelectedEvent::StaticGetTypeId(),
        std::bind(&GameState::OnSceneSelectedEvent, this, std::placeholders::_1));

    SoundEffectManager* sm = SoundEffectManager::Get();
    // comment is length of each scene
    // 0-8 seconds
    mSceneAudioIds[0] = sm->Load("asdfmovie3_1_0-8.wav");
    // 8-13 seconds
    mSceneAudioIds[1] = sm->Load("asdfmovie3_2_8-13.wav");
    // 13-18 seconds
    mSceneAudioIds[2] = sm->Load("asdfmovie3_3_13-18.wav");
    // 18-25 seconds
    mSceneAudioIds[3] = sm->Load("asdfmovie3_4_18-25.wav");
    // 25-29 seconds
    mSceneAudioIds[4] = sm->Load("asdfmovie3_5_25-29.wav");
    // 29-35 seconds
    mSceneAudioIds[5] = sm->Load("asdfmovie3_6_29-35.wav");
    // 35-41 seconds
    mSceneAudioIds[6] = sm->Load("asdfmovie3_7_35-41.wav");
    // 41-46 seconds
    mSceneAudioIds[7] = sm->Load("asdfmovie3_8_41-46.wav");

    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    mBombParticleEffect.Initialize();
    mBombParticleEffect.SetCamera(mCamera);
    ParticleSystemInfo bombParticleInfo;
    bombParticleInfo.textureId = TextureManager::Get()->LoadTexture("FinalAnimation/spark.png");
    bombParticleInfo.maxParticles = 200;
    bombParticleInfo.particlesPerEmit = { 2, 4 };
    bombParticleInfo.delay = 0.0f;
    bombParticleInfo.lifeTime = FLT_MAX;
    bombParticleInfo.timeBetweenEmit = { 0.02f, 0.05f };
    bombParticleInfo.spawnAngle = { -20.0f, 20.0f };
    bombParticleInfo.spawnSpeed = { 0.1f, 0.3f };
    bombParticleInfo.particleLifeTime = { 0.3f, 0.6f };
    bombParticleInfo.spawnDirection = Math::Vector3::YAxis;
    bombParticleInfo.spawnPosition = Math::Vector3::Zero;
    bombParticleInfo.startScale = { Math::Vector3::One * 0.05f, Math::Vector3::One * 0.1f };
    bombParticleInfo.endScale = { Math::Vector3::Zero, Math::Vector3::Zero };
    bombParticleInfo.startColor = { Colors::Yellow, Colors::White };
    bombParticleInfo.endColor = { Colors::White, Colors::White };
    mBombParticleSystem.Initialize(bombParticleInfo);

    mFireParticleEffect.Initialize();
    mFireParticleEffect.SetCamera(mCamera);
    ParticleSystemInfo fireParticleInfo;
    fireParticleInfo.textureId = TextureManager::Get()->LoadTexture("FinalAnimation/fire.png");
    fireParticleInfo.maxParticles = 300;
    fireParticleInfo.particlesPerEmit = { 3, 6 };
    fireParticleInfo.delay = 0.0f;
    fireParticleInfo.lifeTime = FLT_MAX;
    fireParticleInfo.timeBetweenEmit = { 0.01f, 0.03f };
    fireParticleInfo.spawnAngle = { -20.0f, 20.0f };
    fireParticleInfo.spawnSpeed = { 0.1f, 0.25f };
    fireParticleInfo.particleLifeTime = { 0.2f, 0.45f };
    fireParticleInfo.spawnDirection = Math::Vector3::YAxis;
    fireParticleInfo.spawnPosition = Math::Vector3::Zero;
    fireParticleInfo.startScale = { Math::Vector3::One * 0.35f, Math::Vector3::One * 0.7f };
    fireParticleInfo.endScale = { Math::Vector3::One * 0.08f, Math::Vector3::One * 0.16f };
    fireParticleInfo.startColor = { Colors::Yellow, Colors::White };
    fireParticleInfo.endColor = { Colors::White, Colors::White };
    mFireParticleSystem.Initialize(fireParticleInfo);

    constexpr int kRoomSegments = 22;
    constexpr float kRoomSpacing = 1.0f;
    constexpr float kRoomSize = kRoomSegments * kRoomSpacing;
    constexpr float kRoomHalfSize = kRoomSize * 0.5f;

    const Mesh roomFloorMesh = MeshBuilder::CreatePlane(kRoomSegments, kRoomSegments, kRoomSpacing, true);
    const Mesh roomWallMesh = MeshBuilder::CreatePlane(kRoomSegments, kRoomSegments, kRoomSpacing, false);

    auto initializeRoomPlane = [](RenderObject& plane, const Mesh& mesh)
        {
            plane.meshBuffer.Initialize(mesh);
            plane.material = Material{};
            plane.material.emissive = Colors::Black;
            plane.material.ambient = Colors::White;
            plane.material.diffuse = Colors::White;
            plane.material.specular = Colors::Black;
            plane.material.shininess = 1.0f;
            plane.diffuseMapId = 0;
            plane.specMapId = 0;
            plane.normalMapId = 0;
            plane.bumpMapId = 0;
        };

    initializeRoomPlane(mRoomFloor, roomFloorMesh);
    initializeRoomPlane(mRoomWallFront, roomWallMesh);
    initializeRoomPlane(mRoomWallLeft, roomWallMesh);
    initializeRoomPlane(mRoomWallRight, roomWallMesh);

    mRoomFloor.transform.position = { 0.0f, 0.0f, 0.0f };

    mRoomWallFront.transform.position = { 0.0f, kRoomHalfSize, kRoomHalfSize };
    mRoomWallLeft.transform.position = { -kRoomHalfSize, kRoomHalfSize, 0.0f };
    mRoomWallRight.transform.position = { kRoomHalfSize, kRoomHalfSize, 0.0f };

    mRoomWallLeft.transform.rotation = Math::Quaternion::CreateFromAxisAngle(
        Math::Vector3::YAxis, -90.0f * Math::Constants::DegToRad);
    mRoomWallRight.transform.rotation = Math::Quaternion::CreateFromAxisAngle(
        Math::Vector3::YAxis, 90.0f * Math::Constants::DegToRad);

    Mesh bombSphereMesh = MeshBuilder::CreateSphere(20, 20, 0.25f);
    mBombSphere.meshBuffer.Initialize(bombSphereMesh);
    mBombSphere.material = Material{};
    mBombSphere.material.diffuse = Colors::Black;
    mBombSphere.material.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
    mBombSphere.material.specular = Colors::White;
    mBombSphere.material.shininess = 32.0f;
    mBombSphere.diffuseMapId = 0;
    mBombSphere.specMapId = 0;
    mBombSphere.normalMapId = 0;
    mBombSphere.bumpMapId = 0;
    mBombSphere.transform.position = { 1.0f, 0.25f, 1.0f };
    mBombScale = mBombSphere.transform.scale.x;
    mBombSphere.transform.scale = Math::Vector3(mBombScale);

    Mesh earthSphereMesh = MeshBuilder::CreateSphere(30, 30, 0.25f);
    mEarthSphere.meshBuffer.Initialize(earthSphereMesh);
    mEarthSphere.material = Material{};
    mEarthSphere.material.emissive = Colors::Black;
    mEarthSphere.material.ambient = Colors::White;
    mEarthSphere.material.diffuse = Colors::White;
    mEarthSphere.material.specular = Colors::White;
    mEarthSphere.material.shininess = 16.0f;
    mEarthSphere.diffuseMapId = TextureManager::Get()->LoadTexture("planets/earth.jpg");
    mEarthSphere.specMapId = 0;
    mEarthSphere.normalMapId = 0;
    mEarthSphere.bumpMapId = 0;
    mEarthSphere.transform.position = { -1.0f, 0.65f, 0.0f };
    mEarthScale = mEarthSphere.transform.scale.x;
    mEarthSphere.transform.scale = Math::Vector3(mEarthScale);

    mCactus.transform.scale = Math::Vector3(mCactusScale);
    mSalad.transform.scale = Math::Vector3(mSaladScale);

}
void GameState::Terminate()
{
    EventManager* em = EventManager::Get();
    em->RemoveListener(PlaySequenceEvent::StaticGetTypeId(), mPlaySequenceListenerId);
    em->RemoveListener(SceneSelectedEvent::StaticGetTypeId(), mSceneSelectedListenerId);

    SoundEffectManager* sm = SoundEffectManager::Get();
    for (Engine3D::Audio::SoundId soundId : mSceneAudioIds)
    {
        sm->Stop(soundId);
    }

    mCharacter4.Terminate();
    mCharacter5.Terminate();
    mCharacter6.Terminate();
    //mBomb.Terminate();
    mCactus.Terminate();
    mSalad.Terminate();

    mStandardEffect.Terminate();

    mRoomFloor.Terminate();
    mRoomWallFront.Terminate();
    mRoomWallLeft.Terminate();
    mRoomWallRight.Terminate();
    mBombSphere.Terminate();
    mEarthSphere.Terminate();
    mBombParticleSystem.Terminate();
    mBombParticleEffect.Terminate();
    mFireParticleSystem.Terminate();
    mFireParticleEffect.Terminate();
}
void GameState::Update(float deltaTime)
{
    InputSystem* input = InputSystem::Get();
    if (input->IsKeyPressed(KeyCode::SPACE))
    {
        PlaySequenceEvent playEvent;
        EventManager::Broadcast(playEvent);
    }
    if (input->IsKeyPressed(KeyCode::ONE))
    {
        SceneSelectedEvent sceneEvent(0);
        EventManager::Broadcast(sceneEvent);
    }
    if (input->IsKeyPressed(KeyCode::TWO))
    {
        SceneSelectedEvent sceneEvent(1);
        EventManager::Broadcast(sceneEvent);
    }
    if (input->IsKeyPressed(KeyCode::THREE))
    {
        SceneSelectedEvent sceneEvent(2);
        EventManager::Broadcast(sceneEvent);
    }
    if (input->IsKeyPressed(KeyCode::FOUR))
    {
        SceneSelectedEvent sceneEvent(3);
        EventManager::Broadcast(sceneEvent);
    }
    if (input->IsKeyPressed(KeyCode::FIVE))
    {
        SceneSelectedEvent sceneEvent(4);
        EventManager::Broadcast(sceneEvent);
    }
    if (input->IsKeyPressed(KeyCode::SIX))
    {
        SceneSelectedEvent sceneEvent(5);
        EventManager::Broadcast(sceneEvent);
    }
    if (input->IsKeyPressed(KeyCode::SEVEN))
    {
        SceneSelectedEvent sceneEvent(6);
        EventManager::Broadcast(sceneEvent);
    }
    if (input->IsKeyPressed(KeyCode::EIGHT))
    {
        SceneSelectedEvent sceneEvent(7);
        EventManager::Broadcast(sceneEvent);
    }

    if (mSequencePlaying)
    {
        const float sequenceDeltaTime = deltaTime * mSequenceSpeed;
        mSceneTimer += sequenceDeltaTime;
        int nextScene = mCurrentSceneIndex;
        for (int i = 0; i < kSceneCount; ++i)
        {
            if (mSceneTimer >= mScenes[i].startTime && mSceneTimer < mScenes[i].endTime)
            {
                nextScene = i;
                break;
            }
        }

        if (nextScene != mCurrentSceneIndex)
        {
            ApplyScene(nextScene);
        }

        if (mSceneTimer >= mScenes[kSceneCount - 1].endTime)
        {
            mSequencePlaying = false;
            mSceneTimer = 0.0f;
        }
    }

    const Scene& scene0 = mScenes[0];
    if (mSceneTimer >= scene0.startTime && mSceneTimer < scene0.endTime)
    {
        // 0-2s: bomb lerp start -> end
        if (mSceneTimer <= scene0.startTime + 2.0f)
        {
            const float tBomb = Math::Clamp((mSceneTimer - scene0.startTime) / 2.0f, 0.0f, 1.0f);
            const Math::Vector3 bombPosition = Math::Lerp(scene0.bombPosition, scene0.bombEndPosition, tBomb);
            mBombSphere.transform.position = bombPosition;
            mBombParticleSystem.SetPosition(bombPosition);
        }
        // 2-7s: bomb stays at end position
        else if (mSceneTimer < scene0.startTime + 7.0f)
        {
            mBombSphere.transform.position = scene0.bombEndPosition;
            mBombParticleSystem.SetPosition(scene0.bombEndPosition);
        }
        // 7-8s: bomb lerp end -> start
        else
        {
            const float tBombBack = Math::Clamp((mSceneTimer - (scene0.startTime + 7.0f)) / 1.0f, 0.0f, 1.0f);
            const Math::Vector3 bombPosition = Math::Lerp(scene0.bombEndPosition, scene0.bombPosition, tBombBack);
            mBombSphere.transform.position = bombPosition;
            mBombParticleSystem.SetPosition(bombPosition);
        }

        // 2-6s: Character4 position1 -> position2(-2, 0, 0), C4Walking; Character5 C5HoldingSmt
        if (mSceneTimer >= scene0.startTime + 2.0f && mSceneTimer <= scene0.startTime + 6.0f)
        {
            const float tForward = Math::Clamp((mSceneTimer - (scene0.startTime + 2.0f)) / 4.0f, 0.0f, 1.0f);
            mCharacter4.transform.position = Math::Lerp(Math::Vector3(0.0f, 0.0f, 0.0f), Math::Vector3(-2.0f, 0.0f, 0.0f), tForward);

            if (mClipIndex4 != 3)
            {
                mClipIndex4 = 3;
                mAnimator4.PlayAnimation(mClipIndex4, true);
            }
            if (mClipIndex5 != 0)
            {
                mClipIndex5 = 0;
                mAnimator5.PlayAnimation(mClipIndex5, true);
            }
        }
        // 6-7s: Character4 position2 -> position1
        else if (mSceneTimer > scene0.startTime + 6.0f && mSceneTimer <= scene0.startTime + 7.0f)
        {
            const float tBack = Math::Clamp((mSceneTimer - (scene0.startTime + 6.0f)) / 1.0f, 0.0f, 1.0f);
            mCharacter4.transform.position = Math::Lerp(Math::Vector3(-2.0f, 0.0f, 0.0f), Math::Vector3(0.0f, 0.0f, 0.0f), tBack);

            if (mClipIndex4 != 3)
            {
                mClipIndex4 = 3;
                mAnimator4.PlayAnimation(mClipIndex4, true);
            }
        }
        else if (mSceneTimer >= scene0.startTime + 7.0f && mSceneTimer < scene0.endTime)
        {
            mCharacter4.transform.position = Math::Vector3(0.0f, 0.0f, 0.0f);

            if (mClipIndex4 != 9)
            {
                mClipIndex4 = 9;
                mAnimator4.PlayAnimation(mClipIndex4, true);
            }
        }
    }
    else
    {
        mBombSphere.transform.position = scene0.bombPosition;
        mBombScale = -0.001f;
        mBombSphere.transform.scale = Math::Vector3(mBombScale);
    }

    const Scene& scene1 = mScenes[1];
    if (mSceneTimer >= scene1.startTime && mSceneTimer < scene1.startTime + 1.0f)
    {
        // 8-9s: Character5 lerp from (-2,0,0) to (0,0,0)
        const float tCharacter5 = Math::Clamp((mSceneTimer - scene1.startTime) / 1.0f, 0.0f, 1.0f);
        mCharacter5.transform.position = Math::Lerp(Math::Vector3(-2.0f, 0.0f, 0.0f), Math::Vector3(0.0f, 0.0f, 0.0f), tCharacter5);
        if (mClipIndex5 != 6)
        {
            mClipIndex5 = 6;
            mAnimator5.PlayAnimation(mClipIndex5, true);
        }
    }
    else if (mSceneTimer >= scene1.startTime + 1.0f && mSceneTimer < scene1.endTime)
    {
        mCharacter5.transform.position = Math::Vector3(0.0f, 0.0f, 0.0f);
        if (mClipIndex4 != 8)
        {
            mClipIndex4 = 8;
            mAnimator4.PlayAnimation(mClipIndex4, true);
        }
        if (mClipIndex5 != 3)
        {
            mClipIndex5 = 3;
            mAnimator5.PlayAnimation(mClipIndex5, true);
        }
    }

    const Scene& scene2 = mScenes[2];
    if (mSceneTimer >= scene2.startTime && mSceneTimer < scene2.startTime + 2.0f)
    {
        // 13-15s: Character4 clip 2, Character5 clip 1
        if (mClipIndex4 != 2)
        {
            mClipIndex4 = 2;
            mAnimator4.PlayAnimation(mClipIndex4, true);
        }
        if (mClipIndex5 != 1)
        {
            mClipIndex5 = 1;
            mAnimator5.PlayAnimation(mClipIndex5, true);
        }
    }
    else if (mSceneTimer >= scene2.startTime + 2.0f && mSceneTimer < scene2.endTime)
    {
        // 15-18s: Character4 lerp up, clip 10; Character5 clip 1
        const float tCharacter4 = Math::Clamp((mSceneTimer - (scene2.startTime + 2.0f)) / 3.0f, 0.0f, 1.0f);
        mCharacter4.transform.position = Math::Lerp(Math::Vector3(0.0f, 0.0f, 0.0f), Math::Vector3(0.0f, 2.6f, 0.0f), tCharacter4);

        if (mClipIndex4 != 10)
        {
            mClipIndex4 = 10;
            mAnimator4.PlayAnimation(mClipIndex4, true);
        }
        if (mClipIndex5 != 1)
        {
            mClipIndex5 = 1;
            mAnimator5.PlayAnimation(mClipIndex5, true);
        }
    }

    const Scene& scene3 = mScenes[3];
    if (mSceneTimer >= scene3.startTime && mSceneTimer < scene3.startTime + 1.0f)
    {
        // 18-19s: Character4 clip 8, Earth hidden
        mCharacter4.transform.position = Math::Vector3(0.0f, 0.0f, 0.0f);
        mCharacter4.transform.rotation = EulerDegreesToQuaternion({ 0.0f, -77.0f, 0.0f });
        mEarthScale = -0.001f;
        mEarthSphere.transform.scale = Math::Vector3(mEarthScale);
        if (mClipIndex4 != 8)
        {
            mClipIndex4 = 8;
            mAnimator4.PlayAnimation(mClipIndex4, true);
        }
    }
    else if (mSceneTimer >= scene3.startTime + 1.0f && mSceneTimer < scene3.endTime)
    {
        // 19-25s: new camera, Character4 clip 8, Earth scale 12.95
        mCamera.SetPosition({ -1.95f, 2.21f, -4.29f });
        const Math::Vector3 direction = EulerDegreesToDirection({ -23.0f, 176.7f, 0.0f });
        mCamera.SetDirection(direction);
        mCameraLookAt = mCamera.GetPosition() + direction;
        mCharacter4.transform.position = Math::Vector3(0.0f, 0.0f, 0.0f);
        mCharacter4.transform.rotation = EulerDegreesToQuaternion({ 0.0f, -77.0f, 0.0f });
        mEarthSphere.transform.position = { -1.3f, 5.75f, -11.6f };
        mEarthSphere.transform.rotation = EulerDegreesToQuaternion({ -21.0f, -108.0f, -55.0f });
        mEarthScale = 12.95f;
        mEarthSphere.transform.scale = Math::Vector3(mEarthScale);
        if (mClipIndex4 != 8)
        {
            mClipIndex4 = 8;
            mAnimator4.PlayAnimation(mClipIndex4, true);
        }
    }

    const Scene& scene4 = mScenes[4];
    if (mSceneTimer >= scene4.startTime && mSceneTimer < scene4.startTime + 2.0f)
    {
        // 25-27s: Character4 clip 0, Character5 clip 4, Earth hidden
        mCamera.SetPosition({ 0.79f, 0.91f, -1.00f });
        const Math::Vector3 direction = EulerDegreesToDirection({ 6.0f, -8.0f, 0.0f });
        mCamera.SetDirection(direction);
        mCameraLookAt = mCamera.GetPosition() + direction;
        mCharacter4.transform.position = Math::Vector3(0.0f, 0.0f, 0.0f);
        mCharacter4.transform.rotation = EulerDegreesToQuaternion({ 0.0f, -77.0f, 0.0f });
        mCharacter5.transform.position = { 1.0f, 0.0f, -0.1f };
        mCharacter5.transform.rotation = EulerDegreesToQuaternion({ 0.0f, 77.0f, 0.0f });
        mEarthScale = -0.001f;
        mEarthSphere.transform.scale = Math::Vector3(mEarthScale);
        if (mClipIndex4 != 0)
        {
            mClipIndex4 = 0;
            mAnimator4.PlayAnimation(mClipIndex4, true);
        }
        if (mClipIndex5 != 4)
        {
            mClipIndex5 = 4;
            mAnimator5.PlayAnimation(mClipIndex5, true);
        }
    }
    else if (mSceneTimer >= scene4.startTime + 2.0f && mSceneTimer < scene4.startTime + 3.0f)
    {
        // 27-28s: Character4 clip 6, Character5 clip 3
        mCamera.SetPosition({ 0.79f, 0.91f, -1.00f });
        const Math::Vector3 direction = EulerDegreesToDirection({ 6.0f, -8.0f, 0.0f });
        mCamera.SetDirection(direction);
        mCameraLookAt = mCamera.GetPosition() + direction;
        mCharacter4.transform.position = Math::Vector3(0.0f, 0.0f, 0.0f);
        mCharacter4.transform.rotation = EulerDegreesToQuaternion({ 0.0f, -77.0f, 0.0f });
        mCharacter5.transform.position = { 1.0f, 0.0f, -0.1f };
        mCharacter5.transform.rotation = EulerDegreesToQuaternion({ 0.0f, 77.0f, 0.0f });
        mEarthScale = -0.001f;
        mEarthSphere.transform.scale = Math::Vector3(mEarthScale);
        if (mClipIndex4 != 6)
        {
            mClipIndex4 = 6;
            mAnimator4.PlayAnimation(mClipIndex4, true);
        }
        if (mClipIndex5 != 3)
        {
            mClipIndex5 = 3;
            mAnimator5.PlayAnimation(mClipIndex5, true);
        }
    }
    else if (mSceneTimer >= scene4.startTime + 3.0f && mSceneTimer < scene4.endTime)
    {
        // 28-29s: new camera, Character4 hidden, Character5 clip 5
        mCamera.SetPosition({ 0.52f, 0.9f, -0.46f });
        const Math::Vector3 direction = EulerDegreesToDirection({ 4.2f, 46.2f, 0.0f });
        mCamera.SetDirection(direction);
        mCameraLookAt = mCamera.GetPosition() + direction;
        mCharacter4.transform.position = Math::Vector3(0.0f, 0.0f, 0.0f);
        mCharacter4.transform.rotation = EulerDegreesToQuaternion({ 0.0f, -77.0f, 0.0f });
        mCharacter5.transform.position = { 1.0f, 0.0f, -0.1f };
        mCharacter5.transform.rotation = EulerDegreesToQuaternion({ 0.0f, 77.0f, 0.0f });
        mEarthScale = -0.001f;
        mEarthSphere.transform.scale = Math::Vector3(mEarthScale);
        if (mClipIndex4 != 0)
        {
            mClipIndex4 = 0;
            mAnimator4.PlayAnimation(mClipIndex4, true);
        }
        if (mClipIndex5 != 5)
        {
            mClipIndex5 = 5;
            mAnimator5.PlayAnimation(mClipIndex5, true);
        }
    }

    const Scene& scene5 = mScenes[5];
    if (mSceneTimer >= scene5.startTime && mSceneTimer < scene5.endTime)
    {
        // 29-35s: Character4 and Character5 swapped, Salad visible
        mCamera.SetPosition({ 0.79f, 0.91f, -1.00f });
        const Math::Vector3 direction = EulerDegreesToDirection({ 6.0f, -8.0f, 0.0f });
        mCamera.SetDirection(direction);
        mCameraLookAt = mCamera.GetPosition() + direction;
        mCharacter4.transform.position = { 1.0f, 0.0f, -0.1f };
        mCharacter4.transform.rotation = EulerDegreesToQuaternion({ 0.0f, 77.0f, 0.0f });
        mCharacter5.transform.position = Math::Vector3(0.0f, 0.0f, 0.0f);
        mCharacter5.transform.rotation = EulerDegreesToQuaternion({ 0.0f, -77.0f, 0.0f });
        mSalad.transform.position = { 0.6f, 0.6f, -0.2f };
        mSalad.transform.rotation = Math::Quaternion::Identity;
        mSaladScale = 2.0f;
        mSalad.transform.scale = Math::Vector3(mSaladScale);
        mEarthScale = -0.001f;
        mEarthSphere.transform.scale = Math::Vector3(mEarthScale);
        if (mClipIndex4 != 8)
        {
            mClipIndex4 = 8;
            mAnimator4.PlayAnimation(mClipIndex4, true);
        }
        if (mClipIndex5 != 3)
        {
            mClipIndex5 = 3;
            mAnimator5.PlayAnimation(mClipIndex5, true);
        }
    }

    const Scene& scene6 = mScenes[6];
    if (mSceneTimer >= scene6.startTime && mSceneTimer < scene6.startTime + 3.0f)
    {
        // 35-38s
        mCamera.SetPosition({ 0.21f, 0.82f, -1.53f });
        const Math::Vector3 direction = EulerDegreesToDirection({ 10.2f, 2.7f, 0.0f });
        mCamera.SetDirection(direction);
        mCameraLookAt = mCamera.GetPosition() + direction;
        mCharacter4.transform.position = { 1.3f, 0.0f, 0.0f };
        mCharacter4.transform.rotation = EulerDegreesToQuaternion({ 0.0f, 68.0f, 0.0f });
        mCharacter5.transform.position = { 0.8f, 0.0f, -0.1f };
        mCharacter5.transform.rotation = EulerDegreesToQuaternion({ 0.0f, 66.0f, 0.0f });
        mCharacter5.transform.scale = Math::Vector3(0.5f);
        mCactus.transform.position = { -0.6f, -0.8f, -0.3f };
        mCactus.transform.rotation = EulerDegreesToQuaternion({ 2.0f, 0.0f, 0.0f });
        mCactusScale = 3.1f;
        mCactus.transform.scale = Math::Vector3(mCactusScale);
        mEarthScale = -0.001f;
        mEarthSphere.transform.scale = Math::Vector3(mEarthScale);
        if (mClipIndex4 != 1)
        {
            mClipIndex4 = 1;
            mAnimator4.PlayAnimation(mClipIndex4, true);
        }
        if (mClipIndex5 != 0)
        {
            mClipIndex5 = 0;
            mAnimator5.PlayAnimation(mClipIndex5, true);
        }
    }
    else if (mSceneTimer >= scene6.startTime + 3.0f && mSceneTimer < scene6.startTime + 4.0f)
    {
        // 38-39s
        mCamera.SetPosition({ 0.21f, 0.82f, -1.53f });
        const Math::Vector3 direction = EulerDegreesToDirection({ 10.2f, 2.7f, 0.0f });
        mCamera.SetDirection(direction);
        mCameraLookAt = mCamera.GetPosition() + direction;
        mCharacter4.transform.position = { 1.3f, 0.0f, 0.0f };
        mCharacter4.transform.rotation = EulerDegreesToQuaternion({ 0.0f, 68.0f, 0.0f });
        const float tCharacter5 = Math::Clamp((mSceneTimer - (scene6.startTime + 3.0f)) / 1.0f, 0.0f, 1.0f);
        mCharacter5.transform.position = Math::Lerp(
            Math::Vector3(0.8f, 0.0f, -0.1f),
            Math::Vector3(-0.4f, 0.2f, -0.3f),
            tCharacter5);
        mCharacter5.transform.rotation = EulerDegreesToQuaternion({ 1.0f, -79.0f, 2.0f });
        mCharacter5.transform.scale = Math::Vector3(0.5f);
        mCactus.transform.position = { -0.6f, -0.8f, -0.3f };
        mCactus.transform.rotation = EulerDegreesToQuaternion({ 2.0f, 0.0f, 0.0f });
        mCactusScale = 3.1f;
        mCactus.transform.scale = Math::Vector3(mCactusScale);
        mEarthScale = -0.001f;
        mEarthSphere.transform.scale = Math::Vector3(mEarthScale);
        if (mClipIndex4 != 0)
        {
            mClipIndex4 = 0;
            mAnimator4.PlayAnimation(mClipIndex4, true);
        }
        if (mClipIndex5 != -1)
        {
            mClipIndex5 = -1;
            mAnimator5.PlayAnimation(mClipIndex5, true);
        }
    }
    else if (mSceneTimer >= scene6.startTime + 4.0f && mSceneTimer < scene6.endTime)
    {
        // 39-41s
        mCamera.SetPosition({ 0.82f, 0.63f, -0.2f });
        const Math::Vector3 direction = EulerDegreesToDirection({ -22.4f, 62.2f, 0.0f });
        mCamera.SetDirection(direction);
        mCameraLookAt = mCamera.GetPosition() + direction;
        mCharacter4.transform.position = { 1.3f, 0.0f, 0.0f };
        mCharacter4.transform.rotation = EulerDegreesToQuaternion({ 0.0f, 68.0f, 0.0f });
        if (mClipIndex4 != 2)
        {
            mClipIndex4 = 2;
            mAnimator4.PlayAnimation(mClipIndex4, true);
        }
    }

    const Scene& scene7 = mScenes[7];
    if (mSceneTimer >= scene7.startTime && mSceneTimer < scene7.startTime + 2.0f)
    {
        // 41-43s
        mCamera.SetPosition({ 0.42f, 0.82f, -0.51f });
        const Math::Vector3 direction = EulerDegreesToDirection({ 4.1f, -32.9f, 0.0f });
        mCamera.SetDirection(direction);
        mCameraLookAt = mCamera.GetPosition() + direction;
        mCharacter4.transform.position = Math::Vector3(0.0f, 0.0f, 0.0f);
        mCharacter4.transform.rotation = EulerDegreesToQuaternion({ 0.0f, -77.0f, 0.0f });
        mCharacter4.transform.scale = Math::Vector3(1.0f);
        mCharacter5.transform.position = { 1.0f, 0.0f, -0.1f };
        mCharacter5.transform.rotation = EulerDegreesToQuaternion({ 0.0f, 60.0f, 0.0f });
        mCharacter5.transform.scale = Math::Vector3(0.5f);
        mCharacter6.transform.scale = Math::Vector3(-0.001f);
        mBombScale = -0.001f;
        mBombSphere.transform.scale = Math::Vector3(mBombScale);
        mSaladScale = -0.001f;
        mSalad.transform.scale = Math::Vector3(mSaladScale);
        mCactusScale = -0.001f;
        mCactus.transform.scale = Math::Vector3(mCactusScale);
        mEarthScale = -0.001f;
        mEarthSphere.transform.scale = Math::Vector3(mEarthScale);
        mFireParticleSystem.SetPosition(mCharacter5.transform.position);
        if (mClipIndex4 != 1)
        {
            mClipIndex4 = 1;
            mAnimator4.PlayAnimation(mClipIndex4, true);
        }
        if (mClipIndex5 != 2)
        {
            mClipIndex5 = 2;
            mAnimator5.PlayAnimation(mClipIndex5, true);
        }
    }
    else if (mSceneTimer >= scene7.startTime + 2.0f && mSceneTimer < scene7.startTime + 3.0f)
    {
        // 43-44s
        mCamera.SetPosition({ 0.15f, 1.16f, -0.92f });
        const Math::Vector3 direction = EulerDegreesToDirection({ 34.9f, 27.8f, 0.0f });
        mCamera.SetDirection(direction);
        mCameraLookAt = mCamera.GetPosition() + direction;
        mCharacter4.transform.position = Math::Vector3(0.0f, 0.0f, 0.0f);
        mCharacter4.transform.rotation = EulerDegreesToQuaternion({ 0.0f, -77.0f, 0.0f });
        mCharacter4.transform.scale = Math::Vector3(1.0f);
        mCharacter5.transform.position = { 1.0f, 0.0f, -0.1f };
        mCharacter5.transform.rotation = EulerDegreesToQuaternion({ 0.0f, 60.0f, 0.0f });
        mCharacter5.transform.scale = Math::Vector3(0.5f);
        mCharacter6.transform.scale = Math::Vector3(-0.001f);
        mBombScale = -0.001f;
        mBombSphere.transform.scale = Math::Vector3(mBombScale);
        mSaladScale = -0.001f;
        mSalad.transform.scale = Math::Vector3(mSaladScale);
        mCactusScale = -0.001f;
        mCactus.transform.scale = Math::Vector3(mCactusScale);
        mEarthScale = -0.001f;
        mEarthSphere.transform.scale = Math::Vector3(mEarthScale);
        mFireParticleSystem.SetPosition(mCharacter5.transform.position);
        if (mClipIndex4 != 0)
        {
            mClipIndex4 = 0;
            mAnimator4.PlayAnimation(mClipIndex4, true);
        }
        if (mClipIndex5 != 2)
        {
            mClipIndex5 = 2;
            mAnimator5.PlayAnimation(mClipIndex5, true);
        }
    }
    else if (mSceneTimer >= scene7.startTime + 3.0f && mSceneTimer < scene7.endTime)
    {
        // 44-46s
        mCamera.SetPosition({ 1.1f, 1.87f, -2.14f });
        const Math::Vector3 direction = EulerDegreesToDirection({ 25.2f, 42.5f, 0.0f });
        mCamera.SetDirection(direction);
        mCameraLookAt = mCamera.GetPosition() + direction;
        mCharacter4.transform.position = Math::Vector3(0.0f, 0.0f, 0.0f);
        mCharacter4.transform.rotation = EulerDegreesToQuaternion({ 0.0f, -77.0f, 0.0f });
        mCharacter4.transform.scale = Math::Vector3(1.0f);
        mCharacter5.transform.position = { 1.0f, 0.0f, -0.1f };
        mCharacter5.transform.rotation = EulerDegreesToQuaternion({ 0.0f, 60.0f, 0.0f });
        mCharacter5.transform.scale = Math::Vector3(0.5f);
        mCharacter6.transform.position = { 3.1f, 0.0f, -0.7f };
        mCharacter6.transform.rotation = EulerDegreesToQuaternion({ 0.0f, 71.0f, 0.0f });
        mCharacter6.transform.scale = Math::Vector3(1.0f);
        mBombScale = -0.001f;
        mBombSphere.transform.scale = Math::Vector3(mBombScale);
        mSaladScale = -0.001f;
        mSalad.transform.scale = Math::Vector3(mSaladScale);
        mCactusScale = -0.001f;
        mCactus.transform.scale = Math::Vector3(mCactusScale);
        mEarthScale = -0.001f;
        mEarthSphere.transform.scale = Math::Vector3(mEarthScale);
        mFireParticleSystem.SetPosition(mCharacter5.transform.position);
        if (mClipIndex4 != 0)
        {
            mClipIndex4 = 0;
            mAnimator4.PlayAnimation(mClipIndex4, true);
        }
        if (mClipIndex5 != 2)
        {
            mClipIndex5 = 2;
            mAnimator5.PlayAnimation(mClipIndex5, true);
        }
        if (mClipIndex6 != 0)
        {
            mClipIndex6 = 0;
            mAnimator6.PlayAnimation(mClipIndex6, true);
        }
    }

    UpdateCamera(deltaTime);
    const float animationDeltaTime = mSequencePlaying ? deltaTime : 0.0f;
    mAnimator4.Update(animationDeltaTime * mAnimationSpeed4);
    mAnimator5.Update(animationDeltaTime * mAnimationSpeed5);
    mAnimator6.Update(animationDeltaTime * mAnimationSpeed6);
    if (mSequencePlaying && mSceneTimer < scene0.endTime)
    {
        mBombParticleSystem.Update(deltaTime);
    }
    if (mSequencePlaying && mSceneTimer >= scene7.startTime && mSceneTimer < scene7.endTime)
    {
        mFireParticleSystem.Update(deltaTime);
    }

}

void GameState::Render()
{
    if (mDrawSkeleton)
    {
        AnimationUtil::BoneTransforms boneTransforms4;
        AnimationUtil::ComputeBoneTransforms(mCharacter4.modelId, boneTransforms4, &mAnimator4);
        AnimationUtil::DrawSkeleton(mCharacter4.modelId, boneTransforms4);

        AnimationUtil::BoneTransforms boneTransforms5;
        AnimationUtil::ComputeBoneTransforms(mCharacter5.modelId, boneTransforms5, &mAnimator5);
        AnimationUtil::DrawSkeleton(mCharacter5.modelId, boneTransforms5);

        AnimationUtil::BoneTransforms boneTransforms6;
        AnimationUtil::ComputeBoneTransforms(mCharacter6.modelId, boneTransforms6, &mAnimator6);
        AnimationUtil::DrawSkeleton(mCharacter6.modelId, boneTransforms6);

        //SimpleDraw::AddGroundPlane(20.0f, Colors::DarkGray);
        SimpleDraw::Render(mCamera);
        
    }
    else
    {
        //SimpleDraw::AddGroundPlane(20.0f, Colors::DarkGray);
        SimpleDraw::Render(mCamera);

        mStandardEffect.Begin();
        mStandardEffect.Render(mRoomFloor);
        mStandardEffect.Render(mRoomWallFront);
        mStandardEffect.Render(mRoomWallLeft);
        mStandardEffect.Render(mRoomWallRight);
        mStandardEffect.Render(mBombSphere);
        mStandardEffect.Render(mEarthSphere);
        mStandardEffect.Render(mCharacter4);
        mStandardEffect.Render(mCharacter5);
        mStandardEffect.Render(mCharacter6);
        //mStandardEffect.Render(mBomb);
        mStandardEffect.Render(mCactus);
        mStandardEffect.Render(mSalad);

        mStandardEffect.End();
    }

    if (mSequencePlaying && mSceneTimer < mScenes[0].endTime)
    {
        mBombParticleEffect.Begin();
        mBombParticleSystem.Render(mBombParticleEffect);
        mBombParticleEffect.End();
    }
    if (mSequencePlaying && mSceneTimer >= mScenes[7].startTime && mSceneTimer < mScenes[7].endTime)
    {
        mFireParticleEffect.Begin();
        mFireParticleSystem.Render(mFireParticleEffect);
        mFireParticleEffect.End();
    }
   
}

void GameState::DebugUI()
{
    ImGui::SetNextWindowCollapsed(true, ImGuiCond_FirstUseEver);
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
    {
        const Math::Vector3& cameraPosition = mCamera.GetPosition();
        const Math::Vector3 cameraRotation = DirectionToEulerDegrees(mCamera.GetDirection());
        ImGui::Text("Position: %.2f, %.2f, %.2f", cameraPosition.x, cameraPosition.y, cameraPosition.z);
        ImGui::Text("LookAt: %.2f, %.2f, %.2f", mCameraLookAt.x, mCameraLookAt.y, mCameraLookAt.z);
        ImGui::Text("Rotation (Pitch/Yaw/Roll): %.1f, %.1f, %.1f", cameraRotation.x, cameraRotation.y, cameraRotation.z);
    }
    //if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
    //{
    //    if (ImGui::DragFloat3("DirectionalLight", &mDirectionalLight.direction.x, 0.01f))
    //    {
    //        mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
    //    }

    //    ImGui::ColorEdit4("Ambient#Light", &mDirectionalLight.ambient.r);
    //    ImGui::ColorEdit4("Diffuse#Light", &mDirectionalLight.diffuse.r);
    //    ImGui::ColorEdit4("Specular#Light", &mDirectionalLight.specular.r);
    //}

    //if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
    //{
    //    for (uint32_t i = 0; i < mCharacter4.renderObjects.size(); i++)
    //    {
    //        Material& material = mCharacter4.renderObjects[i].material;
    //        std::string renderObjectId = "RenderObject" + std::to_string(i);
    //        ImGui::PushID(renderObjectId.c_str());
    //        if (ImGui::CollapsingHeader("RenderObject"))
    //        {
    //            ImGui::LabelText("Label", "Material: ");
    //            ImGui::ColorEdit4("Emissive#Material", &material.emissive.r);
    //            ImGui::ColorEdit4("Ambient#Material", &material.ambient.r);
    //            ImGui::ColorEdit4("Diffuse#Material", &material.diffuse.r);
    //            ImGui::ColorEdit4("Specular#Material", &material.specular.r);
    //            ImGui::DragFloat("Shininess#Material", &material.shininess, 0.01f, 0.01f, 1000.0f);
    //        }
    //        ImGui::PopID();
    //    }
    //}

    if (ImGui::CollapsingHeader("Characters", ImGuiTreeNodeFlags_DefaultOpen))
    {
        DrawTransformEditor("Character 4", mCharacter4.transform, true);
        ImGui::Separator();
        DrawTransformEditor("Character 5", mCharacter5.transform, true);
        ImGui::Separator();
        DrawTransformEditor("Character 6", mCharacter6.transform, true);
    }

    if (ImGui::CollapsingHeader("Props", ImGuiTreeNodeFlags_DefaultOpen))
    {
        //ImGui::Text("Bomb meshes: %u", static_cast<uint32_t>(mBomb.renderObjects.size()));
        //ImGui::DragFloat3("Bomb Position", &mBomb.transform.position.x, 0.01f);
        //ImGui::DragFloat3("Bomb Scale", &mBomb.transform.scale.x, 0.01f, 0.001f, 1000.0f);

        DrawTransformEditor("Bomb Sphere", mBombSphere.transform, true, &mBombScale);

        ImGui::Separator();
        DrawTransformEditor("Earth Sphere", mEarthSphere.transform, true, &mEarthScale);

        ImGui::Separator();
        ImGui::Text("Cactus meshes: %u", static_cast<uint32_t>(mCactus.renderObjects.size()));
        DrawTransformEditor("Cactus", mCactus.transform, true, &mCactusScale);

        ImGui::Separator();
        ImGui::Text("Salad meshes: %u", static_cast<uint32_t>(mSalad.renderObjects.size()));
        DrawTransformEditor("Salad", mSalad.transform, true, &mSaladScale);
    }

    ImGui::Checkbox("DrawSkeleton", &mDrawSkeleton);
    ImGui::DragFloat("Sequence Speed", &mSequenceSpeed, 0.01f, 0.0f, 5.0f);
    ImGui::DragFloat("AnimSpeed4", &mAnimationSpeed4, 0.01f, 0.0f, 10.0f);
    int maxAnimations4 = mAnimator4.GetAnimationCount();
    if (ImGui::DragInt("AnimIndex4", &mClipIndex4, 1, -1, maxAnimations4 - 1))
    {
        mAnimator4.PlayAnimation(mClipIndex4, true);
    }

    ImGui::DragFloat("AnimSpeed5", &mAnimationSpeed5, 0.01f, 0.0f, 10.0f);
    int maxAnimations5 = mAnimator5.GetAnimationCount();
    if (ImGui::DragInt("AnimIndex5", &mClipIndex5, 1, -1, maxAnimations5 - 1))
    {
        mAnimator5.PlayAnimation(mClipIndex5, true);
    }

    ImGui::DragFloat("AnimSpeed6", &mAnimationSpeed6, 0.01f, 0.0f, 10.0f);
    int maxAnimations6 = mAnimator6.GetAnimationCount();
    if (ImGui::DragInt("AnimIndex6", &mClipIndex6, 1, -1, maxAnimations6 - 1))
    {
        mAnimator6.PlayAnimation(mClipIndex6, true);
    }

    // mStandardEffect.DebugUI();
    ImGui::End();
}

void GameState::InitializeScenes()
{
    for (int i = 0; i < kSceneCount; ++i)
    {
        Scene& scene = mScenes[i];
        scene.duration = 2.0f;
        scene.useCameraRotation = false;
        scene.cameraPosition = { 0.0f, 1.0f, -3.0f };
        scene.cameraLookAt = { 0.0f, 0.0f, 0.0f };
        scene.cameraRotation = { 0.0f, 0.0f, 0.0f };
        scene.character4Position = { 0.0f, 0.0f, 0.0f };
        scene.character4Rotation = Math::Quaternion::Identity;
        scene.character4Scale = 1.0f;
        scene.character5Position = { 1.0f, 0.0f, 0.0f };
        scene.character5Rotation = Math::Quaternion::Identity;
        scene.character5Scale = 1.0f;
        scene.character6Position = { 2.0f, 0.0f, 0.0f };
        scene.character6Rotation = Math::Quaternion::Identity;
        scene.character6Scale = 1.0f;
        scene.bombPosition = Math::Vector3::Zero;
        scene.bombEndPosition = scene.bombPosition;
        scene.bombScale = 1.0f;
        scene.earthPosition = { -1.0f, 0.65f, 0.0f };
        scene.earthScale = 1.0f;
        scene.cactusPosition = Math::Vector3::Zero;
        scene.cactusScale = 1.0f;
        scene.saladPosition = Math::Vector3::Zero;
        scene.saladScale = 1.0f;
        scene.clipIndex4 = 0;
        scene.clipIndex5 = -1;
        scene.clipIndex6 = -1;
    }

    mScenes[0].duration = 8.0f;
    mScenes[1].duration = 5.0f;
    mScenes[2].duration = 5.0f;
    mScenes[3].duration = 7.0f;
    mScenes[4].duration = 4.0f;
    mScenes[5].duration = 6.0f;
    mScenes[6].duration = 6.0f;
    mScenes[7].duration = 5.0f;

    // 0-8s
    Scene& scene0 = mScenes[0];
    scene0.useCameraRotation = true;
    scene0.cameraPosition = { 0.79f, 0.91f, -1.00f };
    scene0.cameraRotation = { 6.0f, -8.0f, 0.0f };
    scene0.character4Position = { 0.0f, 0.0f, 0.0f };
    scene0.character4Rotation = EulerDegreesToQuaternion({ 0.0f, -77.0f, 0.0f });
    scene0.character4Scale = 1.0f;
    scene0.character5Position = { 1.0f, 0.0f, -0.1f };
    scene0.character5Rotation = EulerDegreesToQuaternion({ 0.0f, 77.0f, 0.0f });
    scene0.character5Scale = 1.0f;
    scene0.character6Position = { 0.0f, 0.0f, 0.0f };
    scene0.character6Rotation = Math::Quaternion::Identity;
    scene0.character6Scale = 0.001f;
    scene0.bombPosition = { 0.5f, 0.65f, 0.0f };
    scene0.bombEndPosition = { 0.8f, 0.65f, -0.1f };
    scene0.bombScale = 0.5f;
    scene0.earthPosition = { -1.0f, 0.65f, 0.0f };
    scene0.earthScale = 1.0f;
    scene0.cactusPosition = { 0.0f, 0.0f, 0.0f };
    scene0.cactusScale = 0.001f;
    scene0.saladPosition = { 0.0f, 0.0f, 0.0f };
    scene0.saladScale = 0.001f;
    scene0.clipIndex4 = 5;
    scene0.clipIndex5 = 0;
    scene0.clipIndex6 = -1;

    // 8-13s
    Scene& scene1 = mScenes[1];
    scene1.useCameraRotation = true;
    scene1.cameraPosition = { 0.79f, 0.91f, -1.00f };
    scene1.cameraRotation = { 6.0f, -8.0f, 0.0f };
    scene1.character4Position = { 1.0f, 0.0f, -0.1f };
    scene1.character4Rotation = EulerDegreesToQuaternion({ 0.0f, 77.0f, 0.0f });
    scene1.character4Scale = 1.0f;
    scene1.clipIndex4 = 4;
    scene1.character5Position = { 1.0f, 0.0f, -0.1f };
    scene1.character5Rotation = EulerDegreesToQuaternion({ 0.0f, -77.0f, 0.0f });
    scene1.character5Scale = 1.0f;
    scene1.clipIndex5 = 6;
    scene1.character6Scale = -0.001f;
    scene1.bombScale = -0.001f;
    scene1.saladPosition = { 0.6f, 0.6f, -0.2f };
    scene1.saladScale = 2.0f;
    scene1.cactusScale = -0.001f;

    // 13-18s
    Scene& scene2 = mScenes[2];
    scene2.useCameraRotation = true;
    scene2.cameraPosition = { 0.79f, 0.91f, -1.00f };
    scene2.cameraRotation = { 6.0f, -8.0f, 0.0f };
    scene2.character4Position = { 0.0f, 0.0f, 0.0f };
    scene2.character4Rotation = EulerDegreesToQuaternion({ 0.0f, -77.0f, 0.0f });
    scene2.character4Scale = 1.0f;
    scene2.clipIndex4 = 2;
    scene2.character5Position = { 1.0f, 0.0f, -0.1f };
    scene2.character5Rotation = EulerDegreesToQuaternion({ 0.0f, 77.0f, 0.0f });
    scene2.character5Scale = 1.0f;
    scene2.clipIndex5 = 1;
    scene2.character6Scale = -0.001f;
    scene2.bombScale = -0.001f;
    scene2.saladScale = -0.001f;
    scene2.cactusScale = -0.001f;

    // 18-25s
    Scene& scene3 = mScenes[3];
    scene3.useCameraRotation = true;
    scene3.cameraPosition = { 0.64f, 0.81f, -0.55f };
    scene3.cameraRotation = { 7.5f, -42.0f, 0.0f };
    scene3.character4Position = { 0.0f, 0.0f, 0.0f };
    scene3.character4Rotation = EulerDegreesToQuaternion({ 0.0f, -77.0f, 0.0f });
    scene3.character4Scale = 1.0f;
    scene3.clipIndex4 = 8;
    scene3.character5Scale = -0.001f;
    scene3.clipIndex5 = -1;
    scene3.character6Scale = -0.001f;
    scene3.clipIndex6 = -1;
    scene3.bombScale = -0.001f;
    scene3.saladScale = -0.001f;
    scene3.cactusScale = -0.001f;

    // 25-29s
    Scene& scene4 = mScenes[4];
    scene4.useCameraRotation = true;
    scene4.cameraPosition = { -1.95f, 2.21f, -4.29f };
    scene4.cameraRotation = { -23.0f, 176.7f, 0.0f };
    scene4.character4Position = { 0.0f, 0.0f, 0.0f };
    scene4.character4Rotation = EulerDegreesToQuaternion({ 0.0f, -77.0f, 0.0f });
    scene4.character4Scale = 1.0f;
    scene4.clipIndex4 = 0;
    scene4.character5Position = { 1.0f, 0.0f, -0.1f };
    scene4.character5Rotation = EulerDegreesToQuaternion({ 0.0f, 77.0f, 0.0f });
    scene4.character5Scale = 1.0f;
    scene4.clipIndex5 = 4;
    scene4.character6Scale = -0.001f;
    scene4.clipIndex6 = -1;
    scene4.bombScale = -0.001f;
    scene4.saladScale = -0.001f;
    scene4.cactusScale = -0.001f;
    scene4.earthScale = -0.001f;

    // 29-35s
    Scene& scene5 = mScenes[5];
    scene5.useCameraRotation = true;
    scene5.cameraPosition = { 0.79f, 0.91f, -1.00f };
    scene5.cameraRotation = { 6.0f, -8.0f, 0.0f };
    scene5.character4Position = { 1.0f, 0.0f, -0.1f };
    scene5.character4Rotation = EulerDegreesToQuaternion({ 0.0f, 77.0f, 0.0f });
    scene5.character4Scale = 1.0f;
    scene5.clipIndex4 = 8;
    scene5.character5Position = Math::Vector3(0.0f, 0.0f, 0.0f);
    scene5.character5Rotation = EulerDegreesToQuaternion({ 0.0f, -77.0f, 0.0f });
    scene5.character5Scale = 1.0f;
    scene5.clipIndex5 = 3;
    scene5.character6Scale = -0.001f;
    scene5.clipIndex6 = -1;
    scene5.bombScale = -0.001f;
    scene5.saladPosition = { 0.6f, 0.6f, -0.2f };
    scene5.saladScale = 2.0f;
    scene5.cactusScale = -0.001f;
    scene5.earthScale = -0.001f;

    // 35-41s
    Scene& scene6 = mScenes[6];
    scene6.useCameraRotation = true;
    scene6.cameraPosition = { 0.21f, 0.82f, -1.53f };
    scene6.cameraRotation = { 10.2f, 2.7f, 0.0f };
    scene6.character4Position = { 1.3f, 0.0f, 0.0f };
    scene6.character4Rotation = EulerDegreesToQuaternion({ 0.0f, 68.0f, 0.0f });
    scene6.character4Scale = 1.0f;
    scene6.clipIndex4 = 1;
    scene6.character5Position = { 0.8f, 0.0f, -0.1f };
    scene6.character5Rotation = EulerDegreesToQuaternion({ 0.0f, 66.0f, 0.0f });
    scene6.character5Scale = 0.5f;
    scene6.clipIndex5 = 0;
    scene6.character6Scale = -0.001f;
    scene6.clipIndex6 = -1;
    scene6.bombScale = -0.001f;
    scene6.cactusPosition = { -0.6f, -0.8f, -0.3f };
    scene6.cactusRotation = EulerDegreesToQuaternion({ 2.0f, 0.0f, 0.0f });
    scene6.cactusScale = 3.1f;
    scene6.saladScale = -0.001f;
    scene6.earthScale = -0.001f;

    // 41-46s
    Scene& scene7 = mScenes[7];
    scene7.useCameraRotation = true;
    scene7.cameraPosition = { 0.42f, 0.82f, -0.51f };
    scene7.cameraRotation = { 4.1f, -32.9f, 0.0f };
    scene7.character4Position = { 0.0f, 0.0f, 0.0f };
    scene7.character4Rotation = EulerDegreesToQuaternion({ 0.0f, -77.0f, 0.0f });
    scene7.character4Scale = 1.0f;
    scene7.clipIndex4 = 3;
    scene7.character5Position = { 1.0f, 0.0f, -0.1f };
    scene7.character5Rotation = EulerDegreesToQuaternion({ 0.0f, 60.0f, 0.0f });
    scene7.character5Scale = 0.5f;
    scene7.clipIndex5 = 2;
    scene7.character6Scale = -0.001f;
    scene7.clipIndex6 = -1;
    scene7.bombScale = -0.001f;
    scene7.saladScale = -0.001f;
    scene7.cactusScale = -0.001f;
    scene7.earthScale = -0.001f;

    float startTime = 0.0f;
    for (int i = 0; i < kSceneCount; ++i)
    {
        Scene& scene = mScenes[i];
        scene.startTime = startTime;
        scene.endTime = startTime + scene.duration;
        startTime = scene.endTime;
    }
}

void GameState::ApplyScene(int sceneIndex)
{
    if (sceneIndex < 0 || sceneIndex >= kSceneCount)
    {
        return;
    }

    SoundEffectManager* sm = SoundEffectManager::Get();
    if (mSequencePlaying)
    {
        sm->Stop(mSceneAudioIds[mCurrentSceneIndex]);
    }

    mCurrentSceneIndex = sceneIndex;
    const Scene& scene = mScenes[sceneIndex];
    mCamera.SetPosition(scene.cameraPosition);
    if (scene.useCameraRotation)
    {
        const Math::Vector3 direction = EulerDegreesToDirection(scene.cameraRotation);
        mCamera.SetDirection(direction);
        mCameraLookAt = scene.cameraPosition + direction;
    }
    else
    {
        mCamera.SetLookAt(scene.cameraLookAt);
        mCameraLookAt = scene.cameraLookAt;
    }

    mCharacter4.transform.position = scene.character4Position;
    mCharacter4.transform.rotation = scene.character4Rotation;
    mCharacter4.transform.scale = Math::Vector3(scene.character4Scale);
    mCharacter5.transform.position = scene.character5Position;
    mCharacter5.transform.rotation = scene.character5Rotation;
    mCharacter5.transform.scale = Math::Vector3(scene.character5Scale);
    mCharacter6.transform.position = scene.character6Position;
    mCharacter6.transform.rotation = scene.character6Rotation;
    mCharacter6.transform.scale = Math::Vector3(scene.character6Scale);

    mBombSphere.transform.position = scene.bombPosition;
    mBombScale = scene.bombScale;
    mBombSphere.transform.scale = Math::Vector3(mBombScale);

    mEarthSphere.transform.position = scene.earthPosition;
    mEarthScale = scene.earthScale;
    mEarthSphere.transform.scale = Math::Vector3(mEarthScale);

    mCactus.transform.position = scene.cactusPosition;
    mCactus.transform.rotation = scene.cactusRotation;
    mCactusScale = scene.cactusScale;
    mCactus.transform.scale = Math::Vector3(mCactusScale);

    mSalad.transform.position = scene.saladPosition;
    mSalad.transform.rotation = scene.saladRotation;
    mSaladScale = scene.saladScale;
    mSalad.transform.scale = Math::Vector3(mSaladScale);
    if (sceneIndex == 1)
    {
        mSalad.transform.rotation = Math::Quaternion::Identity;
    }

    mClipIndex4 = scene.clipIndex4;
    mClipIndex5 = scene.clipIndex5;
    mClipIndex6 = scene.clipIndex6;
    mAnimator4.PlayAnimation(mClipIndex4, true);
    mAnimator5.PlayAnimation(mClipIndex5, true);
    mAnimator6.PlayAnimation(mClipIndex6, true);

    if (mSequencePlaying)
    {
        sm->Play(mSceneAudioIds[mCurrentSceneIndex], false);
    }
}

void GameState::OnPlaySequenceEvent(const Engine3D::Core::Event& e)
{
    mSequencePlaying = true;
    mSceneTimer = 0.0f;
    ApplyScene(0);
}

void GameState::OnSceneSelectedEvent(const Engine3D::Core::Event& e)
{
    const SceneSelectedEvent& sceneEvent = static_cast<const SceneSelectedEvent&>(e);
    mSequencePlaying = true;
    mSceneTimer = mScenes[sceneEvent.sceneIndex].startTime;
    ApplyScene(sceneEvent.sceneIndex);
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
    if (input->IsKeyDown(KeyCode::S))
    {
        mCamera.Walk(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::D))
    {
        mCamera.Strafe(moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::A))
    {
        mCamera.Strafe(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::Q))
    {
        mCamera.Rise(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::E))
    {
        mCamera.Rise(moveSpeed * deltaTime);
    }

    if (input->IsMouseDown(MouseButton::RBUTTON))
    {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
    }

}

