#include "GameState.h"
using namespace Engine3D;
using namespace Engine3D::Graphics;
using namespace Engine3D::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f, -4.0f });
    mCamera.SetLookAt({ 0.0f, 1.0f, 0.0f });

    mDirectionalLight.direction = Math::Normalize({ 1.0f,-1.0f, 1.0 });
    mDirectionalLight.ambient = { 0.4f,0.4f,0.4f,1.0f };
    mDirectionalLight.diffuse = { 0.7f,0.7f,0.7f,1.0f };
    mDirectionalLight.specular = { 0.9f,0.9f,0.9f,1.0f };

    //Characters
    mCharacter1.Initialize(L"../../Assets/Models/Character01/Character01.model");
    mCharacter2.Initialize(L"../../Assets/Models/Character02/Character02.model");
    mCharacter3.Initialize(L"../../Assets/Models/Character03/Character03.model");

    
    mCharacter2.transform.position = { 2.0f, 0.0f, 0.0f };
    mCharacter3.transform.position = { -2.0f, 0.0f, 0.0f };
    
    //Ground Mesh
    Mesh groundMesh = MeshBuilder::CreatePlane(10, 10, 1.0f, true);
    mGround.meshBuffer.Initialize(groundMesh);
    mGround.diffuseMapId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/misc/concrete.jpg");
    
    //Screen Quad
    MeshPX screenQuad = MeshBuilder::CreateScreenQuadPX();
	mScreenQuad.meshBuffer.Initialize(screenQuad);


    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);

    shaderFile = L"../../Assets/Shaders/PostProcessing.fx";
    mPostProcessingEffect.Initialize(shaderFile);
    mPostProcessingEffect.SetTexture(&mRenderTarget);
    mPostProcessingEffect.SetTexture(&mCombineTexture, 1);


    GraphicsSystem* gs = GraphicsSystem::Get();
    const uint32_t screenWidth = gs->GetBackBufferWidth();
	const uint32_t screenHeight = gs->GetBackBufferHeight();
    mRenderTarget.Initialize(screenWidth, screenHeight, RenderTarget::Format::RGBA_U8);

    mCombineTexture.Initialize(L"../../Assets/Textures/clouds.png");
}
void GameState::Terminate()
{
    mCombineTexture.Terminate();
    mRenderTarget.Terminate();
    mGround.Terminate();
    mScreenQuad.Terminate();
    mCharacter1.Terminate();
	mCharacter2.Terminate();
    mCharacter3.Terminate();
    mPostProcessingEffect.Terminate();
    mStandardEffect.Terminate();
}
void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
}

void GameState::Render()
{
    mRenderTarget.BeginRender();

        mStandardEffect.Begin();
            mStandardEffect.Render(mCharacter1);
	        mStandardEffect.Render(mCharacter2);
	        mStandardEffect.Render(mCharacter3);
            mStandardEffect.Render(mGround);
        mStandardEffect.End();
    mRenderTarget.EndRender();

    mPostProcessingEffect.Begin();
		mPostProcessingEffect.Render(mScreenQuad);
	mPostProcessingEffect.End();
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
    ImGui::Separator();
    ImGui::Text("Render Target");
    ImGui::Image(
        mRenderTarget.GetRawData(),
        { 128,128 },
        { 0,0 },
        { 1,1 },
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 });
    mStandardEffect.DebugUI();
    mPostProcessingEffect.DebugUI();
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

