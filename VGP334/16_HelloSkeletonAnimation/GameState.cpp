#include "GameState.h"
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

    mCharacter1.Initialize(L"../../Assets/Models/Character01/Character01.model");
    ModelManager* mm = ModelManager::Get();
    mCharacter1.Initialize("Character01/Character01.model");
    mCharacter1.animator = &mAnimator;
    mm->AddAnimation(mCharacter1.modelId, L"../../Assets/Animations/Character01/SambaDancing.animset");

    mAnimator.Initialize(mCharacter1.modelId);

    //mCharacter2.Initialize(L"../../Assets/Models/Character02/Character02.model");
    //mCharacter3.Initialize(L"../../Assets/Models/Character03/Character03.model");

    //mCharacter2.transform.position = { 2.0f, 0.0f, 0.0f };
    //mCharacter3.transform.position = { -2.0f, 0.0f, 0.0f };

    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

}
void GameState::Terminate()
{
    mCharacter1.Terminate();
    //mCharacter2.Terminate();
    //mCharacter3.Terminate();
    mStandardEffect.Terminate();
}
void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
    mAnimator.Update(deltaTime * mAnimationSpeed);
}

void GameState::Render()
{
    if (mDrawSkeleton)
    {
        AnimationUtil::BoneTransforms boneTransforms;
        AnimationUtil::ComputeBoneTransforms(mCharacter1.modelId, boneTransforms, &mAnimator);
        //AnimationUtil::ComputeBoneTransforms(mCharacter2.modelId, boneTransforms);
        //AnimationUtil::ComputeBoneTransforms(mCharacter3.modelId, boneTransforms);
        AnimationUtil::DrawSkeleton(mCharacter1.modelId, boneTransforms);
        //AnimationUtil::DrawSkeleton(mCharacter2.modelId, boneTransforms);
        //AnimationUtil::DrawSkeleton(mCharacter3.modelId, boneTransforms);
        SimpleDraw::AddGroundPlane(20.0f, Colors::DarkGray);
        SimpleDraw::Render(mCamera);
        
    }
    else
    {
        SimpleDraw::AddGroundPlane(20.0f, Colors::DarkGray);
        SimpleDraw::Render(mCamera);

        mStandardEffect.Begin();
        mStandardEffect.Render(mCharacter1);
        //mStandardEffect.Render(mCharacter2);
        //mStandardEffect.Render(mCharacter3);
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
        for (uint32_t i = 0; i < mCharacter1.renderObjects.size(); i++)
        {
            Material& material = mCharacter1.renderObjects[i].material;
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
    ImGui::DragFloat("AnimSpeed", &mAnimationSpeed, 0.01f, 0.0f, 10.0f);
    int maxAnimations = mAnimator.GetAnimationCount();
    if (ImGui::DragInt("AnimIndex", &mClipIndex, 1, -1, maxAnimations - 1))
    {
        mAnimator.PlayAnimation(mClipIndex, true);
    }


    mStandardEffect.DebugUI();
    ImGui::End();
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

