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

    //Sphere1 Mesh
    Mesh sphere1 = MeshBuilder::CreateSphere(20, 20, 0.5f);
    mSphere1.transform.position = { 1.0f, 0.7f, 0.0f };
    mSphere1.meshBuffer.Initialize(sphere1);
	mSphere1.diffuseMapId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/misc/basketball.jpg");

	//Sphere2 Mesh
	Mesh sphere2 = MeshBuilder::CreateSphere(20, 20, 0.5f);
    mSphere2.transform.position = { -1.0f, 0.7f, 0.0f };
	mSphere2.meshBuffer.Initialize(sphere2);
	mSphere2.diffuseMapId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/misc/basketball.jpg");

    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
    mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
    mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());

    mShadowEffect.Initialize();
    mShadowEffect.SetDirectionalLight(mDirectionalLight);
}
void GameState::Terminate()
{
    mShadowEffect.Terminate();
    mStandardEffect.Terminate();
    mCharacter1.Terminate();
	mCharacter2.Terminate();
    mCharacter3.Terminate();
    mSphere1.Terminate();
	mSphere2.Terminate();
    mGround.Terminate();
}
void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
}

void GameState::Render()
{
    mShadowEffect.Begin();
        mShadowEffect.Render(mCharacter1);
	    mShadowEffect.Render(mCharacter2);
	    mShadowEffect.Render(mCharacter3);
        mShadowEffect.Render(mSphere1);
		mShadowEffect.Render(mSphere2);
    mShadowEffect.End();

    mStandardEffect.Begin();
        mStandardEffect.Render(mCharacter1);
	    mStandardEffect.Render(mCharacter2);
	    mStandardEffect.Render(mCharacter3);
        mStandardEffect.Render(mSphere1);
		mStandardEffect.Render(mSphere2);
        mStandardEffect.Render(mGround);
    mStandardEffect.End();
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
    mStandardEffect.DebugUI();
    ImGui::Separator();
    mShadowEffect.DebugUI();
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

