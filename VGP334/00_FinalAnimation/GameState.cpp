#include "GameState.h"
#include "GameEvents.h"
using namespace Engine3D;
using namespace Engine3D::Graphics;
using namespace Engine3D::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    mDirectionalLight.direction = Math::Normalize({ 1.0f,-1.0f,1.0 });
    mDirectionalLight.ambient = { 0.4f,0.4f,0.4f,1.0f };
    mDirectionalLight.diffuse = { 0.7f,0.7f,0.7f,1.0f };
    mDirectionalLight.specular = { 0.9f,0.9f,0.9f,1.0f };

    mCharacter4.Initialize(L"../../Assets/Models/Character04/Character04.model");
    mCharacter5.Initialize(L"../../Assets/Models/Character05/Character05.model");
    mCharacter6.Initialize(L"../../Assets/Models/Character06/Character06.model");

    mCharacter5.transform.position = { 1.0f, 0.0f, 0.0f };
    mCharacter6.transform.position = { 2.0f, 0.0f, 0.0f };

    ModelManager* mm = ModelManager::Get();
    mCharacter4.Initialize("Character04/Character04.model");
    mCharacter5.Initialize("Character05/Character05.model");
    mCharacter6.Initialize("Character06/Character06.model");


    mCharacter4.animator = &mAnimator4;
    mCharacter5.animator = &mAnimator5;
    mCharacter6.animator = &mAnimator6;


    mm->AddAnimation(mCharacter4.modelId, L"../../Assets/Animations/Character04/C4Idle.animset");
    mm->AddAnimation(mCharacter4.modelId, L"../../Assets/Animations/Character04/C4Talking.animset");
    mm->AddAnimation(mCharacter4.modelId, L"../../Assets/Animations/Character04/C4Angry.animset");
    mm->AddAnimation(mCharacter4.modelId, L"../../Assets/Animations/Character04/C4Walking.animset");
    mm->AddAnimation(mCharacter4.modelId, L"../../Assets/Animations/Character04/C4HoldingSalad.animset");
    mm->AddAnimation(mCharacter4.modelId, L"../../Assets/Animations/Character04/C4GivingSmt.animset");
    mm->AddAnimation(mCharacter4.modelId, L"../../Assets/Animations/Character04/C4GettingPunched.animset");
    mm->AddAnimation(mCharacter4.modelId, L"../../Assets/Animations/Character04/C4GoCrazy.animset");
    mm->AddAnimation(mCharacter5.modelId, L"../../Assets/Animations/Character05/Talking05.animset");
    mm->AddAnimation(mCharacter6.modelId, L"../../Assets/Animations/Character06/C6Crazy.animset");

    mAnimator4.Initialize(mCharacter4.modelId);
    mAnimator5.Initialize(mCharacter5.modelId);
    mAnimator6.Initialize(mCharacter6.modelId);

    InitializeScenes();
    ApplyScene(0);

    EventManager* em = EventManager::Get();
    mPlaySequenceListenerId = em->AddListener(PlaySequenceEvent::StaticGetTypeId(),
        std::bind(&GameState::OnPlaySequenceEvent, this, std::placeholders::_1));
    mSceneSelectedListenerId = em->AddListener(SceneSelectedEvent::StaticGetTypeId(),
        std::bind(&GameState::OnSceneSelectedEvent, this, std::placeholders::_1));

    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

}
void GameState::Terminate()
{
    EventManager* em = EventManager::Get();
    em->RemoveListener(PlaySequenceEvent::StaticGetTypeId(), mPlaySequenceListenerId);
    em->RemoveListener(SceneSelectedEvent::StaticGetTypeId(), mSceneSelectedListenerId);

    mCharacter4.Terminate();
    mCharacter5.Terminate();
    mCharacter6.Terminate();

    mStandardEffect.Terminate();
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
        mSceneTimer += deltaTime;
        while (mSceneTimer >= mScenes[mCurrentSceneIndex].duration)
        {
            mSceneTimer -= mScenes[mCurrentSceneIndex].duration;
            const int nextScene = mCurrentSceneIndex + 1;
            if (nextScene >= kSceneCount)
            {
                mSequencePlaying = false;
                mSceneTimer = 0.0f;
                break;
            }
            ApplyScene(nextScene);
        }
    }

    UpdateCamera(deltaTime);
    mAnimator4.Update(deltaTime * mAnimationSpeed4);
    mAnimator5.Update(deltaTime * mAnimationSpeed5);
    mAnimator6.Update(deltaTime * mAnimationSpeed6);

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

        SimpleDraw::AddGroundPlane(20.0f, Colors::DarkGray);
        SimpleDraw::Render(mCamera);
        
    }
    else
    {
        SimpleDraw::AddGroundPlane(20.0f, Colors::DarkGray);
        SimpleDraw::Render(mCamera);

        mStandardEffect.Begin();
        mStandardEffect.Render(mCharacter4);
        mStandardEffect.Render(mCharacter5);
        mStandardEffect.Render(mCharacter6);

        mStandardEffect.End();
    }
   
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::DragFloat3("DirectionalLight", &mDirectionalLight.direction.x, 0.01f))
        {
            mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
        }

        ImGui::ColorEdit4("Ambient#Light", &mDirectionalLight.ambient.r);
        ImGui::ColorEdit4("Diffuse#Light", &mDirectionalLight.diffuse.r);
        ImGui::ColorEdit4("Specular#Light", &mDirectionalLight.specular.r);
    }

    if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
    {
        for (uint32_t i = 0; i < mCharacter4.renderObjects.size(); i++)
        {
            Material& material = mCharacter4.renderObjects[i].material;
            std::string renderObjectId = "RenderObject" + std::to_string(i);
            ImGui::PushID(renderObjectId.c_str());
            if (ImGui::CollapsingHeader("RenderObject"))
            {
                ImGui::LabelText("Label", "Material: ");
                ImGui::ColorEdit4("Emissive#Material", &material.emissive.r);
                ImGui::ColorEdit4("Ambient#Material", &material.ambient.r);
                ImGui::ColorEdit4("Diffuse#Material", &material.diffuse.r);
                ImGui::ColorEdit4("Specular#Material", &material.specular.r);
                ImGui::DragFloat("Shininess#Material", &material.shininess, 0.01f, 0.01f, 1000.0f);
            }
            ImGui::PopID();
        }
    }

    ImGui::Checkbox("DrawSkeleton", &mDrawSkeleton);
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

    mStandardEffect.DebugUI();
    ImGui::End();
}

void GameState::InitializeScenes()
{
    for (int i = 0; i < kSceneCount; ++i)
    {
        Scene& scene = mScenes[i];
        scene.duration = 2.0f;
        scene.cameraPosition = { 0.0f, 1.0f, -3.0f };
        scene.cameraLookAt = { 0.0f, 0.0f, 0.0f };
        scene.character4Position = { 0.0f, 0.0f, 0.0f };
        scene.character5Position = { 1.0f, 0.0f, 0.0f };
        scene.character6Position = { 2.0f, 0.0f, 0.0f };
    }

    mScenes[0].clipIndex = 0;
    mScenes[1].clipIndex = 1;
    mScenes[2].clipIndex = 2;
    mScenes[3].clipIndex = 3;
    mScenes[4].clipIndex = 4;
    mScenes[5].clipIndex = 5;
    mScenes[6].clipIndex = 6;
    mScenes[7].clipIndex = 7;
}

void GameState::ApplyScene(int sceneIndex)
{
    if (sceneIndex < 0 || sceneIndex >= kSceneCount)
    {
        return;
    }

    mCurrentSceneIndex = sceneIndex;
    const Scene& scene = mScenes[sceneIndex];
    mCamera.SetPosition(scene.cameraPosition);
    mCamera.SetLookAt(scene.cameraLookAt);

    mCharacter4.transform.position = scene.character4Position;
    mCharacter4.transform.rotation = scene.character4Rotation;
    mCharacter5.transform.position = scene.character5Position;
    mCharacter5.transform.rotation = scene.character5Rotation;
    mCharacter6.transform.position = scene.character6Position;
    mCharacter6.transform.rotation = scene.character6Rotation;

    mClipIndex4 = scene.clipIndex;
    mAnimator4.PlayAnimation(mClipIndex4, true);
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
    mSceneTimer = 0.0f;
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

